#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Utilities
std::string cpuinfoParser(std::string key);
std::string meminfoParser(std::string _key);
std::string pidStatusParser(const int pid, std::string _key);

// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long int UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

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

std::vector<std::string> CpuUtilization();
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

std::string Command(const int pid);
std::string Ram(const int pid);
std::string Uid(const int pid);
std::string User(const int pid);
long int StartTime(const int pid);
long int UpTime(const int pid);
};  // namespace LinuxParser

#endif