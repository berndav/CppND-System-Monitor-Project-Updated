#include <string>

#include "format.h"

using std::string;

//Completed this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int ss = seconds % 60;
    int mm = (seconds - ss) / 60;
    int hh = mm / 60;
    std::string result = std::to_string(hh) + ":" + std::to_string(hh) + ":" + std::to_string(hh);

    return result; 
    }