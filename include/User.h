#pragma once

#include "macros.h"

class User {
    static std::string m_userName;
public:
    static std::string getCurrentUser();
};
