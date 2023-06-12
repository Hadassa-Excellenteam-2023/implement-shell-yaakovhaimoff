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


void Shell::displayJobs() {
    auto bgProcesses = MyJobs::getJobs();
    cout << "PID\t\tCommand\t\t\tStatus" << endl;
    for (auto it = bgProcesses.begin(); it != bgProcesses.end(); ++it) {
        int status;
        int processStatus = waitpid(it->getPid(), &status, WNOHANG);
        if (processStatus == -1) {
            std::cout << "Error checking process status for PID: " << it->getPid() << std::endl;
        } else if (processStatus == it->getPid()) {
            MyJobs::eraseJob(int(it - bgProcesses.begin()));
        } else {
            std::cout << it->getPid() << "\t" << it->getCommand() << "\t\t" << "Running" << std::endl;
        }
    }
}

void Shell::run() {
    while (true) {

        displayPrompt();

        std::getline(cin, m_command);

        if (m_command == "exit") {
            break;
        } else if (m_command.substr(0, 3) == "cd ") {
            m_directory = m_command.substr(3);
            Directory::changeDirectory(m_directory);
        } else if (m_command == "myjobs") {
            displayJobs();
        } else {
            if (m_command.ends_with("&")) {
                m_command = m_command.substr(0, m_command.size() - 1);
                Command::execute(m_command, true);
                ArgumentParser::resetFds();
            } else {
                Command::execute(m_command, false);
                ArgumentParser::resetFds();
            }
        }
    }
}
