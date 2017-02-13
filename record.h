////////////////////////////////////////////////////////
//
// This class is for one time step of argon simulation
// N - number of atoms in simulated volume
// X - Nx3 matrix of coords (x,y,z) of atoms 
// V - Nx3 matrix of velocities (x,y,z) of atoms
//

#ifndef RECORD_H
#define RECORD_H
#include<ostream>
#include<fstream>

using namespace std;

class record
{
public:
	int N;
	double **V;
	double **X;
	
	record(int);
	~record();
	void init();
	void set(double **, double **);
	void print();
	int getSize(){return sizeof(double)*N*3*2+sizeof(N);};
};

#endif // RECORD_H
