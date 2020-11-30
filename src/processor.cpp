#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 

    // Get latest values for the computation
    this->t2_jiffies = LinuxParser::Jiffies();
    this->t2_active_jiffies = LinuxParser::ActiveJiffies();

    float utilization = (float)(this->t2_active_jiffies - this->t1_active_jiffies)/(float)(this->t2_jiffies- this->t1_jiffies);

    // New values (T2) now become the old values (T1)
    this->t1_active_jiffies = this->t2_active_jiffies;
    this->t1_jiffies = this->t2_jiffies; 

    return utilization; 
}