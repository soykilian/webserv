#include "AutoindexField.hpp"
#include <Fields.hpp>
#include <unistd.h>

Location::Location() : BaseField<std::string>()
{
    this->fields["root"]             = new RootField();
    this->fields["index"]            = new IndexField();
    this->fields["allowed_methods"]  = new AllowedMethodsField();
    this->fields["post_folder"]      = new LoadFolderField();
    this->fields["client_body_size"] = new ClientBodySizeField();
    this->fields["error_page"]       = new ErrorPageField();
    this->fields["post_folder"]      = new LoadFolderField();
    this->fields["redirection"]      = new RedirectionField();
    this->fields["autoindex"]        = new AutoindexField();
    this->fields["cgi"]              = new CGIField();
}

Location::~Location() {}

Location::Location(const Location &other) : BaseField<std::string>()
{
    this->fields = other.fields;
}

Location &Location::operator=(const Location &rhs)
{
    if (this == &rhs)
        return *this;
    return *this;
}

int Location::processValue(std::string value)
{
    // check if the path is a valid path
    // if (access(value.c_str(), F_OK) == -1)
    // {
    //     std::cout << "Path does not exist" << std::endl;
    //     return 0;
    // }
    this->setValue(value);
    return 1;
}

std::string Location::getPath() const { return this->getValue(); }

std::string Location::getRoot() const
{
    return ((RootField *)this->fields.at("root"))->getValue();
}

std::string Location::getIndex() const
{
    return ((RootField *)this->fields.at("index"))->getValue();
}

size_t Location::getClientBodySize() const
{
    return (
        dynamic_cast<ClientBodySizeField *>(this->fields.at("client_body_size"))
            ->getValue());
}

bool Location::isClientBodySizeSet() const
{
    return (
        dynamic_cast<ClientBodySizeField *>(this->fields.at("client_body_size"))
            ->isSet());
}

std::string Location::getErrorPage() const
{
    return (dynamic_cast<ErrorPageField *>(this->fields.at("error_page"))
                ->getValue());
}

bool Location::isAutoindexOn() const
{
    return (dynamic_cast<AutoindexField *>(this->fields.at("autoindex"))
                ->validate());
}
bool Location::isCGIOn() const
{
    return (dynamic_cast<CGIField *>(this->fields.at("cgi"))->validate());
}

bool Location::isAllowedMethod(std::string method) const
{
    return (
        dynamic_cast<AllowedMethodsField *>(this->fields.at("allowed_methods"))
            ->validate(method));
}

bool Location::isRootSet() const
{
    return (dynamic_cast<RootField *>(this->fields.at("root"))->isSet());
}

std::string Location::getFileEnd() const
{
    return (dynamic_cast<LoadFolderField *>(this->fields.at("post_folder"))
                ->getValue());
}
RedirectionField *Location::getRedirection() const
{
    return (dynamic_cast<RedirectionField *>(this->fields.at("redirection")));
}
std::ostream &operator<<(std::ostream &out, Location const &location)
{
    out << "Location path: " << location.getPath() << std::endl;
    out << "\t\t\t Root: " << location.getRoot() << std::endl;
    out << "\t\t\t Index: " << location.getIndex() << std::endl;
    out << "\t\t\t Autoindex: " << location.isAutoindexOn() << std::endl;
    out << "\t\t\t Client body size: " << location.getClientBodySize()
        << std::endl;
    out << "\t\t\t Error page: " << location.getErrorPage() << std::endl;
    out << "\t\t\t Post folder: " << location.getFileEnd() << std::endl;
    out << "\t\t\t Redirection: " << location.getRedirection()->getValue()
        << std::endl;
    out << "\t\t\t CGI: " << location.isCGIOn() << std::endl;
    out << "\t\t\t Allowed methods: "
        << dynamic_cast<AllowedMethodsField *>(
               location.fields.at("allowed_methods"))
               ->getValue()
        << std::endl;

    return out;
}
