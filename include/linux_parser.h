#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

using std::string;

namespace LinuxParser {
// Utilities
string cpuinfoParser(string key);
string meminfoParser(string _key);
string pidStatusParser(const int pid, string _key);

// Key Values

const string meminfoActive {"Active"};
const string meminfoMemTotal {"MemTotal"};
const string meminfoMemFree {"MemFree"};


const string pidStatusName {"Name"};
const string pidStatusVmSize {"VmSize"};
const string pidStatusUid {"Uid"};

const string statTotalProcesses {"processes"};
const string statRunningProcesses {"procs_running"};

// Paths
const string kProcDirectory{"/proc/"};
const string kCmdlineFilename{"/cmdline"};
const string kCpuinfoFilename{"/cpuinfo"};
const string kStatusFilename{"/status"};
const string kStatFilename{"/stat"};
const string kUptimeFilename{"/uptime"};
const string kMeminfoFilename{"/meminfo"};
const string kVersionFilename{"/version"};
const string kOSPath{"/etc/os-release"};
const string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long int UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
string OperatingSystem();
string Kernel();

// CPU
enum CPUStates {
  kCpu_ = 0,
  kUser_,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};

std::vector<string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(const int pid);
long IdleJiffies();

// Processes
enum PidStatFields{
  kPid = 0,
  kCommand,
  kState,
  kUtime = 13,
  kStime = 14,
  kCutime = 15,
  kCstime = 16,
  kStartTime = 21
};

string Command(const int pid);
string Ram(const int pid);
string Uid(const int pid);
string User(const int pid);
long int StartTime(const int pid);
long int UpTime(const int pid);
};  // namespace LinuxParser

#endif