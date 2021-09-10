#include "Solver.h"

Solver::Solver(std::vector<double> data, int numThreads, double eps) {
    int N = data.size()/7;
    this->N = N;
    s.resize(N);
    double m;
    std::array<double, 3> r, v;
    for(int i=0; i<N; i++) {
        m    = data[i*7+0];
        r[0] = data[i*7+1];
        r[1] = data[i*7+2];
        r[2] = data[i*7+3];
        v[0] = data[i*7+4];
        v[1] = data[i*7+5];
        v[2] = data[i*7+6];
        s[i] = Star(m, r, v);
    }
    this->numThreads = numThreads;
    this->step = N / numThreads;
    this->eps  = eps;
    
    threadPool.resize(numThreads);
}

void Solver::calcAcc(int start, int stop, const double velfactor) {
    
    double dr2;
    std::array<double, 3> dr;
    dr.fill(0);
    for (int i=start; i<stop; i++) {
        s[i].a.fill(0);
    }

    for (int i=start; i<stop; i++) {
        for (int j=0; j<N; j++) {
            if (i == j) continue;
            dr2 = 0;
            for (int k=0; k<3; k++) {
                dr[k] = s[i].r[k] - s[j].r[k];
                dr2  += dr[k]*dr[k];
            }
            double rij = std::sqrt(dr2 + eps*eps);
            double apreij = 1 / (rij*rij*rij);
            
            for (int k=0; k<3; k++) {
                s[i].a[k] -= s[j].m * apreij *dr[k];
            }
        }
    }
    stepVelocity(start, stop, velfactor);
}

void Solver::multiAcc(const double velfactor) {
    for(int n=0; n<numThreads; n++) {
        threadPool[n] = std::thread(&Solver::calcAcc, this, n * step, (n + 1) * step, velfactor);
    }
}

void Solver::joinThreads() {
    for (std::thread &t : threadPool) {
        if (t.joinable()) {
            t.join();
        }
    }
}

void Solver::stepVelocity(int start, int stop, const double velfactor) {
    for (int i=start; i<stop; i++) {
        for (int k=0; k<3; k++) {
            s[i].v[k] += velfactor * eta * s[i].a[k];
        }
    }
}

void Solver::stepPosition(const double posfactor) {
    for (int i=0; i<N; i++) {
        for(int k=0; k<3; k++) {
            s[i].r[k] += posfactor * eta * s[i].v[k];
        }
    }
}

double Solver::stepCluster() {
    stepPosition(M_ETA);
    multiAcc(LAM2);
    joinThreads();
    
    stepPosition(M_XI);
    multiAcc(M_LAM);
    joinThreads();
    
    stepPosition(XI_ETA);
    multiAcc(M_LAM);
    joinThreads();
    
    stepPosition(M_XI);
    multiAcc(LAM2);
    joinThreads();
    
    stepPosition(M_ETA);
    return eta;
}

std::vector<double> Solver::getData() {
    std::vector<double> data;  
    for (std::vector<Star>::iterator si = s.begin(); si != s.end(); ++si) {
        data.push_back(si->m);
        data.push_back(si->r[0]);
        data.push_back(si->r[1]);
        data.push_back(si->r[2]);
        data.push_back(si->v[0]);
        data.push_back(si->v[1]);
        data.push_back(si->v[2]);	  
    }	  
    return data;
}
    
    
