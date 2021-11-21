#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <iostream>
#include <set>
using std::cout;

#include "linux_parser.h"
namespace Parser = LinuxParser;

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() {
  pids_ = Parser::Pids();
  for (int pid : pids_) {
    Process new_process(pid);
    processes_.push_back(new_process);
  }
  kernel_ = Parser::Kernel();
  OS_ = Parser::OperatingSystem();
}

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// sort ultility for outside calling
void System::sort() { std::sort(processes_.begin(), processes_.end()); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return Parser::MemoryUtilization(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return Parser::UpTime() / sysconf(_SC_CLK_TCK); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return Parser::TotalProcesses(); }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return Parser::RunningProcesses(); }

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return kernel_; }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return OS_; }