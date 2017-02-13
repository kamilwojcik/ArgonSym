#include "data.h"
#include "functions.h"
#include "string"
#include <iostream>
#include <fstream>
#include <cmath>
#include <math.h>
#include "record_ext.h"
#include "iofile_interface.h"

#include "TROOT.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TObject.h"

using namespace std;
using namespace sym;

int main()
{
	record_ext rec;
	IOfile_interface IOfile(&rec);
	IOfile.input_open("symulacja.data");
	
	int x,y;
	int i;

	double del=0.1e-10/H;
	int maxbin=int(1.0/del);
	double G[200];
	int nbin;
	double x_ij, y_ij, z_ij;
    double R_ij;		
	TH1D *radial=new TH1D("Rozklad przestrzenny","Rozklad przestrzenny",maxbin,0,maxbin/10.);

	
	for(i=0; i<(kroki_s/co_ile_plik); i++)
	{
		IOfile.input_get(i);
		cout<<"Record: "<<i+1<<"\r"<<flush;
		
		//count atoms in area limited by two spheres (rlower, rupper)
		for(int l=0;l<N-1;l++)
		{
		    for(int k=l+1;k<N;k++)
		    {
				{
					x_ij=rec.R[0][k]-rec.R[0][l];
					y_ij=rec.R[1][k]-rec.R[1][l];
					z_ij=rec.R[2][k]-rec.R[2][l];

					x_ij-=2.0*int(x_ij);
					y_ij-=2.0*int(y_ij);
					z_ij-=2.0*int(z_ij);
					
					R_ij=sqrt(x_ij*x_ij+y_ij*y_ij+z_ij*z_ij);
					nbin=int(R_ij/del)+1;
					
					if(nbin<=maxbin)
					{
						G[nbin]+=1.0;
					}
				}
		    }
		}
	}

	cout<<H<<endl;
	double rho=N/8.0;
	double rlower, rupper,vol,dind;

	//normalization and graduation (with respect to mean density rho and number of atoms N)
	for(int k=0;k<maxbin;k++)
	{
		rlower=k*del;
		rupper=(k+1)*del;

		vol=4.0*M_PI*(pow(rupper,3)-pow(rlower,3))/3.0;
		dind=vol*rho;
		G[k]=G[k]/N;
		G[k]/=dind*2000.;
		
		radial->AddBinContent(k,G[k]);
	}
	
	TCanvas *canvas=new TCanvas;
	canvas->cd();    
	radial->Draw();
	canvas->SaveAs("Rozklad.png");
	canvas->Close();
	delete radial;
	
	return 0;
}
