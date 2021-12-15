#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <vector>

using std::cout;

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// ADDED: utility for parsing /proc/cpuinfo
std::string LinuxParser::cpuinfoParser(std::string _key) {
  std::ifstream filestream(kProcDirectory + kCpuinfoFilename);
  std::string key;
  if (filestream.is_open() && !filestream.eof()) {
    const int keyField{0};
    const int valueField{1};
    std::string line, field;
    char del = ':';
    while (std::getline(filestream, line)) {
      std::stringstream linestream(line);
      vector<string> fields{};
      while (std::getline(linestream, field, del)) {
        fields.push_back(field);
      };
      std::string whitespaces(" \t\f\v\n\r");
      key = fields[keyField];
      key.erase(key.find_last_not_of(whitespaces) + 1);
      if (key == _key) {
        return fields[valueField];
      }
    };
    filestream.close();
  }
  return key;
}

// ADDED: utility for parsing /proc/meminfo
std::string LinuxParser::meminfoParser(std::string _key) {
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  std::string key;
  if (filestream.is_open() && !filestream.eof()) {
    const int keyField{0};
    const int valueField{1};
    std::string line, field;
    char del = ':';
    while (std::getline(filestream, line)) {
      std::stringstream linestream(line);
      vector<string> fields{};
      while (std::getline(linestream, field, del)) {
        fields.push_back(field);
      };
      std::string whitespaces(" \t\f\v\n\r");
      key = fields[keyField];
      key.erase(key.find_last_not_of(whitespaces) + 1);
      if (key == _key) {
        return fields[valueField];
      }
    };
    filestream.close();
  }
  return key;
}

// ADDED: Utility for parsing "[pid]/status" file
std::string LinuxParser::pidStatusParser(const int pid, std::string _key) {
  std::string value = "";
  std::ifstream fStream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (fStream.is_open()) {
    std::string line;
    while (std::getline(fStream, line)) {
      std::istringstream linestream(line);
      std::string key, value;
      linestream >> key >> value;
      if (key == (_key + ":")) {
        return value;
      }
    }
    fStream.close();
  }
  return value;
}

// DONE: Compute and return memory utilization in percentage
float LinuxParser::MemoryUtilization() {
  float memTotal = std::stof(LinuxParser::meminfoParser(meminfoMemTotal));
  float memFree = std::stof(LinuxParser::meminfoParser(meminfoMemFree));

  return (memTotal - memFree) / memTotal;
}

// DONE: Read and return the system uptime in ticks
long int LinuxParser::UpTime() {
  std::ifstream fStream(kProcDirectory + kUptimeFilename);
  std::string uptime, idletime;
  if (fStream.is_open()) {
    std::string line;
    std::getline(fStream, line);
    fStream.close();

    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return long(std::stof(uptime)) * sysconf(_SC_CLK_TCK);
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int totalProcesses = 0;

  std::ifstream fstream(kProcDirectory + kStatFilename);
  if (fstream.is_open()) {
    std::string line;
    while (std::getline(fstream, line)) {
      std::vector<string> fields;
      string field;
      std::istringstream linestream(line);

      while (linestream >> field) {
        fields.push_back(field);
      };
      if (fields[0] == statTotalProcesses) {
        totalProcesses = std::stoi(fields[1]);
        fstream.close();
      };
    };
  }
  return totalProcesses;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int runningProcesses = 0;

  std::ifstream fstream(kProcDirectory + kStatFilename);
  if (fstream.is_open()) {
    std::string line;
    while (std::getline(fstream, line)) {
      std::vector<string> fields;
      string field;
      std::istringstream linestream(line);

      while (linestream >> field) {
        fields.push_back(field);
      };
      if (fields[0] == statRunningProcesses) {
        runningProcesses = std::stoi(fields[1]);
        fstream.close();
      };
    };
  }
  return runningProcesses;
}
// DONE: An example of how to read data from the filesystem

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          filestream.close();
          return value;
        }
      }
    }
  }
  filestream.close();
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream fStream(kProcDirectory + kVersionFilename);
  if (fStream.is_open()) {
    std::getline(fStream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  fStream.close();
  return kernel;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<std::string> fields{};
  std::ifstream fStream(kProcDirectory + kStatFilename);
  if (fStream.is_open()) {
    std::string line;
    std::getline(fStream, line);
    fStream.close();
    std::istringstream linestream(line);
    std::string field;
    while (linestream >> field) {
      fields.push_back(field);
    }
    fStream.close();
  }
  return fields;
}

// DONE: Read and return the number of jiffies for the system
long int LinuxParser::Jiffies() {
  vector<string> fields = CpuUtilization();
  long int totalJiffies = 0;
  for (uint i = 1; i < fields.size(); i++) {
    totalJiffies += std::stol(fields[i]);
  }
  return totalJiffies;
}

// DONE: Read and return the number of active jiffies for a PID
long int LinuxParser::ActiveJiffies(const int pid) {
  std::ifstream fStream(kProcDirectory + std::to_string(pid) + kStatFilename);

  long int activeJiffies;
  if (fStream.is_open()) {
    std::string line;
    std::getline(fStream, line);
    fStream.close();

    std::string field;
    vector<std::string> fields;
    std::istringstream linestream(line);
    while (linestream >> field) {
      fields.push_back(field);
    }
    activeJiffies = std::stol(fields[PidStatFields::kUtime]) +
                    std::stol(fields[PidStatFields::kStime]) +
                    std::stol(fields[PidStatFields::kCutime]) +
                    std::stol(fields[PidStatFields::kCstime]);
  }
  return activeJiffies;
}

// DONE: Read and return the number of active jiffies for the system
long int LinuxParser::ActiveJiffies() {
  vector<string> fields = CpuUtilization();
  long int totalJiffies = 0;
  for (uint i = 1; i < fields.size(); i++) {
    totalJiffies += std::stol(fields[i]);
  }
  return totalJiffies - std::stol(fields[CPUStates::kIdle_]) -
         std::stol(fields[CPUStates::kIOwait_]);
}

// DONE: Read and return the number of idle jiffies for the system
long int LinuxParser::IdleJiffies() {
  vector<string> fields = CpuUtilization();
  return std::stol(fields[CPUStates::kIdle_]) +
         std::stol(fields[CPUStates::kIOwait_]);
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(const int pid) {
  std::string procCmdLine;

  std::ifstream fStream(kProcDirectory + std::to_string(pid) +
                        kCmdlineFilename);

  if (fStream.is_open()) {
    std::getline(fStream, procCmdLine);
    fStream.close();
  }

  return procCmdLine;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(const int pid) {
  return pidStatusParser(pid, pidStatusVmSize);
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(const int pid) {
  return pidStatusParser(pid, pidStatusUid);
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(const int pid) {
  string name = "";
  std::string test_uid = Uid(pid);
  std::ifstream fStream(kPasswordPath);
  if (fStream.is_open()) {
    std::string line, pwd, uid;
    while (std::getline(fStream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> name >> pwd >> uid;
      if (uid == test_uid) {
        return name;
      }
    }
    fStream.close();
  }
  return name;
}

// ADDED: Process Start time getter (in clock ticks)
long int LinuxParser::StartTime(const int pid) {
  long int startTime;
  std::ifstream fStream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (fStream.is_open()) {
    std::string line;
    std::getline(fStream, line);
    fStream.close();

    std::string field;
    vector<std::string> fields;
    std::istringstream linestream(line);
    while (linestream >> field) {
      fields.push_back(field);
    }
    startTime = std::stol(fields[PidStatFields::kStartTime]);
  }
  return startTime;
}

// DONE: Read and return the UPTIME of a process in clock ticks
long int LinuxParser::UpTime(const int pid) {
  long int systemUptime = LinuxParser::UpTime();
  long int processStartTIme = LinuxParser::StartTime(pid);
  long int processUptime = systemUptime - processStartTIme;
  return processUptime;
}