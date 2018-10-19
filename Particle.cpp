//
//  Particle.cpp
//  flocking_mips
//
//  Created by <author> on 06/06/2018.
//
//

#include "Particle.hpp"
#include <math.h>
#include <vector>
#include "Cells.hpp"

using namespace std;

Particle::Particle(float initPartR, float initPosX, float initPosY, float initTheta, int iL, int isC, int iN, int uniIndex)  {
    posX = initPosX;
    posY = initPosY;
    theta = initTheta;
    partR = initPartR;
    particleIndex = uniIndex;

    L = iL;
    sC = isC;
    N = iN;

    //Calculate initial squares
    float fCellSize = (float)sC;
    topSq, leftSq, botSq, rightSq, diagTL, diagTR, diagBL, diagBR = false;
    int iX = (int)floor(initPosX/fCellSize);
    int iY = (int)floor(initPosY/fCellSize);
    sqX = iX;
    sqY = iY;

    //Overlapping flags
    float sqCenterX = initPosX+(float)(sC)/2.0f;
    float sqCenterY = initPosY+(float)(sC)/2.0f;
    float relativeX = initPosX-sqCenterX;
    float relativeY = initPosY-sqCenterY;
    if (relativeX < - fCellSize/2.0f + partR) {
        leftSq = true;
    }
    else if (relativeX > fCellSize/2.0f - partR) {
        rightSq = true;
    }
    if (relativeY < - fCellSize/2.0f + partR) {
        topSq = true;
        if (leftSq) {
            diagTL = true;
        }
        if (rightSq) {
            diagTR = true;
        }
    }
    else if (relativeY > fCellSize/2.0f - partR) {
        botSq = true;
        if (leftSq) {
            diagBL = true;
        }
        if (rightSq) {
            diagBR = true;
        }
    }
}
Particle::~Particle() {}

int Particle::updateState() {

}
