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
  std::ifstream fileStream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (fileStream.is_open()) {
    std::string line;
    while (std::getline(fileStream, line)) {
      std::istringstream linestream(line);
      std::string key, value;
      linestream >> key >> value;
      if (key == _key + ":") {
        return value;
      }
    }
    fileStream.close();
  }
  return "";
}

// DONE: Compute and return memory utilization in percentage
float LinuxParser::MemoryUtilization() {
  return (std::stof(LinuxParser::meminfoParser("Active")) /
          std::stof(LinuxParser::meminfoParser("MemTotal")));
}

// DONE: Read and return the system uptime in seconds
long int LinuxParser::UpTime() {
  std::ifstream fileStream(kProcDirectory + kUptimeFilename);
  std::string uptime, idletime;
  if (fileStream.is_open()) {
    std::string line;
    std::getline(fileStream, line);
    fileStream.close();
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
    return std::stol(uptime);
  }
  return 0;
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
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::string line;
    std::string label, data;
    do {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> label >> data;
    } while (label != "processes");
    return std::stoi(data);
    stream.close();
  }
  return -1;  // error
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::string line;
    std::string label, data;
    do {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> label >> data;
    } while (label != "procs_running");
    stream.close();
    return std::stoi(data);
  }
  return -1;  // error
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
  std::ifstream fileStream(kProcDirectory + kVersionFilename);
  if (fileStream.is_open()) {
    std::getline(fileStream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  fileStream.close();
  return kernel;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<std::string> fields{};
  std::ifstream fileStream(kProcDirectory + kStatFilename);
  if (fileStream.is_open()) {
    std::string line;
    std::getline(fileStream, line);
    fileStream.close();
    std::istringstream linestream(line);
    std::string field;
    while (linestream >> field) {
      fields.push_back(field);
    }
    fileStream.close();
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
  std::ifstream fileStream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (fileStream.is_open()) {
    std::string line;
    std::getline(fileStream, line);
    fileStream.close();

    std::string field;
    vector<std::string> fields;
    std::istringstream linestream(line);
    while (linestream >> field) {
      fields.push_back(field);
    }
    if (fields.size() == 52) {
      long active = std::stol(fields[PidStatFields::kUtime]) +
                    std::stol(fields[PidStatFields::kStime]) +
                    std::stol(fields[PidStatFields::kCutime]) +
                    std::stol(fields[PidStatFields::kCstime]);
      return active;
    }
  }
  return -1;  // if error
}

// DONE: Read and return the number of active jiffies for the system
long int LinuxParser::ActiveJiffies() {
  vector<string> fields = CpuUtilization();
  long int totalJiffies = 0;
  for (uint i = 1; i < fields.size(); i++) {
    totalJiffies += std::stol(fields[i]);
  }
  return totalJiffies - std::stol(fields[CPUStates::kIdle_]);
}

// DONE: Read and return the number of idle jiffies for the system
long int LinuxParser::IdleJiffies() {
  vector<string> fields = CpuUtilization();
  return std::stol(fields[CPUStates::kIdle_]);
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(const int pid) {
  return pidStatusParser(pid, "Name");
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(const int pid) {
  return pidStatusParser(pid, "VmSize");
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(const int pid) { return pidStatusParser(pid, "Uid"); }

// DONE: Read and return the user associated with a process
string LinuxParser::User(const int pid) {
  std::string test_uid = Uid(pid);
  std::ifstream fileStream(kPasswordPath);
  if (fileStream.is_open()) {
    std::string line, name, pwd, uid;
    while (std::getline(fileStream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> name >> pwd >> uid;
      if (uid == test_uid) {
        return name;
      }
    }
  }
  return "User not found!";
}

// DONE: Read and return the uptime (start time?) of a process in seconds
long int LinuxParser::UpTime(const int pid) {
  std::ifstream fileStream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (fileStream.is_open()) {
    std::string line;
    std::getline(fileStream, line);
    fileStream.close();

    std::string field;
    vector<std::string> fields;
    std::istringstream linestream(line);
    while (linestream >> field) {
      fields.push_back(field);
    }
    return std::stol(fields[PidStatFields::kStartTime]) / sysconf(_SC_CLK_TCK);
  }
  return -1;  // if error
}
