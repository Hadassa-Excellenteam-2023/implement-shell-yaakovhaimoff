#pragma once

#include "macros.h"

class Command {
    static string resolveCommandPath(const std::string &command);

    static vector<string> parseArguments(const string &command);

    static vector<char *> setArguments(vector<string> &tokens);
public:

    static void execute(const string &command);
};