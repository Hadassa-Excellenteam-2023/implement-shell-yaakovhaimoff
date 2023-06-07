#include "User.h"

std::string User::m_userName = getenv("USER");

std::string User::getCurrentUser() {
    return m_userName;
}
