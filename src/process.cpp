#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : m_pid(pid){
    this->updateRam();
    this->updateCpuUtilization();
}

void Process::updateRam(){
    this->ram = std::stoi(LinuxParser::Ram(this->m_pid))/float(1024);
}

void Process::updateCpuUtilization(){
    this->cpu_utilization = LinuxParser::CpuUtilization(this->m_pid);
}

int Process::Pid() {return this->m_pid;}


float Process::CpuUtilization() {
    this->updateCpuUtilization();
    return this->cpu_utilization;
}


string Process::Command() {
    return LinuxParser::Command(this->m_pid);
}

string Process::Ram() {
    this->updateRam();
    return std::to_string(this->ram);
}

string Process::User() {
    return LinuxParser::User(this->m_pid);
}

long int Process::UpTime() {
    return LinuxParser::UpTime(this->m_pid);
}

bool Process::operator<(Process const& a) const {
    return this->cpu_utilization > a.cpu_utilization;
}