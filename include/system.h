#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  Processor& Cpu();                   // DONE: See src/system.cpp
  std::vector<Process>& Processes();  // DONE: See src/system.cpp
  void sort();                        // ADDED: For sorting process list by (cpu)
  float MemoryUtilization();          // DONE: See src/system.cpp
  long UpTime();                      // DONE: See src/system.cpp
  int TotalProcesses();               // DONE: See src/system.cpp
  int RunningProcesses();             // DONE: See src/system.cpp
  std::string Kernel();               // DONE: See src/system.cpp
  std::string OperatingSystem();      // DONE: See src/system.cpp

  // TODO: Define any necessary private members
 private:
  Processor cpu_;
  std::vector<int> pids_;
  std::vector<Process> processes_;
  std::string kernel_, OS_;
};

#endif