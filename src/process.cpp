#include <unistd.h>

#include <cctype>
#include <iostream>
#include <sstream>
#include <vector>
using std::cout;

#include "linux_parser.h"
#include "process.h"
namespace Parser = LinuxParser;

using std::string;
using std::to_string;
using std::vector;

// TODO: Constructor from pid
Process::Process(int pid) : pid_(pid) {
  user_ = Parser::User(pid_);
  command_ = Parser::Command(pid);
}

// Copy Constructor
Process::Process(const Process& a)
    : pid_(a.pid_),
      user_(a.user_),
      command_(a.command_),
      lastProcJiffies_(a.lastProcJiffies_),
      lastCpuJiffies_(a.lastCpuJiffies_) {}

// Move constructor
Process::Process(Process&& a) noexcept
    : pid_(a.pid_),
      user_(a.user_),
      command_(a.command_),
      lastProcJiffies_(a.lastProcJiffies_),
      lastCpuJiffies_(a.lastCpuJiffies_) {}

// Move assignment
Process& Process::operator=(Process&& a) noexcept {
  if (&a == this) return *this;
  pid_ = a.pid_;
  user_ = a.user_;
  command_ = a.command_;
  lastProcJiffies_ = a.lastProcJiffies_;
  lastCpuJiffies_ = a.lastCpuJiffies_;
  return *this;
}

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return the user (name) that generated this process
string Process::User() { return user_; }

// DONE: Return the command that generated this process
string Process::Command() { return command_; }

// DONE: Return this process's CPU utilization as fraction
float Process::CpuUtilization() {
  long int currentProcjiffies = Parser::ActiveJiffies(pid_);
  long int deltaProcJiffies = currentProcjiffies - lastProcJiffies_;
  //cout << pid_ << ":\t";
  //cout << currentProcjiffies << " - " << lastProcJiffies_ << " = " << deltaProcJiffies << " |\t";
  lastProcJiffies_ = currentProcjiffies;

  long int currentCpuJiffies = Parser::Jiffies();
  long int deltaCpuJiffies = currentCpuJiffies - lastCpuJiffies_;
  //cout << currentCpuJiffies << " - " << lastCpuJiffies_ << " = " << deltaCpuJiffies << " \n ";
  lastCpuJiffies_ = currentCpuJiffies;

  if (deltaCpuJiffies <= 0) return 0.0;
  //else
  return float(deltaProcJiffies) / float(deltaCpuJiffies);
}

// DONE: Return this process's memory utilization
string Process::Ram() { return Parser::Ram(pid_); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return Parser::UpTime() - Parser::UpTime(pid_); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process& a) {
  return (this->CpuUtilization() > a.CpuUtilization());
}