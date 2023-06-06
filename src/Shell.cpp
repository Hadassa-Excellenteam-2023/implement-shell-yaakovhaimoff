#include "Shell.h"

void Shell::displayPrompt() {
    cout << userNameColor << User::getCurrentUser() << resetColor
         << " " << directoryColor << Directory::getCurrentDirectory() << resetColor;
    m_branch = Git::getCurrentBranch();

    // If the current directory is a git repository, display the current m_branch
    if (!m_branch.empty()) {
        cout << " " << branchColor << "(" << m_branch << ")" << resetColor;
    }

    cout << " >> ('exit' to quit): ";
}

void Shell::run() {
    while (true) {

        displayPrompt();

        std::getline(cin, m_command);

        if (m_command == "exit") {
            break;
        } else if (m_command.substr(0, 3) == "cd ") {
            m_directory = m_command.substr(3);
            Directory::changeDirectory(m_previousDir, m_directory);
        } else {
            Command::execute(m_command);
        }
    }
}
