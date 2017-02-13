///////////////////////////////////////////////////
//
// Extended record class
// It can hold matrices for simulation with boundary
// conditions an without in one time step
// - eqiuvalent to running simulation two times
//   with boundary conditions on and off
//   from the same initial state.

#ifndef RECORD_EXT_H
#define RECORD_EXT_H
#include "record.h"

class record_ext
{
public:
	double Rnoperiod[3][1000];
	double Vnoperiod[3][1000];
	double R[3][1000];
	double V[3][1000];

	void print();
	void copy(record_ext &other);
};

#endif // RECORD_EXT_H
