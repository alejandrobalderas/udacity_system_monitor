#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
    Process(int pid);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

    void updateRam();
    void updateCpuUtilization();
  int ram{0};
  float cpu_utilization{0}, t1_cpu_utilization{0}, t2_cpu_utilization{0};
  // TODO: Declare any necessary private members
 private:
    int m_pid{0};

    long t1_total_jiffies{0}, t2_total_jiffies{0}, t1_process_jiffies{0}, t2_process_jiffies{0};
};

#endif