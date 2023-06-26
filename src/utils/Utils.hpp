#ifndef UTILS_HPP
#define UTILS_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <string>

namespace ft
{
static std::map<std::string, std::string> mimeTypes;

bool        is_number(const std::string &s);
std::string concatPath(std::string root, std::string path);
std::string removeRootFromPath(std::string root, std::string path);
bool        isDirectory(std::string path);
std::string getMimeType(std::string path);
} // namespace ft

#endif // !UTILS_HPPDEBUG
