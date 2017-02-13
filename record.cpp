#include "record.h"
#include <iostream>

record::record(int size)
{
	N=size;
}

record::~record()
{
	for (int i=0; i<N; i++)
	{
		delete [] V[i];
		delete [] X[i];
	}
	delete [] V;
	delete [] X;
}


void record::init()
{
	V=new double*[N];
	X=new double*[N];
	for (int i=0; i<N; i++)
	{
		V[i]=new double[3];
		X[i]=new double[3];
	}
}

void record::set(double** V_in, double** X_in)
{
	V=V_in;
	X=X_in;
}

void record::print()
{
	cout<<"V array:"<<endl;
	for (int i=0; i<N; i++)
	{
		cout<<"V["<<i<<"] x, y, z: "<<V[i][0]<<", "<<V[i][1]<<", "<<V[i][2]<<endl;
	}
	
	cout<<"X array:"<<endl;
	for (int i=0; i<N; i++)
	{
		cout<<"X["<<i<<"] x, y, z: "<<X[i][0]<<", "<<X[i][1]<<", "<<X[i][2]<<endl;
	}
}
