#include <vector>
#include <cmath>

#ifndef __Diagnostics_h
#define __Diagnostics_h

class Diagnostics {
public:
    double getMass(const std::vector<double> &data);
    double getTotalKin(const std::vector<double> &data);
    double getTotalPot(const std::vector<double> &data);
    double getTotalEnr(const std::vector<double> &data);
    
    std::vector<double> getKin(const std::vector<double> &data);
    std::vector<double> getPot(const std::vector<double> &data);
    
    double getVelDispersion(const std::vector<double> &data);
    double getOptimalEps(const std::vector<double> &data);
};

#endif
