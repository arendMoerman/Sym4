#include "Diagnostics.h"

double Diagnostics::getMass(const std::vector<double> &data) {
    int N = data.size() / 7;
    
    double M = 0;
    for (int i=0; i<N; i++) {
        M += data[i*7];
    }
    return M;
}

double Diagnostics::getTotalKin(const std::vector<double> &data) {
    int N = data.size() / 7;
    
    double Kin = 0;
    for (int i=0; i<N; i++) {
        double v2 = 0;
        for (int k=0; k<3; k++) {
            v2 += data[i*7+4+k] * data[i*7+4+k];
        }
        Kin += 0.5 * data[i*7] * v2;
    }
    return Kin;
}

double Diagnostics::getTotalPot(const std::vector<double> &data) {
    int N = data.size() / 7;
    
    double Pot = 0;
    for (int i=0; i<N-1; i++) {
        for (int j=i+1; j<N; j++) {
            double rij = 0;
            for (int k=0; k<3; k++) {
                rij += (data[i*7+1+k] - data[j*7+1+k]) * 
                    (data[i*7+1+k] - data[j*7+1+k]);
            }
        rij = std::sqrt(rij);
        
        Pot -= data[i*7] * data[j*7] / rij;
        }
    }
    return Pot;
}

double Diagnostics::getTotalEnr(const std::vector<double> &data) {
    double Kin = getTotalKin(data);
    double Pot = getTotalPot(data);
    
    return Kin + Pot;
}
    
std::vector<double> Diagnostics::getKin(const std::vector<double> &data) {
    int N = data.size() / 7;
    
    std::vector<double> Kin;
    for (int i=0; i<N; i++) {
        double v2 = 0;
        for (int k=0; k<3; k++) {
            v2 += data[i*7+4+k] * data[i*7+4+k];
        }
        Kin.push_back(0.5 * data[i*7] * v2);
    }
    return Kin;
}

std::vector<double> Diagnostics::getPot(const std::vector<double> &data) {
    int N = data.size() / 7;
    
    std::vector<double> Pot(N, 0);
    for (int i=0; i<N-1; i++) {
        for (int j=i+1; j<N; j++) {
            double rij = 0;
            for (int k=0; k<3; k++) {
                rij += (data[i*7+1+k] - data[j*7+1+k]) * 
                    (data[i*7+1+k] - data[j*7+1+k]);
            }
        rij = std::sqrt(rij);
        
        double potE = -0.5 * data[i*7] * data[j*7] / rij;
        
        Pot[i] = potE;
        Pot[j] = potE;
        }
    }
    return Pot;
}

double Diagnostics::getVelDispersion(const std::vector<double> &data) {
    double M = getMass(data);
    double K = getTotalKin(data);
    
    double sigma = sqrt(2*K / M);
    return sigma;
}

double Diagnostics::getOptimalEps(const std::vector<double> &data) {
    int N = data.size() / 7;
    double eps = 1E99;
    
    for (int i=0; i<N-1; i++) {
        for (int j=i+1; j<N; j++) {
            double rij = 0;
            for (int k=0; k<3; k++) {
                rij += (data[i*7+1+k] - data[j*7+1+k]) * 
                    (data[i*7+1+k] - data[j*7+1+k]);
            }
            eps = std::min(eps, rij);
        }
    }
    eps /= 2;
    return eps;
}
        
    
    
