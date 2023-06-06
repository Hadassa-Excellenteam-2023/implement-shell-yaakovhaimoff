#include "User.h"

string User::getCurrentUser() {
    char *userName = getenv("USER");
    if (userName != nullptr) {
        return {userName};
    } else {
        return "";
    }
}