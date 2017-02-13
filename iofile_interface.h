#ifndef IOFILE_INTERFACE_H
#define IOFILE_INTERFACE_H

#include <iostream>
#include <fstream>
#include "record_ext.h"

using namespace std;

class IOfile_interface
{
	int record_size;
public:
	std::ifstream in_file;
	std::ofstream out_file;
	record_ext *rec;
	
	IOfile_interface(record_ext *buffer);
	
	bool input_open(const char * in_file_name);
	void input_close();
	
	bool output_open(const char * out_file_name);
	void output_close();
	
	bool output_append();
	bool input_get(int index);
	bool input_get();
};

#endif // IOFILE_INTERFACE_H
