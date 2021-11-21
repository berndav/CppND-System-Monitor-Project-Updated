#include <string>
#include <cstdio>
#include <sstream>

#include "format.h"

using std::string;
using std::sprintf;

//Completed this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
std::string Format::ElapsedTime(long int seconds) {
    char ss_str[] = "00"; 
    char mm_str[] = "00"; 
    char hh_str[] = "00";
    if (seconds <= 0) return "00:00:00";
    int ss = seconds % 60;
    sprintf(ss_str,"%02d",(ss));
    int minutes = (seconds - ss) / 60;
    int mm = minutes % 60;
    sprintf(mm_str,"%02d",mm);
    int hh = (minutes / 60)  % 100;
    sprintf(hh_str, "%02d", hh);
    std::ostringstream strungTogether;
    strungTogether << hh_str << ":" << mm_str << ":" << ss_str;
    std::string result = strungTogether.str();

    return result; 
    }