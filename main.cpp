#include "data.h"
#include "functions.h"
#include <string>
#include "record_ext.h"
#include "iofile_interface.h"
#include <cmath>

//root cern

#include "TROOT.h"
#include "TCanvas.h"
#include "TObject.h"
#include "TH1F.h"
#include "TH3F.h"
#include "TStyle.h"

using namespace std;
using namespace sym;


int main(void)
{

	double U, Temp_red, Temp, scal_coef;
	double Unoperiod, Temp_red_noperiod, Temp_noperiod;
	TH3F wykres("Symulacja","Argon",100,-1,1,100,-1,1,100,-1,1);
	TCanvas * cnv = new TCanvas();
	cnv->cd();
	wykres.SetMaximum(1);
	wykres.SetMinimum(-1);
	wykres.SetMarkerSize(0.5);
	wykres.SetMarkerStyle(8);
	gStyle->SetOptStat(0);
	
	record_ext record;
	IOfile_interface IO_file(&record);
	
	double **F;
	F=new double*[3];
	for (int i=0; i<3; i++)
	{
		F[i]=new double[(int)N];
	}
	
	//init spatial distribution: -1, -0.5, 0, 0.5  +- displacement; periodic x,y,z
	init_R(record.R,-1,A, M,ampl);
	//init velocities - mean == 0
	init_V(record.V, N, v_max);
	

	Temp_red = count_Tred(record.V,N);
	Temp = Temp_red;
	scal_coef = sqrt(T/Temp);
	
	cout<<"PO ZAINICJOWANIU\n";
	cout<<"Temperatura przed skalowaniem: "<<Temp<<endl;
	
	scale_V(record.V,N,scal_coef);
	Temp_red = count_Tred(record.V,N);
	Temp = Temp_red;
	cout<<"Temperatura po skalowaniu: "<<Temp<<endl;
	
	
	U=count_UF(record.R, F, N, H, sigma);
	cout<<"Energia potencjalna: "<<U<<endl;

	
	cout<<"\nROZPOCZYNANIE I ETAPU - DOCHODZENIE DO STANU ROWNOWAGI ("<<kroki_r <<" krokow)\nL="<<L<<endl;
	
	double temporary_temp;
	for(int t=0;t<kroki_r; t++)
	{
		for(int p=0; p<N; p++)
		{
			record.R[0][p]+=record.V[0][p]+0.5*F[0][p]*dt_red2; //X = X0 + V0*t + 1/2 at^2 w jedn. zred.
			record.R[1][p]+=record.V[1][p]+0.5*F[1][p]*dt_red2;
			record.R[2][p]+=record.V[2][p]+0.5*F[2][p]*dt_red2;
			
			record.V[0][p]+=0.5*F[0][p]*dt_red2;  //V = V0 + at w jedn. zred.
			record.V[1][p]+=0.5*F[1][p]*dt_red2;
			record.V[2][p]+=0.5*F[2][p]*dt_red2;
		}
		periodic_bc(record.R,N);
		
		U=count_UF(record.R, F, N, H, sigma);
		
		for(int p=0; p<N; p++)
		{
			record.V[0][p]+=0.5*F[0][p]*dt_red2;  //V = V0 + at w jedn. zred.
			record.V[1][p]+=0.5*F[1][p]*dt_red2;
			record.V[2][p]+=0.5*F[2][p]*dt_red2;
		}
		double meanX=0, meanY=0, meanZ=0;
		for(int p=0; p<N; p++)
		{
			meanX+=record.V[0][p];
			meanY+=record.V[1][p];
			meanZ+=record.V[2][p];
		}

		//calculating mean velocities
		meanX=meanX/N;
		meanY=meanY/N;
		meanZ=meanZ/N;
		
		//eliminating mean velocities - makes them equal zero
		for (int i=0; i<N; i++)
		{
			record.V[0][i]-=meanX;
			record.V[1][i]-=meanY;
			record.V[2][i]-=meanZ;
		}

		
		if (abs(T-Temp)>2)
		{
			temporary_temp=Temp;
			scal_coef = sqrt(T/Temp);
			scale_V(record.V,N,scal_coef);
		}
		
		Temp_red = count_Tred(record.V,N);
		Temp = Temp_red;
		
		if ((t+1)%(kroki_r/10)==0)
		{
			cout<<(t+1)*100/kroki_r<<"%: temp.: "<<temporary_temp<<"  temp. po skalowaniu: "<<Temp<<"  energia potencjalna: "<<U<<endl;
		}
	}
	cout<<"\nZROBIONE.\n\nROZPOCZYNANIE II ETAPU - SYMULACJA BEZ SKALOWANIA ("<<kroki_s<<" krokow)\n"<<endl;

	IO_file.output_open("symulacja.data");
	
	for (int i=0; i<3; i++)
	{
		copy(record.R[i], record.R[i]+(int)(N-1), record.Rnoperiod[i]);
		copy(record.V[i], record.V[i]+(int)(N-1), record.Vnoperiod[i]);
	}
	
	for(int t=0;t<kroki_s; t++)
	{
		for(int p=0; p<N; p++)
		{
			record.R[0][p]+=record.V[0][p]+0.5*F[0][p]*dt_red2; //X = X0 + V0*t + 1/2 at^2 w jedn. zred.
			record.R[1][p]+=record.V[1][p]+0.5*F[1][p]*dt_red2;
			record.R[2][p]+=record.V[2][p]+0.5*F[2][p]*dt_red2;
			
			record.V[0][p]+=0.5*F[0][p]*dt_red2;  //V = V0 + at w jedn. zred.
			record.V[1][p]+=0.5*F[1][p]*dt_red2;
			record.V[2][p]+=0.5*F[2][p]*dt_red2;
			
			record.Rnoperiod[0][p]+=record.Vnoperiod[0][p]+0.5*F[0][p]*dt_red2; //X = X0 + V0*t + 1/2 at^2 w jedn. zred.
			record.Rnoperiod[1][p]+=record.Vnoperiod[1][p]+0.5*F[1][p]*dt_red2;
			record.Rnoperiod[2][p]+=record.Vnoperiod[2][p]+0.5*F[2][p]*dt_red2;
			
			record.Vnoperiod[0][p]+=0.5*F[0][p]*dt_red2;  //V = V0 + at w jedn. zred.
			record.Vnoperiod[1][p]+=0.5*F[1][p]*dt_red2;
			record.Vnoperiod[2][p]+=0.5*F[2][p]*dt_red2;
		}
		periodic_bc(record.R,N);
		
		U=count_UF(record.R, F, N, H, sigma);
		
		for(int p=0; p<N; p++)
		{
			record.V[0][p]+=0.5*F[0][p]*dt_red2;  //V = V0 + at w jedn. zred.
			record.V[1][p]+=0.5*F[1][p]*dt_red2;
			record.V[2][p]+=0.5*F[2][p]*dt_red2;
			
			record.Vnoperiod[0][p]+=0.5*F[0][p]*dt_red2;  //V = V0 + at w jedn. zred.
			record.Vnoperiod[1][p]+=0.5*F[1][p]*dt_red2;
			record.Vnoperiod[2][p]+=0.5*F[2][p]*dt_red2;
		}
		double meanX=0, meanY=0, meanZ=0, meanXnoperiod=0, meanYnoperiod=0, meanZnoperiod=0;
		for(int p=0; p<N; p++)
		{
			meanX+=record.V[0][p];
			meanY+=record.V[1][p];
			meanZ+=record.V[2][p];
			
			meanXnoperiod+=record.Vnoperiod[0][p];
			meanYnoperiod+=record.Vnoperiod[1][p];
			meanZnoperiod+=record.Vnoperiod[2][p];
		}

		//calculating mean velocities
		meanX=meanX/N;
		meanY=meanY/N;
		meanZ=meanZ/N;
		
		meanXnoperiod=meanXnoperiod/N;
		meanYnoperiod=meanYnoperiod/N;
		meanZnoperiod=meanZnoperiod/N;
		
		//eliminating mean velocities - makes them equal zero
		for (int i=0; i<N; i++)
		{
			record.V[0][i]-=meanX;
			record.V[1][i]-=meanY;
			record.V[2][i]-=meanZ;
			
			record.Vnoperiod[0][i]-=meanXnoperiod;
			record.Vnoperiod[1][i]-=meanYnoperiod;
			record.Vnoperiod[2][i]-=meanZnoperiod;
		}
		
		Temp_red = count_Tred(record.Vnoperiod,N);
		Temp = Temp_red;
		
		if ((t+1)%100==0)
		{
			cout<<(t+1)/100<<"%: temp.: "<<Temp<<"  energia potencjalna: "<<U<<endl;
		}
		
		string name, title;
		if (t%co_ile_obrazek==0)
		{
			name=to_string(t)+".png";
			for(int p=0; p<N; p++)
			{
				wykres.Fill(record.R[0][p],record.R[1][p],record.R[2][p]);
			}
			
			title="Argon (iteration: ";
			title+=to_string(t)+")";
			wykres.SetTitle(title.c_str());
			wykres.Draw();
			cnv->SaveAs(name.c_str());
			wykres.Reset();
		}
		
		if (t%co_ile_plik==0)
		{
			IO_file.output_append();
		}
	}
	
	IO_file.output_close();
	cnv->Close();
	
	cout<<"\n\nKONIEC."<<endl;

	
	return 0;
}

