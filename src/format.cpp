#include <string>

#include "format.h"

//Completed this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
std::string Format::ElapsedTime(long int seconds) { 
    int ss = seconds % 60;
    int minutes = (seconds - ss) / 60;
    int mm = minutes % 60;
    int hh = minutes / 60;
    std::string result = std::to_string(hh) + ":" + std::to_string(mm) + ":" + std::to_string(ss);

    return result; 
    }