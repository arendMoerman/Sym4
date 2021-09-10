#include "DataHandler.h"

DataHandler::DataHandler(int &N, std::string &config, std::vector<std::string> &par) {
    this->N 		= N;
    this->config 	= config;
    this->par		= par;
    
    if(config == "file") {
         readInput(par[0]);
    }
    if(config == "cold_plummer") {
         getColdPlummer();
    }
    if(config == "plummer") {
         getPlummer();
    }
}

void DataHandler::myOut(double &t, std::vector<double> &data, std::ofstream &out) {
    //out << t << std::endl;
    for(int i=0; i<N; i++) {
        for(int j=0; j<7; j++) {
            out << data[i*7+j] << " ";
        }
        out << std::endl;
    }
}

void DataHandler::myEn(double &t, std::vector<double> &energies, std::ofstream &out) {
    out << t << " ";
    for(int i=0; i<N+1; i++) {
        out << energies[i] << " ";
    }
    out << std::endl;
}

void DataHandler::myEn(double &t, double &energy, std::ofstream &out) {
    out << t << " " << energy << " " << std::endl;
}

void DataHandler::readInput(std::string &inputFile) {
    std::ifstream inFile;
    std::string value;
    inFile.open("inputs/" + inputFile);
    std::vector<double> data;
    
    if (inFile) {
        while (inFile >> value) {
            double val = atof(value.c_str());
            data.push_back(val);
        }
    }
    this->data = data;
}

void DataHandler::getColdPlummer() {
     std::random_device rd;
     std::mt19937 generator(rd());
     std::uniform_real_distribution<> mm(0, 1);
     std::uniform_real_distribution<> pphi(0, 2 * M_PI);
     std::uniform_real_distribution<> ttheta(-1, 1);
     
     std::vector<double> data;
     for (int i=0; i<N; i++) {
          double m	     = mm(generator);
          double phi 	 = pphi(generator);
          
          double theta = acos(ttheta(generator));
          double r	= 1 / (sqrt(pow(m, -2.0/3.0) - 1.0));
          
          double x 	= r * sin(theta) * cos(phi);
          double y 	= r * sin(theta) * sin(phi);
          double z 	= r * cos(theta);

          data.push_back(m);
          data.push_back(x);
          data.push_back(y);
          data.push_back(z);
          data.push_back(0.0);
          data.push_back(0.0);
          data.push_back(0.0);
     }
     moveToCenter(data);
     Nbodyfier(data);
     this->data = data;
}

void DataHandler::getPlummer() {
     std::random_device rd;
     std::mt19937 generator(rd());
     std::uniform_real_distribution<> mm(0, 1);
     std::uniform_real_distribution<> pphi(0, 2 * M_PI);
     std::uniform_real_distribution<> ttheta(-1, 1);
     
     std::uniform_real_distribution<> vv(0, 1);
     std::uniform_real_distribution<> vphi(0, 2 * M_PI);
     std::uniform_real_distribution<> vtheta(-1, 1);
     
     
     std::vector<double> data;
     for (int i=0; i<N; i++) {
          // Positions
          double m	     = mm(generator);
          double phi 	 = pphi(generator);
          double v       = vv(generator);
          
          double theta = acos(ttheta(generator));
          double r	= 1 / (sqrt(pow(m, -2.0/3.0) - 1.0));
          
          double x 	= r * sin(theta) * cos(phi);
          double y 	= r * sin(theta) * sin(phi);
          double z 	= r * cos(theta);
          
          // Velocities
          double vl = 0.0;
          double vu = 0.1;
          while (vu > vl*vl*pow((1.0-vl*vl), 3.5)) {
              std::uniform_real_distribution<> vvl(0, 1);
              std::uniform_real_distribution<> vvu(0, 1);
              vl = vvl(generator);
              vu = vvu(generator) / 10;
          }
          double ve = vl * sqrt(2.0) * pow((1.0 + r * r), -0.25);
          theta = acos(vtheta(generator));
          phi   = vphi(generator);
          
          double vx = ve * sin(theta) * cos(phi);
          double vy = ve * sin(theta) * sin(phi);
          double vz = ve * cos(theta);

          data.push_back(m);
          data.push_back(x);
          data.push_back(y);
          data.push_back(z);
          data.push_back(vx);
          data.push_back(vy);
          data.push_back(vz);
     }
     moveToCenter(data);
     Nbodyfier(data);
     this->data = data;
     
     Diagnostics diag;
     std::cout << diag.getVelDispersion(data) << std::endl;
}

void DataHandler::moveToCenter(std::vector<double> &data) {
     std::array<double, 3> com;
     std::array<double, 3> cov;
     Diagnostics diag;
     
     double M = diag.getMass(data);
     
     com.fill(0);
     cov.fill(0);
     for(int i=0; i<N; i++) {
          for(int k=0; k<3; k++) {
          	com[k] += data[i*7] * data[i*7+k+1] / M;
          	cov[k] += data[i*7] * data[i*7+k+4] / M;
          }
     }
     
     for(int i=0; i<N; i++) {
          for(int k=0; k<3; k++) {
          	data[i*7+k+1] -= com[k];
          	data[i*7+k+4] -= cov[k];
          }
     }
}

void DataHandler::Nbodyfier(std::vector<double> &data) {
     Diagnostics diag;
     
     double M = diag.getMass(data);
     double Pot = diag.getTotalPot(data);
     
     double rVirial = -1*M*M / (4*Pot);
     double sigma   = diag.getVelDispersion(data);
     
     double Cm      = 1.0 / M;
     double Cr      = 1.0 / rVirial;
     double Cv      = 1.0;
     
     if(sigma > 0.0) {
        double Cv   = 1.0 / sqrt(2.0) / sigma;
     }
     
     for(int i=0; i<N; i++) {
          for(int k=0; k<3; k++) {
               data[i*7+k+1] *= Cr;
               data[i*7+k+4] *= Cv;
          }
          data[i*7] *= Cm;
     }
}

double DataHandler::randomUniform(double start, double stop, std::mt19937 generator) {
    std::uniform_real_distribution<> rando(start, stop);
    double rand = rando(generator);
    return rand;
}



