#include "Git.h"

std::string Git::getCurrentBranch() {
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