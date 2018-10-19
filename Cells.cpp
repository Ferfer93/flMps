//
//  Cells.cpp
//  flocking_mips
//
//  Created by <author> on 06/06/2018.
//
//

#include "Cells.hpp"

Cells::Cells(int indexInp, int iXinput, int iYinput, float dTinput, int Linput, int sCinput, int Ninput)  {
    index = indexInp;
    iX = iXinput;
    iY = iYinput;
    dT = dTinput;
    L  = Linput;
    sC = sCinput;
    N  = Ninput;
}
Cells::~Cells() {}
