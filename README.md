# ArgonSym
A simple simulation of 1k argon atoms in cube which edge length is about 36 Angstrems.

	*Temperature is close to boiling point.

	*Lennard-Jones potential was uzed with sphreical range limit

	*Periodic boundary conditions were applied. There is also array for distribution of atoms without periodic boundary condition needed to see mean space displacement evolution in time, but forces and velocities are calculated only for periodic boundary condition.

	*one step of simulation corresponds time equal 1 ps

- main.cpp - simulation and generation of snapshots every 20 steps. Generating `symulacja.data' file.

- rozklad.cpp - uses `symulacja.data' to generate plot of statistical radial dispersion (proportional to probability of finding another atom from a chosen atom at given distance)

- obliczenia2.cpp - generating two other statistics: VCF and MSD


TO COMPILE you need to have ROOT installed. Then add to g++:
--std=c++11 `root-config --cflags glibs`