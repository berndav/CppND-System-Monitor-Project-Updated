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
    return float(Parser::ActiveJiffies()) / float(Parser::Jiffies());
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