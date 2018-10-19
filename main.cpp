#include <iostream>
#include "Simulation.hpp"
#include "globalVariables.hpp"
#include "Particle.hpp"
#include "Cells.hpp"

using namespace std;

int main(int argc, char **argv) {

    if (argc != 10) {
        cout << helpStr;
        return 0;
    }

    Simulation simFramework;
    try {
                                //100,10,400,0.1,0.3,0.5,1,0.6
        simFramework = Simulation(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atof(argv[4]),atof(argv[5]),atof(argv[6]),atof(argv[7]),atof(argv[8]));
    }
    catch (int err) {
        cout << "Error code: " << err << endl;
        cout << err_strings[err-1] << endl;
        return 0;
    }

    int err = simFramework.initState(RANDOMINIT);
    if (err != 0) {
        cout << "Error code " << err << endl;
        cout << err_strings[err-1] << endl;
        return 0;
    }
    for (int i = 0; i < atoi(argv[9]); i++) {
        simFramework.updateCycle(LISTUPDATE);
        if (i%1000==0) {
            float progress = (float)(i/1000)/(atof(argv[9])/1000.0);
            int barWidth = 70;
            cout << "[";
            int pos = barWidth * progress;
            for (int i = 0; i < barWidth; ++i) {
                if (i < pos) std::cout << "=";
                else if (i == pos) std::cout << ">";
                else std::cout << " ";
            }
            std::cout << "] " << int(progress * 100.0) << " %\r";
            std::cout.flush();
            //cout << i/1000 << endl;
            /*int a = 0;
            for (int i = 0; i < simFramework.systemSize/simFramework.cellSize; i++) {
                for (int j = 0; j < simFramework.systemSize/simFramework.cellSize; j++) {
                    a+=simFramework.cellLattice[j*simFramework.systemSize/simFramework.cellSize+i].particleIndices.size();
                }
            }
            cout << a << endl;*/
        }
        if (i%100==0)
            simFramework.printSnapshot();
        //simFramework.printSnapLatt();
    }
    cout << endl;
    return 0;
}
