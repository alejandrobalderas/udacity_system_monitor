#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

#include <iostream>

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

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

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

float LinuxParser::MemoryUtilization() {
  float total_memory{0}, available_memory{0};
  string line, key, value;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()){
    while(std::getline(stream, line) && (!total_memory || !available_memory)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key=="MemTotal:"){
        total_memory=std::stof(value);
        }
      else if (key=="MemAvailable:"){
        available_memory=std::stod(value);
        }
      if(total_memory && available_memory){
        break;
      }
    }
  }
  return 1-(available_memory / total_memory);
}

long int LinuxParser::UpTime() {
  long int uptime_seconds;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime_seconds;
  }
  return uptime_seconds ;
}


long LinuxParser::Jiffies() {
  long jiffies{0};
  string line, key, current_jiffie;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "cpu"){
        while(linestream){
          linestream >> current_jiffie;
          jiffies += std::stoi(current_jiffie);
        }
      }
    }
  }

  return jiffies;
}


long LinuxParser::ActiveJiffies() {
  long jiffies{0};
  string line, key, current_jiffie;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "cpu"){
        for(int tmp = 0; tmp < 3; tmp++){
          linestream >> current_jiffie;
          jiffies += std::stoi(current_jiffie);

        }
      }
    }
  }

  return jiffies;
}

float LinuxParser::ProcessJiffies(int pid){
  string line, tmp_string;
  float utime{0}, stime{0}, cutime{0}, cstime{0};
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i = 0; i<13; i++){
      linestream >> tmp_string;
    }
    linestream >> utime >> stime>> cutime>> cstime;
  }

  return utime + stime + cutime + cstime;
}

float LinuxParser::CpuUtilization(int pid) {
  string line, tmp_string;
  float utime{0}, stime{0}, cutime{0}, cstime{0};
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i = 0; i<13; i++){
      linestream >> tmp_string;
    }
    linestream >> utime >> stime>> cutime>> cstime;
  }

  float uptime = (float)LinuxParser::UpTime();
  float total_time = utime + stime + cutime + cstime;
  float freq = sysconf(_SC_CLK_TCK);
  float start_time = LinuxParser::UpTime(pid);
  float seconds = uptime - start_time/freq; 

  float result = ((total_time / freq) / seconds);
  return result;
}

int LinuxParser::TotalProcesses() {
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if ( stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key=="processes"){
        break;
      }
    }
  }
  return std::stoi(value);
}


int LinuxParser::RunningProcesses() {
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if ( stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key=="procs_running"){
        break;
      }
    }
  }
  return std::stoi(value);
}

string LinuxParser::Command(int pid) {
  string pid_string {to_string(pid)};
  string line, command;
  std::ifstream stream(kProcDirectory + pid_string + kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> command;
    return command;
  }
  else{return "";}
}

string LinuxParser::Ram(int pid) {
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "VmData:"){
        break;
      }
    }
  }
  return value;
}

string LinuxParser::Uid(string pid) {
  string line, key, x, user;
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> x >> user;
      if(user == pid)
        return key;
    }
  }
  return pid;
}

string LinuxParser::User(int pid) {
   string user, line, key;
    std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatusFilename);
    if (stream.is_open()){
        while(std::getline(stream, line)){
            std::istringstream linestream(line);
            linestream >> key;
            if(key == "Uid:"){
                linestream >> user;
                return LinuxParser::Uid(user);
            }
        }
    }
    return "uidError";
}

long LinuxParser::UpTime(int pid) {
  string line, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i=0;i<22;i++){
      linestream >> value;
    }
  }
  return std::stol(value) / sysconf(_SC_CLK_TCK);
}