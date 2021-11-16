#include <unistd.h>
#include <cctype>
#include <sstream>
#include <vector>

#include <iostream>
using std::cout;

#include "process.h"
#include "linux_parser.h"
namespace Parser = LinuxParser;

using std::string;
using std::to_string;
using std::vector;

// TODO: Constructor from pid
Process::Process(int pid) : pid_(pid){
    user_ = Parser::User(pid_);
    command_ = Parser::Command(pid);
}

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return the user (name) that generated this process
string Process::User() { return user_; }

// DONE: Return the command that generated this process
string Process::Command() { return command_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {
/*     const long int Hertz {sysconf(_SC_CLK_TCK)};
    long int systemUptime = Parser::UpTime();
    long int startTime = Parser::UpTime(pid_);
    long int totalTime = Parser::ActiveJiffies(pid_);

    return ((float(totalTime) / float(Hertz)) / float(systemUptime - startTime)); */

    return 100 * (float(Parser::ActiveJiffies(pid_)) / float(Parser::ActiveJiffies()));
}

// DONE: Return this process's memory utilization
string Process::Ram() { return Parser::Ram(pid_); }

// DPNE: Return the age of this process (in seconds)
long int Process::UpTime() { return Parser::UpTime(pid_); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
    return (this->pid_ < a.pid_);
    }