#include "record_ext.h"
#include <iostream>

void record_ext::copy(record_ext& other)
{
	for (int coord=0; coord<3; coord++) for (int atom=0; atom<1000; atom++)
	{
		R[coord][atom]=other.R[coord][atom];
		Rnoperiod[coord][atom]=other.Rnoperiod[coord][atom];
		V[coord][atom]=other.V[coord][atom];
		Vnoperiod[coord][atom]=other.Vnoperiod[coord][atom];
	}
}


void record_ext::print()
{
	cout<<"V array[:][0]:"<<endl;
	for (int i=0; i<3; i++)
	{
		cout<<"V["<<i<<"] x, y, z: "<<V[i][0]<<", "<<V[i][1]<<", "<<V[i][2]<<endl;
	}
	
	cout<<"R array:"<<endl;
	for (int i=0; i<3; i++)
	{
		cout<<"R["<<i<<"] x, y, z: "<<R[i][0]<<", "<<R[i][1]<<", "<<R[i][2]<<endl;
	}
	
	cout<<"Vnoperiod array:"<<endl;
	for (int i=0; i<3; i++)
	{
		cout<<"Vnoperiod["<<i<<"] x, y, z: "<<Vnoperiod[i][0]<<", "<<Vnoperiod[i][1]<<", "<<Vnoperiod[i][2]<<endl;
	}
	
	cout<<"Rnoperiod array:"<<endl;
	for (int i=0; i<3; i++)
	{
		cout<<"Rnoperiod["<<i<<"] x, y, z: "<<Rnoperiod[i][0]<<", "<<Rnoperiod[i][1]<<", "<<Rnoperiod[i][2]<<endl;
	}
}
