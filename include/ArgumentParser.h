#pragma once

#include "macros.h"

class ArgumentParser {
    static vector<pair<int, int>> m_fds;

public:
    static void resetFds();

    static string resolveCommandPath(const std::string &command);

    static vector<string> parseArguments(const string &command);

    static vector<char *> setArguments(vector<string> &tokens);

    static void redirection(int, int, const std::string &);
};