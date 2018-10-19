//
//  Particle.hpp
//  flocking_mips
//
//  Created by <author> on 06/06/2018.
//
//

#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>
#include <vector>
#include "Cells.hpp"

using namespace std;

class Particle {
private:
public:
    //Particle state
    int particleIndex;
    float posX,posY;
    int sqX,sqY;
    float theta;
    float partR;
    float speed;

    //Universe
    float dT;
    float rotDiffusion;
    float rspDiffusion;
    int L, N, sC; //Passed from running instance of simulation

    //Square counting
    int squareX, squareY;
    bool topSq, leftSq, botSq, rightSq, diagTL, diagTR, diagBL, diagBR;

    //Methods
    Particle(float partR, float initPosX, float initPosY, float initTheta, int iL, int isC, int iN, int iIndex);
    int updateState();
    ~Particle();
protected:

};


#endif /* Particle_hpp */
