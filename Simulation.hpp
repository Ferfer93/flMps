//
//  Simulation.hpp
//  flocking_mips
//
//  Created by <author> on 06/06/2018.
//
//

#ifndef Simulation_hpp
#define Simulation_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include "Particle.hpp"
#include "Cells.hpp"
#include <random>
#include <time.h>

using namespace std;

class Simulation {
private:
    //RandGauss
    mt19937 RndGen;
    normal_distribution<float> realGDistr;
public:
    //Universe
    int systemSize;
    int cellSize;
    int partNumber;
    float partR;
    vector<Cells> cellLattice;

    float dT;
    float rotDiffusion;
    float rspDiffusion;
    float v0;
    float J;

    vector<Particle> particleSet;


    //Methods
    Simulation();
    Simulation(int sS, int cS, int pN, float dT, float rD, float sD, float partR, float J);
    ~Simulation();

    int initState(int method);
    int checkNakedCollision(float, float, int);
    int checkSingleSqColl(float, float, int, int);
    int addParticleToLattice(float, float, int);
    int removeParticleFromLattice(float, float, int);
    int printSnapshot();
    int printSnapLatt();

    int updateCycle(int method);
    int moveParticle(int index);

    unsigned char neighFlags(float X, float Y);
protected:

};


#endif /* Simulation_hpp */
