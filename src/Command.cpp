#include "Command.h"

std::string Command::resolveCommandPath(const std::string &command) {
    // Check if the command is already a full path
    if (command.find('/') == 0 && access(command.c_str(), X_OK) == 0) {
        return command;
    }

    char *pathValue = getenv("PATH");
    if (pathValue == nullptr) {
        std::cerr << "Error: PATH environment variable not found.\n";
        return "";
    }

    std::string pathString(pathValue);
    std::stringstream pathStream(pathString);
    std::string directory;
    while (std::getline(pathStream, directory, ':')) {
        std::string fullCommand = directory.append("/" + command);
        if (access(fullCommand.c_str(), X_OK) == 0) {
            return fullCommand;
        }
    }
    return "";
}

vector<string> Command::parseArguments(const string &command) {
    std::istringstream iss(command);
    vector<string> tokens;
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

vector<char *> Command::setArguments(vector<string> &tokens) {
    vector<char *> args(tokens.size() + 1);
    for (size_t i = 0; i < tokens.size(); ++i) {
        args[i] = &tokens[i][0];
    }
    args[tokens.size()] = nullptr;
    return args;
}

void Command::execute(const string &command, bool isBackground) {
    // Parse the command into command name and arguments
    vector<string> tokens = parseArguments(command);

    // Check if command is empty and return if it is
    if (tokens.empty()) {
        return;
    }

    // Check if the command exists
    if (resolveCommandPath(tokens[0]).empty()) {
        cout << tokens[0] << ": command not found.\n";
        return;
    }

    // Set arguments for execvp
    vector<char *> args = setArguments(tokens);

    pid_t pid = fork();
    if (pid == -1) {
        cout << "Error creating child process.\n";
        return;
    } else if (pid == 0) {
        // Child process
        if (execvp(tokens[0].c_str(), args.data()) == -1) {
            cout << "Error executing command in child.\n";
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        int status;
        if (isBackground) {
            MyJobs::addJob(command, pid);
            return;
        }
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS) {
            cout << "Error executing command in parent.\n";
        }
    }
}