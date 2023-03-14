#include "Utils.hpp"
#include <iostream>
#include <string>
#include <unistd.h>

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
