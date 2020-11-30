#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds, bool with_days) {
    // seconds = 3661;
    int d{0}, h{0}, m{0};
    string res = "";
    h = seconds / 3600;
    seconds %= 3600;
    if(with_days && h>23){
        d = h/24;
        h %= 24;
        res += std::to_string(d) + " days, ";
    }
    m = seconds /60;
    seconds %= 60;
    if (h < 10){res += "0" + std::to_string(h) + ":";}
    else {res += std::to_string(h) + ":";}
    if (m < 10){res += "0" + std::to_string(m) + ":";}
    else {res += std::to_string(m) + ":";}
    if (seconds < 10){res += "0" + std::to_string(seconds);}
    else {res += std::to_string(seconds);}
    return res;
}