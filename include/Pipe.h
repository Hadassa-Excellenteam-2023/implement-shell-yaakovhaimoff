#pragma once

#include "macros.h"
#include "ArgumentParser.h"

class Pipe {

    static std::vector<int> m_pipeFds;

    static void closePipes();

    static void createPipes(int);

    static std::vector<std::string> parsePipedCommand(const std::string&);

    static std::vector<std::string> parsePipedCommands(const std::string& command);

public:
    static void executePipe(const string&);

};