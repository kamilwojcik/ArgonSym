#ifndef DATA_H
#define DATA_H

#include <cmath>

namespace sym
{
//parametry komorki
int M=10; //kostka o boku z M komorek elementarnych, tak jakby...
double N=pow(M,3.); //ile atomow
double A=2./double(M); //bok komorki elementarnej
double ampl=0.02; //wyhylenia początkowe

//stale fizyczne
const double n_av=6.022045e23; //Stała Avogadro
const double k_boltz=1.38066e-23; //st. Boltzmanna

//parametry symulacji - wykorzystane wielkości
const double rho=1395; // gęstość w [kg/m3] w punkcie potrójnym
const double T=87.3; //temperatura w [K], około t. wrzenia Ar
const double mass_g=40.; //masa molowa Ar
double mass_kg=mass_g/(n_av*1000); //masa atomu Ar w [kg]
const double epsilon_k=119.8; //temperatura odniesienia, [K]
double epsilon_j=epsilon_k*k_boltz; //energia odniesienia w [J]
const double dt=1e-14; //krok czasowy [s]
int kroki_r=1000; //kroki dochodzenia do równowagi
int kroki_s=10000; //kroki symulacji
int co_ile_plik=5;
int co_ile_obrazek=20;
//wielkości wyliczone
double L=cbrt((N*mass_kg)/rho); //dlugosc boku kostki [m]
double H=L/2.;
double dt_red=dt/sqrt(H*H*mass_kg/(epsilon_j)); //mianownik - czas, w jakim cząstka Ar o energii epsilon_j pokona h*sqrt(2)
double dt_red2=dt_red*dt_red;
double T_red=T/epsilon_k; //temp. zredukowana (w [epsilon_k])
double v_max=dt_red*sqrt(3.*T_red);
const double sigma = 3.41e-10;
double sigma_red = sigma/H;
double sigma_red2 = sigma_red*sigma_red;

/////////////////////
double dUoff=(((pow(sigma_red,6)-pow(sigma_red,12))-pow((sigma_red/0.98),6)-pow((sigma_red/0.98),12)))/0.02;
double Uoff=pow(sigma_red,6)-pow(sigma_red,12);
}

#endif