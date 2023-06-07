#include "Directory.h"

string Directory::m_previousDir;

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

void Directory::changeDirectory(const string &directory) {
    if (directory == "-") {
        if (m_previousDir.empty()) {
            cout << "Error: Previous directory not found.\n";
            return;
        }
        if (chdir(m_previousDir.c_str()) != 0) { cout << "Error changing directory.\n"; }
    } else {
        char currentDir[256];
        if (getcwd(currentDir, sizeof(currentDir)) == nullptr) {
            cout << "Error getting current directory.\n";
            return;
        }
        m_previousDir = currentDir;
        if (chdir(directory.c_str()) != 0) { cout << "Error changing directory.\n"; }
    }
}