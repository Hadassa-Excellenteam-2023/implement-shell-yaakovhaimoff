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

    void displayPrompt();

    void displayJobs();

public:
    void run();

};
