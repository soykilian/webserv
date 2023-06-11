#include "BaseClass.hpp"

// BASE
//
// class Base;
// template <typename T> class BaseField;

Base::Base()
{
    this->setIsPath(false);
    this->m_isSet = false;
}

Base::~Base() {}

bool Base::isSet() const { return this->m_isSet; }

bool Base::isPath() const { return this->m_isPath; }

void Base::setIsPath(bool isPath) { this->m_isPath = isPath; }

void Base::setIsSet(bool isSet) { this->m_isSet = isSet; }

std::string Base::getRoot() { return this->root; }

void Base::setRoot(std::string str) { this->root = str; }
