#include <vector>
#include <string>

using namespace std;

#ifndef GlobalVariables
#define GlobalVariables

#define PI 3.141593
#define SQ2 1.414

//Initialization
#define RANDOMINIT 0
#define LATTICEINIT 1

//Updates
#define LISTUPDATE 0
#define RANDOMUPDATE 1

//Neighbour flags
#define NT 0x1
#define NB 0x2
#define NL 0x4
#define NR 0x8
#define NTL 0x10
#define NTR 0x20
#define NBL 0x40
#define NBR 0x80

//Help text
static const string helpStr = "Usage\nArg 1: System size\nArg 2: Cell size\nArg 3: Number of particles\nArg 4: Timestep size\nArg 5: Rotational diffusion\nArg 6: Space diffusion\nArg 7: Particle radius\nArg 8: Alignment coupling\nArg 9: Number of timesteps\n";

//Errors
#define ERR_TOOMANYPARTICLES 1
#define ERR_WRONGCELLSIZE 2
#define ERR_BIGPARTICLES 3
#define ERR_WRONGCELLDIV 4

//Error strings
static const vector<string> err_strings = { "Density is too high",
                                            "Cell size bigger than half the system size",
                                            "Particles bigger than a third of the cell size",
                                            "Cell size must divide exactly the system size"};

#endif
