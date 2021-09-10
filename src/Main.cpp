#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>

#include "Star.h"
#include "Solver.h"
#include "DataHandler.h"
#include "Diagnostics.h"
#include "Timer.h"

int main(int argc, char* argv[]) {
    double t_begin          		= atof(argv[1]);
    double t_end            		= atof(argv[2]);
    double dt_snap          		= atof(argv[3]);
    double eta              		= atof(argv[4]);
    int N                   		= atoi(argv[5]);
    int numThreads                  = atoi(argv[6]);
    std::string config			    = argv[7];
    std::vector<std::string> par;
    for(int i=8; i<argc; i++) {
         par.push_back(argv[i]);
    }
    
    double t = t_begin;
    double t_snap = t_begin;

    DataHandler handler(N, config, par);

    std::vector<double> data = handler.data;
    Diagnostics diag;
    double eps = diag.getOptimalEps(data);
    Solver grav(data, numThreads, eps);
    
    int prec = 16; // Machine precision
    grav.eta = eta;
    
    Timer timer;
    double t_cpu = 0;
    ///////////////// OUTPUT & ENERGIES ///////////////////
    
    std::ofstream outFile;
    outFile.open("data.out");
    outFile << std::setprecision(prec);
    
    std::ofstream enerFile;
    enerFile.open("energies.out");
    enerFile << std::setprecision(prec);
    //std::vector<double> energies = grav.getEnergies();
    
    double energy = diag.getTotalEnr(data);
    
    handler.myOut(t, data, outFile);
    handler.myEn(t, energy, enerFile);
    
    ///////////////////////////////////////////////////////
    
    /* Evolution loop */
    while(t < t_end) {
        timer.start();
        
        t += grav.stepCluster();
        
        timer.stop();
        t_cpu += timer.get();
        data = grav.getData();
        if(t > t_snap) {
            //energies = grav.getEnergies();
            energy = diag.getTotalEnr(data);
            
            handler.myOut(t, data, outFile);
            handler.myEn(t, energy, enerFile);
            
            std::cerr << t_snap << "/" << t_end << std::endl;
            t_snap += dt_snap;
        }

        if(t > t_end) t = t_end;
    }
    outFile.close();
    enerFile.close();
    std::cerr << "# Simulation finished!" << std::endl;
    std::cerr << "N             = " << N << std::endl;
    std::cerr << "numThreads    = " << numThreads << std::endl;
    std::cerr << "t_cpu         = " << t_cpu << std::endl;
    std::cerr << "eps           = " << eps << std::endl;
} 
