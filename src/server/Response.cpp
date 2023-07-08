#include "Utils.hpp"
#include <Response.hpp>
#include <fstream>
#include <string>
#include <valarray>
#include <vector>
#define READ_END  0
#define WRITE_END 1
Response::Response(Request *request)
    : request(request), server(request->getServer())
{
}

Response::Response() : server(empty) {}

Response::~Response() {}

Response &Response::operator=(const Response &other)
{
    this->request = other.request;
    return *this;
}

Response::Response(const Response &other)
    : request(other.request), server(other.request->getServer())
{
}

Server const *Response::getServer() const { return this->currentServer; }

Location const *Response::getLocation() const { return this->currentLocation; }

std::string Response::addDate(std::string message)
{
    std::time_t              currentTime = std::time(nullptr);
    std::string              stringTime  = std::ctime(&currentTime);
    std::stringstream        ss(stringTime);
    std::string              token;
    std::vector<std::string> tokens;
    while (ss >> token)
        tokens.push_back(token);
    message += tokens[0] + ", " + tokens[2] + " " + tokens[1] + " " +
               tokens[4] + " " + tokens[3] + " GMT";
    return message;
}

std::string Response::getErrorPage(std::string code)
{
    std::string   message;
    std::string   body;
    std::string   line;
    std::ifstream file;
    std::string   errPage = this->currentLocation != NULL
                                ? (this->currentLocation->getErrorPage().empty()
                                       ? this->currentServer->getErrorPage()
                                       : this->currentLocation->getErrorPage())
                                : this->currentServer->getErrorPage();

    if (!errPage.empty())
    {
        file.open(this->server.getErrorPage());
        if (!file.is_open())
            file.open("./http/" + std::string(code)+ ".html");
    }
    else
        file.open("./http/" + std::string(code)+ ".html");

    if (!file.is_open())
    {
        std::cout << "Error opening error page" << std::endl;
        message = "HTTP/1.1 " + code + " Not Found\r\n";
        message += "Date: ";
        message = addDate(message);
        return message;
    }

    while (std::getline(file, line))
        body += line;

    message = "HTTP/1.1 " + code + " Not Found\r\n";
    message += "Content-Type: text/html\n";
    message += "Content-Length: ";
    message += std::to_string(body.length()) + "\n";
    message += "Date: ";
    message = addDate(message);
    message += "\n\n";
    message += body;

    return message;
}

std::vector<Server const *> Response::getServersByHost() const
{
    return this->serversByHost;
}

std::string Response::fileEdition(int flag)
{
    std::fstream           file;
    std::string            route;
    std::string            message;
    std::string            folder;
    std::string::size_type idx;

    if (!this->currentLocation->getFileEnd().empty())
        folder = ft::concatPath(this->currentLocation->getRoot(),
                                this->currentLocation->getFileEnd());
    else if (this->currentServer->getFileEnd().empty())
        return getErrorPage("600");
    else if (folder.empty())
        folder = ft::concatPath(this->currentServer->getRoot(), folder);

    idx   = this->request->getRoute().find_last_of('/');
    route = folder + "/" + this->request->getRoute().substr(idx + 1);

    /*POST*/
    if (flag == 1)
    {
        file.open(route,
                  std::fstream::in | std::fstream::out | std::fstream::trunc);
        if (!file.is_open())
        {
            std::cout << "Error opening file" << std::endl;
            return getErrorPage("404");
        }
        file << this->request->getBody();
        file.close();
    }
    /*DELETE*/
    else
    {
        int result = std::remove(route.c_str());
        if (result != 0)
        {
            std::cout << "Error deleting file" << std::endl;
            return getErrorPage("404");
        }
        else
            std::cout << "File successfully deleted" << std::endl;
    }
    message = "HTTP/1.1 201 No Content\r\n";
    message += "Date: ";
    message = addDate(message);
    return message;
}

char **Response::set_env()
{
    std::map<std::string, std::string> env;
    env["SERVER_SOFTWARE"]   = "";
    env["SERVER_NAME"]       = this->server.getHost();
    env["GATEWAY_INTERFACE"] = "CGI/version";
    env["SERVER_PROTOCOL"]   = this->request->getVersion();
    env["SERVER_PORT"]       = this->server.getPort();
    env["PATH_INFO"]         = this->request->getRoute();
    env["PATH_TRANSLATED"]   = ft::concatPath(
        this->currentLocation != NULL ? this->currentLocation->getRoot()
                                      : this->currentServer->getRoot(),
        env["PATH_INFO"]);
    env["SCRIPT_NAME"]  = this->cgi_path;
    env["QUERY_STRING"] = this->query_params;
    env["REMOTE_ADDR"]  = this->request->getHeader("X-Forwarded-For");
    if (this->request->getMethod().compare("POST") == 0)
    {
        env["CONTENT_TYPE"]   = this->request->getHeader("Content-Type");
        env["CONTENT_LENGTH"] = this->request->getHeader("Content-Length");
    }
    std::map<std::string, std::string>::iterator it;
    std::vector<char *>                          envMatrix;
    for (it = env.begin(); it != env.end(); ++it)
    {
        std::string variable = it->first + "=" + it->second;
        char       *envEntry = new char[variable.size() + 1];
        std::strcpy(envEntry, variable.c_str());
        envMatrix.push_back(envEntry);
    }
    envMatrix.push_back(NULL);
    char **envArray = new char *[envMatrix.size()];
    std::copy(envMatrix.begin(), envMatrix.end(), envArray);
    for (std::vector<char *>::iterator it = envMatrix.begin();
         it != envMatrix.end(); ++it)
    {
        delete[] (*it);
    }
    return envArray;
}

std::string Response::getPath()
{
    std::string::size_type start = 0;
    std::string::size_type end;
    std::string            path;
    std::string            route = this->php_path;
    char                  *tmp   = getenv("PATH");

    if (!tmp)
    {
        std::cout << "Error getting environment variables" << std::endl;
        return "";
    }
    std::cout << "Route: " << route << std::endl;
    std::string env(tmp);
    end = env.find(":", start);
    while (end != std::string::npos)
    {
        path = env.substr(start, end - start);
        path.append("/php");
        if (access(path.c_str(), F_OK) == 0)
            break;
        start = end + 1;
        end   = env.find(":", start);
    }
    return path;
}

std::string Response::get_cgi()
{

    std::string path = this->getPath();
    if (path.empty())
        return getErrorPage("500");
    char *const args[] = {const_cast<char *>(path.c_str()),
                          const_cast<char *>(this->php_path.c_str()), NULL};
    int         fd[2];
    pipe(fd);
    int pid = fork();
    if (pid == 0)
    {
        close(fd[READ_END]);
        dup2(fd[WRITE_END], STDOUT_FILENO);
        close(fd[WRITE_END]);
        char **env = set_env();
        execve(path.c_str(), args, env);
        delete[] env;
        perror("execve");
        exit(1);
    }
    else
    {
        close(fd[WRITE_END]);
        char        buffer[1024];
        std::string capturedOutput;
        ssize_t     bytesRead;
        while ((bytesRead = read(fd[READ_END], buffer, sizeof(buffer))) > 0)
            capturedOutput.append(buffer, bytesRead);
        close(fd[READ_END]);
        return capturedOutput;
    }
    return getErrorPage("404");
}

std::string Response::processCgi()
{
    std::string route = ft::concatPath(this->currentLocation != NULL
                                           ? this->currentLocation->getRoot()
                                           : this->currentServer->getRoot(),
                                       this->request->getRoute());
    int index       = route.find(".php");
    int query_index = route.find("?");

    if (access(route.c_str(), F_OK) == -1)
    {
        std::cout << "Error opening file" << std::endl;
        return getErrorPage("400");
    }
    this->query_params = "";
    if (query_index != -1)
    {
        this->cgi_path     = route.substr(index + 4, query_index);
        this->query_params = route.substr(query_index);
    }
    else
        this->cgi_path = route.substr(index + 4);
    this->php_path = route.substr(0, index + 4);
    std::cout << "PHP file path: " << this->php_path << std::endl;
    return get_cgi();
}

void Response::setLocationAndServer(std::string path)
{
    Server const                 *curr = &this->server;
    std::valarray<Server const *> servers;
    Location const               *longestMatchLoc  = NULL;
    Server const                 *longestMatchServ = &this->server;
    size_t                        longestMatch     = 0;

    if (this->getServersByHost().size() > 0)
    {
        for (size_t i = 0; i < this->getServersByHost().size(); i++)
        {
            for (size_t j = 0; j < servers[i]->locations.size(); j++)
            {
                if (servers[i]->locations[j]->getValue().length() >
                    longestMatch)
                {
                    longestMatch =
                        servers[i]->locations[j]->getValue().length();
                    longestMatchLoc  = servers[i]->locations[j];
                    longestMatchServ = servers[i];
                }
            }
        }
        this->currentServer   = longestMatchServ;
        this->currentLocation = longestMatchLoc;
        return;
    }

    while (curr)
    {
        for (size_t i = 0; i < curr->locations.size(); i++)
        {
            if ((path.substr(0, curr->locations[i]->getValue().length()) ==
                 curr->locations[i]->getValue()))
            {
                if (curr->locations[i]->getValue().length() > longestMatch)
                {
                    longestMatch     = curr->locations[i]->getValue().length();
                    longestMatchLoc  = curr->locations[i];
                    longestMatchServ = curr;
                }
            }
        }
        curr = curr->next;
    }
    this->currentServer   = longestMatchServ;
    this->currentLocation = longestMatchLoc;
}

std::string Response::getResponse()
{
    std::string message  = "HTTP/1.1 200 OK\r\n";
    int         phpIndex = this->request->getRoute().find(".php");
    std::string body;
    std::string line;
    short       flag = 0;
    if (this->request->getVersion().compare("HTTP/1.1") != 0 
    && this->request->getVersion().compare("HTTP/1.0") != 0)
    {
        std::cout << "Invalid HTTP version" << std::endl;
        return getErrorPage("505");
    }
    this->serversByHost =
        this->server.getServerByHost(this->request->getHost());
    setLocationAndServer(this->request->getRoute());
    if (this->currentLocation != NULL && !this->currentLocation->getRedirection()->getValue().empty())
    {
        RedirectionField *redirection = this->currentLocation->getRedirection();
        std::string code = redirection->getCode();
        std::string uri = redirection->getUri();
        std::string red_message = redirection->getRedirectionMessage();
        message = "HTTP/1.1 " + code + " " + red_message + "\n";
        message += "Location: " + uri + "\r\n";
        message += "Date: ";
        message = addDate(message);
        message += "\n\n";
        return message;
    }
    // Check if the method is allowed
    if (!this->currentServer->isAllowedMethod(request->getMethod()))
        return getErrorPage("405");
    if (this->currentLocation != NULL)
        if (!this->currentLocation->isAllowedMethod(request->getMethod()))
            return getErrorPage("405");

    // TODO: server by host
    if (phpIndex != -1)
        if ((int(this->request->getRoute().length()) > phpIndex + 4 &&
             this->request->getRoute().at(phpIndex + 4) == 47) ||
            (int(this->request->getRoute().length()) == phpIndex + 4))
            return processCgi();
    // TODO: server by host
    if (this->request->getMethod().compare("POST") == 0)
        return fileEdition(1);
    // TODO: server by host
    if (this->request->getMethod().compare("DELETE") == 0)
        return fileEdition(0);
    if (this->request->getMethod().compare("GET") != 0)
        return getErrorPage("405");

    // GET METHOD
    std::string fileName =
        this->server.getResponseFile(this->request, this, &flag);

    // Flag for the directory listing
    if (flag == 1)
    {
        message += "Content-Type: ";
        message += ft::getMimeType("index.html");
        message += "\n";
        message += "Content-Length: ";
        message += std::to_string(fileName.length());
        message += "\n";
        message += "Date: ";
        message = addDate(message);
        message += "\n\n";
        message += fileName;
        return message;
    }

    if (fileName.empty())
        return getErrorPage("404");

    std::ifstream file(fileName);

    if (!file.is_open())
        return getErrorPage("404");

    while (std::getline(file, line))
        body += line;

    message += "Content-Type: ";
    message += ft::getMimeType(fileName);
    message += "\n";
    message += "Content-Length: ";
    message += std::to_string(body.length());
    message += "\n";
    message += "Date: ";
    message = addDate(message);
    message += "\n\n";
    message += body;
#ifdef DEBUG
    std::cout << "*** RESPONSE ***" << std::endl;
    std::cout << message << std::endl;
    std::cout << "*** END RESPONSE ***" << std::endl;
#endif
    return message;
}
