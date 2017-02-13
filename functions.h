#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "data.h"

using namespace std;

namespace sym
{
	double displacement(double ampl)
	{
		static long int seed = time(NULL);
		srand(seed);
		seed=rand();
		return (((seed % 100001) - 50000.) / 50000.) * ampl;
	}

	double sum_array(double * arr, int arr_size)
	{
		double sum=0;
		for (int i=0; i<arr_size; i++) sum+=*(arr+i);
		return sum;
	}

	double sum_pow2_array(double * arr, int arr_size)
	{
		double sum=0;
		for (int i=0; i<arr_size; i++) sum+=arr[i]*arr[i];
		return sum;
	}

	bool init_V(double V[3][1000], int N, double v_max)
	{
		double V_x, V_y, V_z, meanX=0, meanY=0, meanZ=0;
		
		//random velocities
		for (int i=0; i<N; i++)
		{
			V_x=displacement(v_max);
			V_y=displacement(v_max);
			V_z=displacement(v_max);
		
			V[0][i]=V_x;
			V[1][i]=V_y;
			V[2][i]=V_z;
		
			meanX+=V_x;
			meanY+=V_y;
			meanZ+=V_z;
		}
		
		//calculating mean velocities
		meanX=meanX/N;
		meanY=meanY/N;
		meanZ=meanZ/N;
		
		//eliminating mean velocities - makes them equal zero
		for (int i=0; i<N; i++)
		{
			V[0][i]-=meanX;
			V[1][i]-=meanY;
			V[2][i]-=meanZ;
		}
		return true;
	}

		
	bool periodic_bc(double R[3][1000], int N)
	{
		for (int i=0; i<N; i++)
		{
					R[0][i]=R[0][i]-2*(int)R[0][i];
					R[1][i]=R[1][i]-2*(int)R[1][i];
					R[2][i]=R[2][i]-2*(int)R[2][i];
		}
		return true;
	}

	
	bool init_R(double R[3][1000], double init_coord, double cell_vector, int N_cells, double displacement_ampl)
	{
		int ijk=0;
		for (int i=0; i<N_cells; i++)
		{
			for (int j=0; j<N_cells; j++)
			{
				for (int k=0; k<N_cells; k++)
				{
					R[0][ijk]=init_coord+i*cell_vector+displacement(displacement_ampl);
					R[1][ijk]=init_coord+j*cell_vector+displacement(displacement_ampl);
					R[2][ijk]=init_coord+k*cell_vector+displacement(displacement_ampl);
					ijk++;
				}
			}
		}
		return true;
	}
		
	bool map_multiply(double coef, double * arr, int arr_size)
	{
		for(int i=0; i<arr_size; i++)
		{
			*(arr+i) *= coef;
		}
	}

	double count_UF(double R[3][1000], double **F, double N, double H, double sigma)
	{
		double U, Uc = 0.;
		U=Uc;
		double D_pot=0.;
		double xi,yi,zi,xij,yij,zij,rij2,rs2,rs6,rs12, rij;
		
		for(int i=0; i<N; i++)
		{
			F[0][i]=0.;
			F[1][i]=0.;
			F[2][i]=0.;
		}
		
		for (int i=0; i<N-1; i++) //wybieramy i-ty atom...
		{
			xi=R[0][i];
			yi=R[1][i];
			zi=R[2][i];
			for (int j=i+1; j<N; j++) //...i obliczamy odległość od j-tego w x,y i z, (ij i ji to to samo, więc i+1<j<N)
			{
				xij=xi-R[0][j];
				yij=yi-R[1][j];
				zij=zi-R[2][j];
				xij-=2.*(int)xij; //periodyczne war. brzegowe
				yij-=2.*(int)yij;
				zij-=2.*(int)zij;
				rij2=pow(xij, 2.)+pow(yij, 2.)+pow(zij, 2.); //sferyczne obcięcie
				rij=sqrt(rij2);
				//potencjał LJ: Uij=4 epsilon_k( (sigma/rij)^6 - (sigma/rij)^12)
				//UWAGA: układ niestabilny! Trzeba obcięty potencjał...
				if(rij2<=1.0)
				{
					rs2=sigma_red2/rij2;
					rs6=pow(rs2, 3); //->(sigma/rij)^6     epsilon_k nie ma bo sigma_red
					rs12=rs6*rs6; //->(sigma/rij)^12
					U+=rs12-rs6-Uoff-dUoff*rij;
					
					//f = - grad U = 24epsilon_k/rij^2 (2 (sigma/rij)^-12 - (sigma/rij)^-6)) 
					//w jedn. zred: f= 24/rij2(2 (sigma/rij)^-12 - (sigma/rij)^-6))
					//czyli f= 24*D_pot
					
					D_pot=(2.0*rs12-rs6)/rij2;
					
					F[0][i]+=D_pot*xij-dUoff*rij;
					F[1][i]+=D_pot*yij-dUoff*rij;
					F[2][i]+=D_pot*zij-dUoff*rij;
					F[0][j]-=D_pot*xij+dUoff*rij;
					F[1][j]-=D_pot*yij+dUoff*rij;
					F[2][j]-=D_pot*zij+dUoff*rij;
				}
			}
		}
		U*=4.;
		for(int i=0;i<N;i++)
		{
			F[0][i]*=24.;
			F[1][i]*=24.;
			F[2][i]*=24.;
		}
		return U;
	}
	
	double count_Ek(double V[3][1000], int size)
	{
		return (sum_pow2_array(V[0],size)+sum_pow2_array(V[1],size)+sum_pow2_array(V[2],size))/(2.*dt_red*dt_red);
	}
	
	
	double count_Tred(double V[3][1000], int size)
	{
		return epsilon_k*count_Ek(V,size)/(1.5*(size-1.));
	}
	

	bool scale_V(double V[3][1000], int size, double coef)
	{
		map_multiply(coef, V[0], size);
		map_multiply(coef, V[1], size);
		map_multiply(coef, V[2], size);
		return true;
	}
}

#endif // FUNCTIONS_H