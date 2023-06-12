#include "ArgumentParser.h"

vector<pair<int, int>> ArgumentParser::m_fds;

void ArgumentParser::resetFds() {
    for (const auto &pair: m_fds) {
        if (dup2(pair.first, pair.second) == -1) {
            throw std::runtime_error("Failed to restore fd");
        }
        if (close(pair.first) == -1) {
            throw std::runtime_error("Failed to close fd");
        }
    }
    m_fds.clear();
}

string ArgumentParser::resolveCommandPath(const string &command) {
    // Check if the command is already a full path
    if (command.find('/') == 0 && access(command.c_str(), X_OK) == 0) {
        return command;
    }

    char *pathValue = getenv("PATH");
    if (pathValue == nullptr) {
        std::cerr << "Error: PATH environment variable not found.\n";
        return "";
    }

    string pathString(pathValue);
    std::stringstream pathStream(pathString);
    string directory;
    while (std::getline(pathStream, directory, ':')) {
        string fullCommand = directory.append("/" + command);
        if (access(fullCommand.c_str(), X_OK) == 0) {
            return fullCommand;
        }
    }
    return "";
}


vector<string> ArgumentParser::parseArguments(const std::string &command) {
    std::istringstream iss(command);
    string token;
    vector<string> tokens;
    bool input = false; // flag for input redirection
    bool output = false; // flag for output redirection
    while (iss >> token) {
        if (output) {
            // Handle redirection by passing the file name to the redirect function
            redirection(STDOUT_FILENO, O_WRONLY | O_CREAT | O_TRUNC, token);
        } else if (input) {
            // Handle input redirection similarly
            redirection(STDIN_FILENO, O_RDONLY, token);
        } else if (token == ">") {
            output = true;
        } else if (token == "<") {
            input = true;
        } else {
            tokens.push_back(token);
        }
    }
    return tokens;
}

void ArgumentParser::redirection(int newFd, int fMode, const std::string &fileName) {
    int originalFd = dup(newFd);
    if (originalFd == -1) {
        throw std::runtime_error("Failed to duplicate file descriptor");
    }
    int fd = open(fileName.c_str(), fMode, 0666);
    if (fd == -1) {
        throw std::invalid_argument("Failed to open file: " + fileName);
    }
    if (dup2(fd, newFd) == -1) {
        throw std::runtime_error("Failed to redirect file");
    }
    if (close(fd) == -1) {
        throw std::runtime_error("Failed to close file");
    }
    m_fds.emplace_back(originalFd, newFd);
}

vector<char *> ArgumentParser::setArguments(vector<string> &tokens) {
    vector<char *> args(tokens.size() + 1);
    for (size_t i = 0; i < tokens.size(); ++i) {
        args[i] = &tokens[i][0];
    }
    args[tokens.size()] = nullptr;
    return args;
}