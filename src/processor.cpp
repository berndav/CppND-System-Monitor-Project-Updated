#include "processor.h"
#include "linux_parser.h"

#include <iostream>

namespace Parser = LinuxParser;
Processor::Processor() {
    vendor_ = Parser::cpuinfoParser("vendor_id");
    name_ = Parser::cpuinfoParser("model name");
    speed_ = Parser::cpuinfoParser("cpu MHz");
    numCores_ = Parser::cpuinfoParser("cpu cores");
    //std::cout << "Processor created\n";
}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
return 0.480;
   /*  std::vector<std::string> procStats = Parser::CpuUtilization();
    int idleJiffies = 0;
    int totalJiffies = 0;
    const int idleColumn = 4;
    if (procStats[0] != "cpu")
        return 0.0;
    else{
        for (int i = 1; i < 10; i++){
            totalJiffies += std::stoi(procStats[i]);
        }
        idleJiffies = std::stoi(procStats[idleColumn]);
        float result = 100 * (1 - (float(idleJiffies)/float(totalJiffies)));
        std::cout << "CPU ute = " << result << "% \n";
        return result;
    }
    std::cout << "CPU ute FAIL! \n";
    return 0.0; //error */
    }


    std::string Processor::Vendor(){
        return vendor_;
    }

    std::string Processor::Name(){
        return name_;
    }

    std::string Processor::Speed(){
        return speed_;
    }

    std::string Processor::NumberOfCores(){
        return numCores_;
    }