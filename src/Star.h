#include <iostream>
#include <array>
#include <vector>
#include <cmath>
#include <numeric> 
#include <cstdlib>

#ifndef __Star_h
#define __Star_h

class Star {
public:
    double m;
    std::array<double, 3> r, v, a;
    
    Star();
    Star(double m, std::array<double, 3> r, std::array<double, 3> v);
  
    friend bool operator== (const Star &si, const Star &sj) {
        if(si.r[0] == sj.r[0] && si.r[1] == sj.r[1] && si.r[2] == sj.r[2])
            return true;
        else
            return false;
    }
};

#endif


