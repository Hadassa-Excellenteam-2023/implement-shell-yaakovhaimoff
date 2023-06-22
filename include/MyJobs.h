#pragma once

#include "macros.h"

class MyJobs {

    string command;
    pid_t pid;

    static vector<MyJobs> m_jobs;

public:
    MyJobs(string command, pid_t pid) : command(std::move(command)), pid(pid) {}

    static vector<MyJobs> getJobs() { return m_jobs; }

    static void addJob(string command, pid_t pid) { m_jobs.emplace_back(command, pid); }

    static void eraseJob(int index) { m_jobs.erase(m_jobs.begin() + index); }

    pid_t getPid() { return pid; }

    string getCommand() { return command; }
};