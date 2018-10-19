CXX=g++
RM=rm -f

EXTL=m

SRCS=main.cpp Simulation.cpp Cells.cpp Particle.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: mainSim

mainSim: $(SRCS)
	$(CXX) -std=c++14 $(SRCS) -o mainSim -l$(EXTL)

clean:
	rm -f mainSim
