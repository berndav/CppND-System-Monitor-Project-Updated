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

// DONE: Constructor from pid
Process::Process(int pid) : pid_(pid) {
  user_ = Parser::User(pid_);
  command_ = Parser::Command(pid_);
  startTime_ = Parser::StartTime(pid_);
  lastProcActiveJiffies_ = Parser::ActiveJiffies(pid_);
  lastCpuJiffies_ = Parser::ActiveJiffies();
}

// Copy Constructor
Process::Process(const Process& a)
    : pid_(a.pid_),
      user_(a.user_),
      command_(a.command_),
      startTime_(a.startTime_),
      lastProcActiveJiffies_(a.lastProcActiveJiffies_),
      lastCpuJiffies_(a.lastCpuJiffies_) {}

// Copy assignment
Process& Process::operator=(Process& a) {
  if (&a == this) return *this;
  pid_ = a.pid_;
  user_ = a.user_;
  command_ = a.command_;
  startTime_ = a.startTime_;
  lastProcActiveJiffies_ = a.lastProcActiveJiffies_;
  lastCpuJiffies_ = a.lastCpuJiffies_;
  return *this;
}

// Move constructor
Process::Process(Process&& a) noexcept
    : pid_(a.pid_),
      user_(a.user_),
      command_(a.command_),
      startTime_(a.startTime_),
      lastProcActiveJiffies_(a.lastProcActiveJiffies_),
      lastCpuJiffies_(a.lastCpuJiffies_) {}

// Move assignment
Process& Process::operator=(Process&& a) noexcept {
  if (&a == this) return *this;
  pid_ = a.pid_;
  user_ = a.user_;
  command_ = a.command_;
  startTime_ = a.startTime_;
  lastProcActiveJiffies_ = a.lastProcActiveJiffies_;
  lastCpuJiffies_ = a.lastCpuJiffies_;
  return *this;
}

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return the user (name) that generated this process
string Process::User() { return user_; }

// DONE: Return the command that generated this process
string Process::Command() {
  string commandString = command_;
  if(commandString.size() > 50){
    commandString.resize(50);
    commandString += "...";
  }
  
  return commandString;
}

// DONE: Return this process's CPU utilization as FRACTION of total CPU time.
// ncurses_display::DisplayProcesses multiplies by 100 to get percent.
// updated when Display() updates. [NOT RIGHT NOW]
float Process::CpuUtilization() {
  /*
  // From the mists of time
  long int systemUptime = Parser::UpTime();
  long int activeTicks = Parser::ActiveJiffies(pid_);
  long int procTicks = systemUptime - startTime_;
  float cpuUsage = float(activeTicks) / float(procTicks);

  return cpuUsage;
  */

  // Attempt at updating current measurement
  long int currentProcActiveJiffies = Parser::ActiveJiffies(pid_);
  //long int deltaProcActiveJiffies = currentProcActiveJiffies - lastProcActiveJiffies_; 
  //lastProcActiveJiffies_ = currentProcActiveJiffies;

  long int currentCpuJiffies = Parser::ActiveJiffies();
  //long int deltaCpuJiffies = (currentCpuJiffies - lastCpuJiffies_);
  //deltaCpuJiffies = deltaCpuJiffies > 1 ? deltaCpuJiffies : 1;
  lastCpuJiffies_ = currentCpuJiffies;

// Aaaand I dialed it back to the all time method.
  float currentProcCpuUtilization = float(currentProcActiveJiffies) / float(currentCpuJiffies);

  return currentProcCpuUtilization;
  
}

// DONE: Return this process's memory utilization (IN MB!!!)
string Process::Ram() {
  const int mbDivisor = 2 ^ 10;
  std::string ram_str = Parser::Ram(pid_);
  //cout << ram_str << "\t";
  std::istringstream ram_stringstream(ram_str);
  string value;
  string units;
  ram_stringstream >> value >> units;
  long  memInKb = std::stol(value);
  long memInMB = memInKb / mbDivisor;

  return to_string(memInMB);
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() {
  return (Parser::UpTime() - startTime_) / sysconf(_SC_CLK_TCK);
}

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process& a) {
  //return (this->pid_ < a.pid_); //sort by pid
  return (this->CpuUtilization() > a.CpuUtilization()); // sort by Cpu%
}