#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>

class Processor {
 public:
    Processor();
    float Utilization();

    // Accessors
    std::string Vendor();
    std::string Name();
    std::string Speed();
    std::string NumberOfCores();

  // TODO: Declare any necessary private members
 private:
 long int prevActive_;
 long int prevTotal_;

 std::string vendor_;
 std::string name_;
 std::string speed_;
 std::string numCores_;
};

#endif