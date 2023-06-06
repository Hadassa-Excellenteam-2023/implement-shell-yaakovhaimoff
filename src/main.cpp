#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>
#include <vector>
#include <fcntl.h>


using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::vector;

std::string resolveCommandPath(const std::string &command) {
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

vector<string> parseArguments(const string &command) {
    std::istringstream iss(command);
    vector<string> tokens;
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

vector<char *> setArguments(vector<string> &tokens) {
    vector<char *> args(tokens.size() + 1);
    for (size_t i = 0; i < tokens.size(); ++i) {
        args[i] = &tokens[i][0];
    }
    args[tokens.size()] = nullptr;
    return args;
}

void executeCommand(const string &command) {

    // Parse the command into command name and arguments
    vector<string> tokens = parseArguments(command);

    // Check if command is empty and return if it is
    if (tokens.empty()) {
        return;
    }

    // does command exists
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
            cout << "Error executing command.\n";
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS) {
            cout << "Error executing command.\n";
        }
    }
}

string getCurrentDirectory() {
    char currentDir[256];
    if (getcwd(currentDir, sizeof(currentDir)) != nullptr) {
        char *lastSlash = std::strrchr(currentDir, '/');
        if (lastSlash != nullptr) {
            return {lastSlash + 1}; // Skip the slash to get the directory name
        } else {
            return {currentDir};
        }
    } else {
        return "";
    }
}

void changeDirectory(string &previousDir, const string &directory) {
    if (directory == "-") {
        if (previousDir.empty()) {
            cout << "Error: Previous directory not found.\n";
            return;
        }
        if (chdir(previousDir.c_str()) != 0) { cout << "Error changing directory.\n"; }
    } else {
        char currentDir[256];
        if (getcwd(currentDir, sizeof(currentDir)) == nullptr) {
            cout << "Error getting current directory.\n";
            return;
        }
        previousDir = currentDir;
        if (chdir(directory.c_str()) != 0) { cout << "Error changing directory.\n"; }
    }
}

std::string getCurrentBranch() {
    std::string command = "git rev-parse --abbrev-ref HEAD 2>/dev/null";
    FILE *pipe = popen(command.c_str(), "r");
    if (pipe == nullptr) {
        // Error executing command
        return "";
    }

    char buffer[128];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);

    // Remove newline character from the result
    if (!result.empty() && result[result.length() - 1] == '\n') {
        result.erase(result.length() - 1);
    }

    if (result.empty()) {
        // No branch name found
        return "";
    }

    return result;
}

void runShell() {
    const string userNameColor = "\033[93m";  // Light yellow
    const string directoryColor = "\033[95m";  // Pink
    const string branchColor = "\033[92m";  // Light green
    const string resetColor = "\033[0m";  // Reset color
    const string userName = getenv("USER"); // Get the username from the environment variable

    string command; // Variable to store the command
    string branch; // Variable to store the current branch
    string previousDir;  // Variable to store the previous directory
    string directory; // Variable to store the directory to change to

    while (true) {

        cout << userNameColor << userName << resetColor
             << " " << directoryColor << getCurrentDirectory() << resetColor;

        branch = getCurrentBranch();
        if (!branch.empty()) {
            cout << " " << branchColor << "(" << branch << ")" << resetColor;
        }

        cout << " >> ('exit' to quit): ";
        std::getline(cin, command);

        if (command == "exit") {
            break;
        } else if (command.substr(0, 3) == "cd ") {
            directory = command.substr(3);
            changeDirectory(previousDir, directory);
        } else {
            executeCommand(command);
        }
    }
}

int main() {
    runShell();

    return 0;
}