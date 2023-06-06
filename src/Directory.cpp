#include "Directory.h"

string Directory::getCurrentDirectory() {
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

void Directory::changeDirectory(string &previousDir, const string &directory) {
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