#pragma once

#include "macros.h"

class Directory {
    static string m_previousDir;
public:
    static string getCurrentDirectory();

    static void changeDirectory(const string &directory);
};