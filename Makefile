CXX=g++
RM=rm -f

EXTL=m

SRCS=main.cpp Simulation.cpp Cells.cpp Particle.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: mainSim

mainSim: $(SRCS)
	@mkdir -p bin
	$(CXX) -std=c++14 $(SRCS) -O2 -Wall -o bin/mainSim -l$(EXTL)

clean:
	rm -f bin/*
	rmdir bin
