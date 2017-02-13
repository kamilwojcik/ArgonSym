#include <iostream>
#include "functions.h"
#include "record.h"
#include "record_ext.h"
#include "iofile_interface.h"

///////////////TESTS////////////////

using namespace sym;

int displacement_test()
{
	cout<<"\n/////////////////////////\ndisplacement test\n"<<endl;
	for (int i=0; i<5; ++i)
	{
		cout<<"Ampl=0.5: "<<displacement(0.5)<<" ampl=2: "<<displacement(2)<<endl;
	}
	cout<<endl;
	return 0;
}

int sum_array_test()
{
	cout<<"\n/////////////////////////\nsum_array test\n"<<endl;
	
	static long int seed = time(NULL);
	srand(seed);
	
	double a[5], sum=0;
	cout<<"Random test array: {";
	for (int i=0; i<5; ++i)
	{
		a[i]=rand()%11;
		cout<<a[i]<<"  ";
		sum+=a[i];
	}
	
	cout<<"}\n\nSum: "<<sum<<"\nsum_array result: "<<sum_array(a,5)<<'\n'<<endl;
	
	return 0;
}

int sum_pow2_array_test()
{
	cout<<"\n/////////////////////////\nsum_pow2_array test\n"<<endl;
	
	static long int seed = time(NULL);
	srand(seed);
	
	double a[5], a2[5], sum=0;
	cout<<"Random test array: {";
	for (int i=0; i<5; ++i)
	{
		a[i]=rand()%11;
		cout<<a[i]<<"  ";
	}
	
	cout<<"}\nMapped with pow2: {";
	for (int i=0; i<5; ++i)
	{
		a2[i]=a[i]*a[i];
		cout<<a2[i]<<"  ";
		sum+=a2[i];
	}
	
	cout<<"}\n\nSum: "<<sum<<"\nsum_pow2_array result: "<<sum_pow2_array(a,5)<<'\n'<<endl;
	
	return 0;
}

int map_multiply_test()
{
	cout<<"\n/////////////////////////\nmap_multiply test\n"<<endl;
	
	static long int seed = time(NULL);
	srand(seed);
	
	double a[5], a2[5], sum=0;
	cout<<"Random test array a[5]: {";
	for (int i=0; i<5; ++i)
	{
		a[i]=rand()%11;
		cout<<a[i]<<"  ";
	}
	
	map_multiply(2.5, a, 5);
	
	cout<<"}\nafter map_multiply(2.5, a, 5): {";
	for (int i=0; i<5; ++i)
	{
		cout<<a[i]<<"  ";
	}
	cout<<"}\n"<<endl;
	
	return 0;
}

int func_tests()
{
	displacement_test();
	sum_array_test();
	sum_pow2_array_test();
	map_multiply_test();
	
	return 0;
}

void test_record()
{
	cout<<"Test record. If V[4][1]==10 it works."<<endl;
	record rec(5);
	rec.init();
	rec.V[4][1]=10;
	rec.print();
}

void test_record_ext()
{
	cout<<"Test record_ext If V[3][1]==10 and Vperiod[3][1]==10 it works."<<endl;
	record_ext rec;
	rec.V[3][1]=10;
	rec.Vperiod[3][1]=10;
	rec.print();
}

void test_IOfile_interface()
{
	cout<<"Test of IOfile_interface. Preparing record:"<<endl;
	record_ext rec;
	
	IOfile_interface IOinterface(&rec);
	IOinterface.output_open("test_out.data");
	cout<<"Saving into a file 3 records with Xperiod[0][0]=entry index"<<endl;
	rec.Xperiod[0][0]=0;
	IOinterface.output_append();
	rec.Xperiod[0][0]=1;
	IOinterface.output_append();
	rec.Xperiod[0][0]=2;
	IOinterface.output_append();
	cout<<"Closing output and opening the same file as an input"<<endl;
	IOinterface.output_close();
	IOinterface.input_open("test_out.data");
	cout<<"Reading 0. entry using input_get()"<<endl;
	IOinterface.input_get();
	cout<<rec.Xperiod[0][0]<<endl;
	cout<<"Reading 1. entry using input_get(1)"<<endl;
	IOinterface.input_get(1);
	cout<<rec.Xperiod[0][0]<<endl;
	cout<<"Reading 2. entry using input_get()"<<endl;
	IOinterface.input_get();
	cout<<rec.Xperiod[0][0]<<endl;
	cout<<"Closing input file"<<endl;
}