#!/bin/bash
echo "Bash version ${BASH_VERSION}..."
for i in {1..100}
do
	for J in {0.0,0.01,0.03,0.06,0.1,0.3,0.6,1.0,3.0,6.0}
	do
		printf "Running for J: $J, simulation $i/100\n"
       		nice -19 ./mainSim 102 3 400 0.1 0.3 0.5 1 $J 500000
       		mv snaps.out sim_$J.$i.out
	done
done
