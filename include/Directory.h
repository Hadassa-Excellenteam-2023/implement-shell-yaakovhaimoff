#pragma once

#include "macros.h"

class Directory {
public:
    static string getCurrentDirectory();

    static void changeDirectory(string &previousDir, const string &directory);
};