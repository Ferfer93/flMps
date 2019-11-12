//
//  Simulation.cpp
//  flocking_mips
//
//  Created by <author> on 06/06/2018.
//
//

#include "Simulation.hpp"
#include "globalVariables.hpp"
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <random>
#include <time.h>
#include "Cells.hpp"

using namespace std;

Simulation::Simulation() {

}

Simulation::Simulation(int sS, int cS, int pN, float dTi, float rD, float sD, float partRinput, float Jinp) :
    RndGen(random_device()()),
    realGDistr(0.0, 1.0)   {
    if (cS > sS/2)
        throw ERR_WRONGCELLSIZE;
    if (sS%cS != 0)
        throw ERR_WRONGCELLDIV;
    if (partRinput > (float)(cS)/3.0f)
        throw ERR_BIGPARTICLES;
    systemSize = sS;
    cellSize = cS;
    partNumber = pN;
    dT = dTi;
    rotDiffusion = rD;
    rspDiffusion = sD;
    partR = partRinput;
    v0 = 3.0f;
    J = Jinp;

    //Initialize Cell system
    int cellsPerLine = systemSize/cellSize;
    int totalCellNumber = cellsPerLine*cellsPerLine;
    int auxIndex = 0;
    for (int i = 0; i < cellsPerLine; i++) {
        for (int j = 0; j < cellsPerLine; j++) {
            Cells auxCell = Cells(auxIndex, i, j, dT, systemSize, cellSize, partNumber);
            cellLattice.push_back(auxCell);
            auxIndex++;
        }
    }
    cout << "Simulation object instance created" << endl;
}
Simulation::~Simulation() {}

int Simulation::initState(int method) {
    if (method == RANDOMINIT) {
        random_device rd;
        mt19937 rndGen(rd());
        uniform_real_distribution<float> realUDistr(0.0, 1.0);
        //fill until totalnumber == partNumber
        int auxIndex = 0;
        while (auxIndex < partNumber) {
            float ppX = realUDistr(rndGen)*((float)systemSize);
            float ppY = realUDistr(rndGen)*((float)systemSize);
            float thetaAux = realUDistr(rndGen)*2*PI;
            if (checkNakedCollision(ppX,ppY,-1))
                continue;
            Particle auxPart = Particle(partR, ppX, ppY, thetaAux, systemSize, cellSize, partNumber, auxIndex);
            particleSet.push_back(auxPart);
            addParticleToLattice(ppX,ppY,auxIndex);
            auxIndex++;
        }
    }
    else if (method == LATTICEINIT) {
        if ((float)(systemSize*systemSize)/((float)partNumber) < 4.0f*partR*partR) {
            return ERR_TOOMANYPARTICLES;
        }
        //Assume there's space and fill it.
        float partLine = ceil(sqrt((float)partNumber));
        float separation = (float)(systemSize) / partLine;
        float auxPosX = separation; float auxPosY = separation;
        int auxIndex = 0;
        for (int i = 0; i < partNumber; i++) {
            Particle aux = Particle(partR, auxPosX, auxPosY, 0, systemSize, cellSize, partNumber, auxIndex);
            auxIndex++;
            auxPosX += separation;
            if (auxPosX > systemSize) {
                auxPosX = separation;
                auxPosY += separation;
            }
            particleSet.push_back(aux);
        }
        cout << "Initial state created" << endl;
    }
    return 0;
}

int Simulation::checkNakedCollision(float pX, float pY, int exclInd) {
    float fCellSize = (float)cellSize;
    int iX = (int)floor(pX/fCellSize);
    int iY = (int)floor(pY/fCellSize);
    /*if (iX < 0 || iX > 29)
        cout << "Waning X delete chkNK() " << "(" << iX << "," << iY << ")" << " " << pX << " " << pY << endl;
    if (iY < 0 || iY > 29)
        cout << "Waning Y delete chkNK() " << "(" << iX << "," << iY << ")" << " " << pX << " " << pY << endl;*/
    int modL = systemSize/cellSize;
    int sqInd = iY+iX*systemSize/cellSize;
    if (checkSingleSqColl(pX, pY, sqInd, exclInd))
        return 1;

    float sqCenterX = pX+(float)(cellSize)/2.0f;
    float sqCenterY = pY+(float)(cellSize)/2.0f;
    float relativeX = pX-sqCenterX;
    float relativeY = pY-sqCenterY;
    sqInd = iY+((iX-1+modL)%modL)*systemSize/cellSize;
    if (checkSingleSqColl(pX, pY, sqInd, exclInd))
        return 1;
    sqInd = iY+((iX+1)%modL)*systemSize/cellSize;
    if (checkSingleSqColl(pX, pY, sqInd, exclInd))
        return 1;
    sqInd = (iY-1+modL)%modL+(iX)*systemSize/cellSize;
    if (checkSingleSqColl(pX, pY, sqInd, exclInd))
        return 1;
    sqInd = (iY-1+modL)%modL+((iX-1+modL)%modL)*systemSize/cellSize;
    if (checkSingleSqColl(pX, pY, sqInd, exclInd))
        return 1;
    sqInd = (iY-1+modL)%modL+((iX+1)%modL)*systemSize/cellSize;
    if (checkSingleSqColl(pX, pY, sqInd, exclInd))
        return 1;
    sqInd = (iY+1)%modL+(iX)*systemSize/cellSize;
    if (checkSingleSqColl(pX, pY, sqInd, exclInd))
        return 1;
    sqInd = (iY+1)%modL+((iX-1+modL)%modL)*systemSize/cellSize;
    if (checkSingleSqColl(pX, pY, sqInd, exclInd))
        return 1;
    sqInd = (iY+1)%modL+((iX+1)%modL)*systemSize/cellSize;
    if (checkSingleSqColl(pX, pY, sqInd, exclInd))
        return 1;
    return 0;
}

int Simulation::checkSingleSqColl(float pX, float pY, int sqInd, int exclInd) {
    for (int i = 0; i < cellLattice[sqInd].particleIndices.size(); i++) {
        int prtchkInd = cellLattice[sqInd].particleIndices[i];
        if (prtchkInd == exclInd)
            continue;
        float chkX = particleSet[prtchkInd].posX;
        float chkY = particleSet[prtchkInd].posY;
        if ((pX-chkX)*(pX-chkX)+(pY-chkY)*(pY-chkY) < 4.0f*partR*partR) {
            //cout << "Coll" << endl;
            return 1;
        }
    }
    return 0;
}

int Simulation::addParticleToLattice(float pX, float pY, int prtIndex) {
    float fCellSize = (float)cellSize;
    int iX = (int)floor(pX/fCellSize);
    int iY = (int)floor(pY/fCellSize);
    /*if (iX < 0 || iX > 29)
        cout << "Waning X delete addprt() " << "(" << iX << "," << iY << ")" << " " << pX << " " << pY << endl;
    if (iY < 0 || iY > 29)
        cout << "Waning Y delete addprt() " << "(" << iX << "," << iY << ")" << " " << pX << " " << pY << endl;*/
    int modL = systemSize/cellSize;
    int sqInd = iY+iX*systemSize/cellSize;
    cellLattice[sqInd].particleIndices.push_back(prtIndex);

    sqInd = iY+((iX-1+modL)%modL)*systemSize/cellSize;
    cellLattice[sqInd].particleIndices.push_back(prtIndex);
    sqInd = iY+((iX+1)%modL)*systemSize/cellSize;
    cellLattice[sqInd].particleIndices.push_back(prtIndex);
    sqInd = (iY-1+modL)%modL+(iX)*systemSize/cellSize;
    cellLattice[sqInd].particleIndices.push_back(prtIndex);
    sqInd = (iY-1+modL)%modL+((iX-1+modL)%modL)*systemSize/cellSize;
    cellLattice[sqInd].particleIndices.push_back(prtIndex);
    sqInd = (iY-1+modL)%modL+((iX+1)%modL)*systemSize/cellSize;
    cellLattice[sqInd].particleIndices.push_back(prtIndex);
    sqInd = (iY+1)%modL+(iX)*systemSize/cellSize;
    cellLattice[sqInd].particleIndices.push_back(prtIndex);
    sqInd = (iY+1)%modL+((iX-1+modL)%modL)*systemSize/cellSize;
    cellLattice[sqInd].particleIndices.push_back(prtIndex);
    sqInd = (iY+1)%modL+((iX+1)%modL)*systemSize/cellSize;
    cellLattice[sqInd].particleIndices.push_back(prtIndex);
    return 0;
}

int Simulation::removeParticleFromLattice(float pX, float pY, int prtIndex) {
    float fCellSize = (float)cellSize;
    int iX = (int)floor(pX/fCellSize);
    int iY = (int)floor(pY/fCellSize);
    /*if (iX < 0 || iX > 29)
        cout << "Waning X delete remprt() " << "(" << iX << "," << iY << ")" << " " << pX << " " << pY << endl;
    if (iY < 0 || iY > 29)
        cout << "Waning Y delete remprt() " << "(" << iX << "," << iY << ")" << " " << pX << " " << pY << endl;*/
    int modL = systemSize/cellSize;
    int sqInd = iY+iX*systemSize/cellSize;
    for (int i = 0; i < cellLattice[sqInd].particleIndices.size(); i++) {
        if (cellLattice[sqInd].particleIndices[i] == prtIndex)
            cellLattice[sqInd].particleIndices.erase(cellLattice[sqInd].particleIndices.begin()+i);
    }

    sqInd = iY+((iX-1+modL)%modL)*systemSize/cellSize;
    for (int i = 0; i < cellLattice[sqInd].particleIndices.size(); i++) {
        if (cellLattice[sqInd].particleIndices[i] == prtIndex)
            cellLattice[sqInd].particleIndices.erase(cellLattice[sqInd].particleIndices.begin()+i);
    }
    sqInd = iY+((iX+1)%modL)*systemSize/cellSize;
    for (int i = 0; i < cellLattice[sqInd].particleIndices.size(); i++) {
        if (cellLattice[sqInd].particleIndices[i] == prtIndex)
            cellLattice[sqInd].particleIndices.erase(cellLattice[sqInd].particleIndices.begin()+i);
    }
    sqInd = (iY-1+modL)%modL+(iX)*systemSize/cellSize;
    for (int i = 0; i < cellLattice[sqInd].particleIndices.size(); i++) {
        if (cellLattice[sqInd].particleIndices[i] == prtIndex)
            cellLattice[sqInd].particleIndices.erase(cellLattice[sqInd].particleIndices.begin()+i);
    }
    sqInd = (iY-1+modL)%modL+((iX-1+modL)%modL)*systemSize/cellSize;
    for (int i = 0; i < cellLattice[sqInd].particleIndices.size(); i++) {
        if (cellLattice[sqInd].particleIndices[i] == prtIndex)
            cellLattice[sqInd].particleIndices.erase(cellLattice[sqInd].particleIndices.begin()+i);
    }
    sqInd = (iY-1+modL)%modL+((iX+1)%modL)*systemSize/cellSize;
    for (int i = 0; i < cellLattice[sqInd].particleIndices.size(); i++) {
        if (cellLattice[sqInd].particleIndices[i] == prtIndex)
            cellLattice[sqInd].particleIndices.erase(cellLattice[sqInd].particleIndices.begin()+i);
    }
    sqInd = (iY+1)%modL+(iX)*systemSize/cellSize;
    for (int i = 0; i < cellLattice[sqInd].particleIndices.size(); i++) {
        if (cellLattice[sqInd].particleIndices[i] == prtIndex)
            cellLattice[sqInd].particleIndices.erase(cellLattice[sqInd].particleIndices.begin()+i);
    }
    sqInd = (iY+1)%modL+((iX-1+modL)%modL)*systemSize/cellSize;
    for (int i = 0; i < cellLattice[sqInd].particleIndices.size(); i++) {
        if (cellLattice[sqInd].particleIndices[i] == prtIndex)
            cellLattice[sqInd].particleIndices.erase(cellLattice[sqInd].particleIndices.begin()+i);
    }
    sqInd = (iY+1)%modL+((iX+1)%modL)*systemSize/cellSize;
    for (int i = 0; i < cellLattice[sqInd].particleIndices.size(); i++) {
        if (cellLattice[sqInd].particleIndices[i] == prtIndex)
            cellLattice[sqInd].particleIndices.erase(cellLattice[sqInd].particleIndices.begin()+i);
    }
    return 0;
}

//Writing methods
int Simulation::printSnapshot(int skip) {
    //File writing
    ofstream snapFile;
    snapFile.open("snaps.out", ios::app);
    for (int i = 0; i < partNumber; i++) {
        if (i%skip == 0)
            snapFile << particleSet[i].particleIndex << " " << particleSet[i].posX << " " << particleSet[i].posY << " " << particleSet[i].theta << " " << (int)neighFlags(particleSet[i].posX,particleSet[i].posY) << endl;
    }
    snapFile.close();
}

int Simulation::printSnapLatt() {
    ofstream snapFile;
    snapFile.open("snapsL.out", ios::app);
    int latticeLength = systemSize/cellSize;
    float maxNumber = (float)cellSize/(2.0f*partR);
    for (int i = 0; i < latticeLength; i++) {
        for (int j = 0; j < latticeLength; j++) {
            int sqInd = j + latticeLength*i;
            snapFile << (float)cellLattice[sqInd].particleIndices.size()/maxNumber;
            if (j < latticeLength-1)
                snapFile << " ";
        }
        snapFile << endl;
    }
    snapFile.close();
}

//Update methods
int Simulation::updateCycle(int method) {
    if (method == LISTUPDATE) {
        for (int i = 0; i < partNumber; i++) {
            moveParticle(particleSet[i].particleIndex);
        }
    }
    else if (method == RANDOMUPDATE) {
        for (int i = 0; i < partNumber; i++) {
            /* Get random particle
             * Update position
             * - calculate new position
             * - check excluded collision
             * --- if negative
             * --- change position
             * --- update squares
             * - diffuse angle
             */
        }
    }
}

int Simulation::moveParticle(int pInd) {
    float pX = particleSet[pInd].posX;
    float pY = particleSet[pInd].posY;
    float th = particleSet[pInd].theta;
    float rX = realGDistr(RndGen);
    float rY = realGDistr(RndGen);
    float rt = realGDistr(RndGen);

    float nX = pX + v0*cos(th)*dT + SQ2*rspDiffusion*dT*rX;
    float nY = pY + v0*sin(th)*dT + SQ2*rspDiffusion*dT*rY;
    if (nX >= systemSize)
        nX -= (float)systemSize;
    if (nX < 0.0f)
        nX += (float)systemSize;
    if (nY >= systemSize)
        nY -= (float)systemSize;
    if (nY < 0.0f)
        nY += (float)systemSize;
    //Second check? wtf?
    if (nX == systemSize)
        nX = 0.0f;
    if (nY == systemSize)
        nY = 0.00f;
    if (nX >= systemSize || nX < 0.0f)
        cout << "Warning move X " << "(" << nX << "," << nY << ")" << " " << pX << " " << pY << endl;
    if (nY >= systemSize || nY < 0.0f)
        cout << "Warning move X " << "(" << nX << "," << nY << ")" << " " << pX << " " << pY << endl;
    if (checkNakedCollision(nX, nY, pInd)) {
        particleSet[pInd].theta += SQ2*rotDiffusion*dT*rt-2*PI*floor((th+SQ2*rotDiffusion*dT*rt))/(2*PI));
        return 1;
    }
    removeParticleFromLattice(pX,pY,pInd);
    addParticleToLattice(nX,nY,pInd);
    particleSet[pInd].posX = nX;
    particleSet[pInd].posY = nY;
    float rotH = 0.0f;
    if (J != 0) {
        float fCellSize = (float)cellSize;
        int iX = (int)floor(pX/fCellSize);
        int iY = (int)floor(pY/fCellSize);
        int modL = systemSize/cellSize;
        int sqInd = iY+iX*systemSize/cellSize;
        int nParts = cellLattice[sqInd].particleIndices.size();
        for (int i = 0; i < nParts; i++) {
            if (cellLattice[sqInd].particleIndices[i] != pInd) {
                rotH += sin(th - particleSet[cellLattice[sqInd].particleIndices[i]].theta);
            }
        }
    }
    th = particleSet[pInd].theta;
    particleSet[pInd].theta += -J*rotH*dT + SQ2*rotDiffusion*dT*rt-2*PI*floor((th-J*rotH*dT + SQ2*rotDiffusion*dT*rt))/(2*PI));

    return 0;
}

unsigned char Simulation::neighFlags(float pX, float pY) {
    unsigned char activeFlags = 0;

    float fCellSize = (float)cellSize;
    int iX = (int)floor(pX/fCellSize);
    int iY = (int)floor(pY/fCellSize);
    int modL = systemSize/cellSize;
    int sqInd = iY+iX*systemSize/cellSize;

    //Overlapping flags
    float sqCenterX = ((float)iX)*fCellSize+(float)(cellSize)/2.0f;
    float sqCenterY = ((float)iY)*fCellSize+(float)(cellSize)/2.0f;
    float relativeX = pX-sqCenterX;
    float relativeY = pY-sqCenterY;
    if (relativeX < - fCellSize/2.0f + partR) {
        activeFlags |= NL;
    }
    else if (relativeX > fCellSize/2.0f - partR) {
        activeFlags |= NR;
    }
    if (relativeY < - fCellSize/2.0f + partR) {
        activeFlags |= NT;
        if (activeFlags & NL) {
            activeFlags |= NTL;
        }
        if (activeFlags & NR) {
            activeFlags |= NTR;
        }
    }
    else if (relativeY > fCellSize/2.0f - partR) {
        activeFlags |= NB;
        if (activeFlags & NL) {
            activeFlags |= NBL;
        }
        if (activeFlags & NR) {
            activeFlags |= NBR;
        }
    }
    return activeFlags;
}
