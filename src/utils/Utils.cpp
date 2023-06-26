#include "Utils.hpp"
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

static void initMap()
{
    std::ifstream mimeFile;
    std::string   line;
    mimeFile.open("./src/utils/mimetypes.txt");

    if (!mimeFile.is_open())
    {
        std::cout << "Error opening mime types file" << std::endl;
        return;
    }
    while (std::getline(mimeFile, line))
    {
        std::string key    = line.substr(0, line.find_first_of(":"));
        std::string value  = line.substr(line.find_first_of(":") + 1);
        ft::mimeTypes[key] = value;
    }
}

bool ft::is_number(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}

std::string ft::concatPath(std::string root, std::string path)
{
    if (root.empty() || path.empty())
        return root + path;

    // Remove any trailing slashes from the root and leading slashes from the
    // path
    if (root.back() == '/')
        root.pop_back();

    if (path.front() == '/')
        path = path.substr(1);

    // Concatenate the root and path with a single slash between them
    return root + "/" + path;
}

std::string ft::removeRootFromPath(std::string root, std::string path)
{
    if (root.empty() || path.empty())
        return path;

    // Remove the root from the start of the path
    //  Example:
    //  root = "/home/user" path = "/home/user/file.txt"
    //  result = "/file.txt"
    if (path.find(root) == 0)
        path = path.substr(root.length());

    return path;
}

bool ft::isDirectory(std::string path)
{
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) != 0)
        return false;
    return S_ISDIR(statbuf.st_mode);
}

std::string ft::getMimeType(std::string path)
{
    std::string extension = path.substr(path.find_last_of(".") + 1);
    if (mimeTypes.empty())
        initMap();

    if (mimeTypes.find(extension) == mimeTypes.end())
        return "text/plain";
    return mimeTypes[extension];
}
