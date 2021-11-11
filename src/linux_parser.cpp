#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
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

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  long uptime, idletime;
  if (stream.is_open()) {
    std::string line;
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
    return uptime;
  }
  return -1.0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  std::ifstream stream(kProcDirectory + kUptimeFilename);

  if (stream.is_open()) {
    std::string line;
    std::getline(stream, line);
    std::istringstream linestream(line);
    std::string label;
    long jif_user, jif_nice, jif_sys, jif_idle;
    linestream >> label >> jif_user >> jif_nice >> jif_sys >> jif_idle;
    if (label == "cpu") return jif_user + jif_sys + jif_idle;
  }

  return -1.0;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::string line;
    std::getline(stream, line);
    int _pid, intbuffer, utime, stime;
    std::string cmdline;
    stream >> _pid >> cmdline;
    for (int i = 3; i < 14; i++) {  // 14th item is utime
      stream >> intbuffer;
    };
    stream >> utime >> stime;
    return utime + stime;
  }
  return -1;  // if error
}

//  Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::string line;
    std::getline(stream, line);
    std::istringstream linestream(line);
    std::string label;
    long jif_user, jif_nice, jif_sys, jif_idle;
    linestream >> label >> jif_user >> jif_nice >> jif_sys >> jif_idle;
    if (label == "cpu") return jif_user + jif_sys;
  }

  return -1.0;
}

//  Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::string line;
    std::getline(stream, line);
    std::istringstream linestream(line);
    std::string label;
    long jif_user, jif_nice, jif_sys, jif_idle;
    linestream >> label >> jif_user >> jif_nice >> jif_sys >> jif_idle;
    if (label == "cpu") return jif_idle;
  }
  return -1.0; //cpu label not found
}

//  Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  std::ifstream stream(kProcDirectory + kCpuinfoFilename);
  vector<std::string> cpu_utes;
  if (stream.is_open()) {
    std::string line;
    std::getline(stream, line);
    std::istringstream linestream(line);
    std::string label;
    linestream >> label;
    if (label != "cpu") return cpu_utes;  // bail now: first line should be cpu
    std::string item;
    for (int i = 0; i < 10; i++) {
      linestream >> item;
      cpu_utes.push_back(item);
    }
    return cpu_utes;
  }
  return cpu_utes;
}

//  Read and return the total number of processes
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
  }
  return -1;  // error
}

//  Read and return the number of running processes
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
    return std::stoi(data);
  }
  return -1;  // error
}

// ADDED: Refactor parsing of the "[pid]/status" file
std::string LinuxParser::pidStatusParser(const int pid, std::string _key) {
  cout << "\npSP: seeking " << _key << " for pid: " << pid;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    std::string line, key, value;
    std::getline(stream, line);
    do {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> key >> value;
    } while (key != _key + ":");
    if (key == _key + ":") return value;
  };
  return "key " + _key + " not found!";
}

//  Read and return the command associated with a process
string LinuxParser::Command(int pid) { return pidStatusParser(pid, "Name"); }

//  Read and return the memory used by a process
string LinuxParser::Ram(int pid) { return pidStatusParser(pid, "VmSize"); }

//  Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { return pidStatusParser(pid, "Uid"); }

//  Read and return the user associated with a process
string LinuxParser::User(int pid) {
  std::string test_uid = Uid(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    std::string line, name, pwd, uid;
    do {
      std::getline(stream, line);
      std::replace(line.begin(), line.end(), ' ', ':');
      std::istringstream linestream(line);
      linestream >> name >> pwd >> uid;
    } while (uid != test_uid);
    if (uid == test_uid) {
      return name;
    };
  };
  return "User not found!";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) {
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
