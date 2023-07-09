#include "Pipe.h"

std::vector<int> Pipe::m_pipeFds;

void Pipe::createPipes(int count) {
    m_pipeFds.resize(count * 2);

    for (int i = 0; i < count; ++i) {
        if (pipe(&m_pipeFds[i * 2]) == -1) {
            throw std::runtime_error("Failed to create pipe");
        }
    }
}

std::vector<std::string> Pipe::parsePipedCommands(const std::string& command) {
    std::istringstream iss(command);
    std::string token;
    std::vector<std::string> commands;
    while (getline(iss, token, '|')) {
        // Skip empty commands
        if (!token.empty()) {
            commands.push_back(token);
        }
    }
    return commands;
}

void Pipe::executePipe(const std::string& command) {
    // Count the number of pipes in the command
    int pipeCount = int(std::count(command.begin(), command.end(), '|'));
    createPipes(pipeCount);
    std::vector<std::string> commands = parsePipedCommands(command);
    size_t numCommands = commands.size();

    for (size_t i = 0; i < numCommands; ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            throw std::runtime_error("Failed to create child process");
        } else if (pid == 0) {
            // Child process
            if (i < numCommands - 1) {
                // Set up pipe output for the child process (except the last command)
                if (dup2(m_pipeFds[i * 2 + 1], STDOUT_FILENO) == -1) {
                    throw std::runtime_error("Failed to duplicate file descriptor");
                }
            }

            // Set up pipe input for the child process (except the first command)
            if (i > 0) {
                if (dup2(m_pipeFds[(i - 1) * 2], STDIN_FILENO) == -1) {
                    throw std::runtime_error("Failed to duplicate file descriptor");
                }
            }
            closePipes();

            // Parse the command into command name and arguments
            std::vector<std::string> tokens = ArgumentParser::parseArguments(commands[i]);

            // Check if command is empty and return if it is
            if (tokens.empty()) {
                exit(EXIT_SUCCESS);
            }
            // Check if the command exists
            if (ArgumentParser::resolveCommandPath(tokens[0]).empty()) {
                std::cerr << tokens[0] << ": command not found.\n";
                exit(EXIT_FAILURE);
            }
            // Execute the command
            std::vector<char*> args = ArgumentParser::setArguments(tokens);
            execvp(tokens[0].c_str(), args.data());
            exit(EXIT_FAILURE);
        }
    }

    // Close all pipe file descriptors in the parent process
    closePipes();

    // Wait for all child processes to finish
    for (size_t i = 0; i < numCommands; ++i) {
        wait(nullptr);
    }
}

void Pipe::closePipes() {
    for (int m_pipeFd : m_pipeFds) {
        if (close(m_pipeFd) == -1) {
            throw std::runtime_error("Failed to close pipe");
        }
    }
}

