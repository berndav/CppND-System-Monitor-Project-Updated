#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int _pid);
  Process(const Process& a);    //Copy constructor
  Process& operator=(Process& a); // Copy assignment
  Process(Process&& a) noexcept; // Move constructor
  Process& operator=(Process&& a) noexcept; //Move assignment
  int Pid();                   // DONE: initialized on constructor
  std::string User();          // DONE: See src/process.cpp
  std::string Command();       // DONE: See src/process.cpp
  float CpuUtilization();      // DONE: See src/process.cpp
  std::string Ram();           // DONE: See src/process.cpp
  long int UpTime();           // DONE: See src/process.cpp
  bool operator<(Process& a);  // DONE: See src/process.cpp

  long int prActJiff(){return lastProcActiveJiffies_;};
  long int cpuActJiff(){ return lastCpuJiffies_;};

  // DONE: Declare any necessary private members
 private:
  int pid_;
  std::string user_;
  std::string command_;
  long int startTime_;  // in ticks

  long int lastProcActiveJiffies_;
  long int lastCpuJiffies_;
};

#endif