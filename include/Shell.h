#pragma once

#include "macros.h"
#include "Command.h"
#include "Directory.h"
#include "Git.h"
#include "User.h"


class Shell {
    string m_branch;
    string m_command;
    string m_directory;
    string m_previousDir;

public:
    void run();

    void displayPrompt();
};
