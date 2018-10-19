//
//  Cells.hpp
//  flocking_mips
//
//  Created by <author> on 06/06/2018.
//
//

#ifndef Cells_hpp
#define Cells_hpp

#include <iostream>
#include <vector>

using namespace std;

class Cells {
private:
public:
    //Cell state
    int iX, iY;
    int index;
    //Some array of cell indices easy to modify, prob vector.h library of ints
    vector<int> particleIndices;

    //Universe
    float dT;
    int L, N, sC; //Passed from running instance of simulation

    Cells(int index, int iX, int iY, float dT, int L, int sC, int N);
    ~Cells();
protected:

};


#endif /* Cells_hpp */
