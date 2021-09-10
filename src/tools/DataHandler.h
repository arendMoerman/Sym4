#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>
#include <random>
#include <array>
#include "Diagnostics.h"
#define M_PI           3.14159265358979323846

#ifndef __DataHandler_h
#define __DataHandler_h

class DataHandler {
public:
    int N;
    std::string config;
    std::vector<std::string> par;
    
    std::vector<double> data;
    
    DataHandler(int &N, std::string &config, std::vector<std::string> &par);
	
    void myOut(double &t, std::vector<double> &data, std::ofstream &out);
    void myEn(double &t, std::vector<double> &energies, std::ofstream &out);
    void myEn(double &t, double &energies, std::ofstream &out);
    void readInput(std::string &inputFile);
    
    void getColdPlummer();
    void getPlummer();
    
    void moveToCenter(std::vector<double> &data);  
    void Nbodyfier(std::vector<double> &data);
    
    double randomUniform(double start, double stop, std::mt19937 generator);
};
#endif
