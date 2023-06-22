#pragma once

#include "macros.h"
#include "MyJobs.h"
#include "ArgumentParser.h"
#include "Pipe.h"

class Command {

public:
    static void execute(const string &command, bool);
};