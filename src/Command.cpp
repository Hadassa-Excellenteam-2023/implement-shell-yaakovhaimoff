#include "Command.h"

void Command::execute(const string &command, bool isBackground) {
    // check if command is pipe
    if (command.find('|') != string::npos) {
        Pipe::executePipe(command);
        return;
    }
    // Parse the command into command name and arguments
    vector<string> tokens = ArgumentParser::parseArguments(command);

    // Check if command is empty and return if it is
    if (tokens.empty()) {
        return;
    }

    // Check if the command exists
    if (ArgumentParser::resolveCommandPath(tokens[0]).empty()) {
        cout << tokens[0] << ": command not found.\n";
        return;
    }

    // Set arguments for execvp
    vector<char *> args = ArgumentParser::setArguments(tokens);

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