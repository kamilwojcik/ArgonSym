#include "data.h"
#include "functions.h"
#include "string"
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "record_ext.h"
#include "iofile_interface.h"
#include "data.h"

//CERN ROOT
#include "TH1D.h"
#include "TCanvas.h"

using namespace std;
using namespace sym;

int main()
{
	record_ext rec_current, rec_previous;
	double MSD[1000];
	double VCF[1000];
	double CNORM[1000];
	double T[1000];
	
	double del=0.1e-10/H;
	int maxbin=round(1.0/del);
	double G[maxbin];
	cout<<"del: "<<del<<endl;
	cout<<"maxbin: "<<maxbin<<endl;
	
	IOfile_interface IOfile(&rec_current);
	IOfile.input_open("symulacja.data");
	
	double xij, yij, zij;
	int kmax=(kroki_s/co_ile_plik);
	double ktmax;
	double ncor;
	int kmin=0;
	int kct;
	double dtt=co_ile_plik*0.01;
	cout<<"L: "<<L<<endl;
	for(int k0=0; k0<kmax/2; k0++) 
	{
		IOfile.input_get(k0);
		rec_previous.copy(rec_current);
		
		ncor=(kmax-kmin)/2;
		ktmax=min(kmax,k0+kmax/2);
		for(int kt=k0;kt<ktmax;kt++)
		{
			IOfile.input_get(kt);
			
			kct=kt-k0;
			
		    for(int k=0;k<N;k++)
		    {
				xij=rec_previous.R[0][k]-rec_current.R[0][k];	
				yij=rec_previous.R[1][k]-rec_current.R[1][k];	
				zij=rec_previous.R[2][k]-rec_current.R[2][k];	
				MSD[kct]+=xij*xij+yij*yij+zij*zij;
				VCF[kct]+=rec_previous.V[0][k]*rec_current.V[0][k]+rec_previous.V[1][k]*rec_current.V[1][k]+rec_previous.V[2][k]*rec_current.V[2][k];
		    }
		    CNORM[kct]+=1.0;
		}
		if ((1000*k0*2/kmax)%10==0) cout<<1+(100*k0)*2/kmax<<" %\r"<<flush;
	}
	cout<<endl;
	        
	double AA=VCF[0];
	TH1D *Vcf=new TH1D("VCF","VCF",kmax/2,0,(double)kmax/2.*dtt);    
	TH1D *Msd=new TH1D("MSD","MSD",kmax/2,0,(double)kmax/2.*dtt);    
	    
	for(int a=0;a<kmax/2;a++)
	{
		MSD[a]/=CNORM[a];
		T[a]=double(a)*dtt;
		VCF[a]/=CNORM[a];
		VCF[a]/=AA;
		Msd->Fill(T[a],(MSD[a]*H*H)/1e-20);
		Vcf->Fill(T[a],VCF[a]);
	}
	
	TCanvas *cnv=new TCanvas();
	cnv->cd();
	Msd->Draw();
	cnv->SaveAs("MSD.png");
	Vcf->Draw();
	cnv->SaveAs("VCF.root");
	cnv->Close();
	delete Msd;
	delete Vcf;

	return 0;
}
