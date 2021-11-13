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
      key.erase(key.find_last_not_of(whitespaces)+1);
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
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open() && !stream.eof()) {
    std::string line, key, value;
    do {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> key >> value;
    } while (key != _key + ":");
    stream.close();
    if (key == _key + ":") {
      return value;
    }
  } else {
    return ("key " + _key + " not found!");
  }
  return ("key " + _key + " not found!");
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

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  long uptime, idletime;
  if (stream.is_open()) {
    std::string line;
    std::getline(stream, line);
    stream.close();
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
    return uptime;
  }
  return -1.0;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  std::ifstream stream(kProcDirectory + kUptimeFilename);

  if (stream.is_open()) {
    std::string line;
    std::getline(stream, line);
    stream.close();
    std::istringstream linestream(line);
    std::string label;
    long jif_user, jif_nice, jif_sys, jif_idle;
    linestream >> label >> jif_user >> jif_nice >> jif_sys >> jif_idle;
    if (label == "cpu") return jif_user + jif_sys + jif_idle;
  }

  return -1.0;
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(const int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::string line;
    std::getline(stream, line);
    stream.close();
    int _pid, intbuffer, utime, stime;
    std::string cmdline;
    std::istringstream linestream(line);
    linestream >> _pid >> cmdline;
    for (int i = 3; i < 14; i++) {  // 14th item is utime
      linestream >> intbuffer;
    };
    linestream >> utime >> stime;
    return utime + stime;
  }
  return -1;  // if error
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::string line;
    std::getline(stream, line);
    std::istringstream linestream(line);
    std::string label;
    long jif_user, jif_nice, jif_sys, jif_idle;
    linestream >> label >> jif_user >> jif_nice >> jif_sys >> jif_idle;
    stream.close();
    if (label == "cpu") return jif_user + jif_sys;
  }

  return -1.0;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::string line;
    std::getline(stream, line);
    std::istringstream linestream(line);
    std::string label;
    long jif_user, jif_nice, jif_sys, jif_idle;
    linestream >> label >> jif_user >> jif_nice >> jif_sys >> jif_idle;
    stream.close();
    if (label == "cpu") return jif_idle;
  }
  return -1.0;  // cpu label not found
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  vector<std::string> cpu_utes{};
  if (filestream.is_open()) {
    std::string line;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    std::string label;
    linestream >> label;
    if (label != "cpu") return cpu_utes;  // bail now: first line should be cpu
    std::string item;
    for (int i = 1; i < 11; i++) {
      linestream >> item;
      cpu_utes.push_back(item);
    }
    filestream.close();
    return cpu_utes;
  }
  return cpu_utes;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream stream(kProcDirectory + kCpuinfoFilename);
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
  std::ifstream stream(kProcDirectory + kCpuinfoFilename);
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
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    std::string line, name, pwd, uid;
    do {
      std::getline(stream, line);
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> name >> pwd >> uid;
    } while (uid != test_uid);
    if (uid == test_uid) {
      return name;
    };
  };
  return "User not found!";
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(const int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::string line;
    std::getline(stream, line);
    std::string field;
    for (int i = 0; i < 14; i++) {  // 14th item is utime
      stream >> field;
    };
    return std::stol(field);
  }
  return -1;  // if error
}
