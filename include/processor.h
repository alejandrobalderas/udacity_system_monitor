#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    long t1_jiffies{0}, t2_jiffies{0}, t1_active_jiffies{0}, t2_active_jiffies{0};
};


#endif