#include <iostream>
#include <array>
#include <vector>
#include <cmath>
#include <numeric> 
#include <cstdlib>
#include <thread>
#include "Star.h"

#ifndef __Solver_h
#define __Solver_h

class Solver : public Star {
private:
    const double M_ETA  =  0.1786178958448091E0;
    const double M_LAM  = -0.2123418310626054E0;
    const double M_XI   = -0.6626458266981849E-1;
    
    const double LAM2   = (1 - 2*M_LAM) / 2;
    const double XI_ETA = 1 - 2*(M_XI + M_LAM);
public:
    
    std::vector<std::thread> threadPool;
    std::vector<Star> s;
    double eta;
    double eps;
    int N;
    int numThreads;
    int step;

    Solver(std::vector<double> data, int numThreads, double eps);

    void calcAcc(int start, int stop, const double velfactor);
    void multiAcc(const double velfactor);
    void joinThreads();
    
    void stepVelocity(int i, const double velfactor);
    void stepPosition(const double posfactor);
    
    double stepCluster();
    
    std::vector<double> getData();
};
#endif
