#include "Utils.hpp"
#include <Response.hpp>
#include <fstream>
#include <string>
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

    (void)code;

    if (!this->server.getErrorPage().empty())
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
        return "";
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
    std::ofstream          file;
    std::string            route;
    std::string            message;
    std::cout << "traza" << std::endl;
    if (this->server.getFileEnd().empty())
        return getErrorPage("400");

    route = ft::concatPath(server.getFileEnd(),this->request->getRoute());
    std::cout << "Route: " << route << std::endl;
    if (access(route.c_str(), F_OK) == 1)
    {
        return getErrorPage("400");
    }
    /*POST*/
    if (flag == 1)
    {
        file.open(route);
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
    env["PATH_TRANSLATED"] =
        ft::concatPath(this->server.getRoot(), env["PATH_INFO"]);
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

std::vector<std::string> getEnvVector(char **env)
{
    std::vector<std::string> result;
    int                      i = 0;

    while (env[i])
    {
        result.push_back(env[i]);
        i++;
    }
    return result;
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
    int fd[2];
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
    std::string route = ft::concatPath(this->server.getRoot(),this->request->getRoute());
    int         index       = route.find(".php");
    int         query_index = route.find("?");
    

    // if (route.length() != static_cast<size_t>(index) + 4)
    // {
    //     std::cout << route.length() << " " << index << std::endl;
    //     if (route.at(index + 4) != 47)
    //         return getErrorPage("404");
//    }
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

std::string Response::getResponse()
{
    std::string message  = "HTTP/1.1 200 OK\r\n";
    int         phpIndex = this->request->getRoute().find(".php");
    std::string body;
    std::string line;
    short       flag = 0;
    if (this->request->getVersion().compare("HTTP/1.1") != 0 
    || this->request->getVersion().compare("HTTP/1.0") != 0)
        return getErrorPage("505");
    this->serversByHost =
        this->server.getServerByHost(this->request->getHost());
    if (!this->server.isAllowedMethodByPath(this->request, this))
        return getErrorPage("405");
    // TODO: server by host
    if (phpIndex != -1)
        if  ((int(this->request->getRoute().length()) > phpIndex+4
        && this->request->getRoute().at(phpIndex + 4) == 47)
        || (int(this->request->getRoute().length()) == phpIndex+4))
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
