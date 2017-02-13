#include "iofile_interface.h"
#include <iostream>
#include <fstream>

using namespace std;

IOfile_interface::IOfile_interface(record_ext *buffer)
{
	rec=buffer;
	record_size=sizeof(*buffer);
	cout<<"IOfile_interface buffer size: "<<record_size<<endl;
}

bool IOfile_interface::input_open(const char* in_file_name)
{
	in_file.open(in_file_name, std::ios::in | std::ios::binary);
	return in_file.good();
}

bool IOfile_interface::output_open(const char* out_file_name)
{
	out_file.open(out_file_name, std::ios::trunc | std::ios::binary);
	return out_file.good();
}

void IOfile_interface::input_close()
{
	in_file.close();
}

void IOfile_interface::output_close()
{
	out_file.close();
}

bool IOfile_interface::input_get(int index)
{
	in_file.seekg(record_size*index,in_file.beg);
	in_file.read((char*)rec, record_size);
}

bool IOfile_interface::input_get()
{
	in_file.read((char*)rec, record_size);
}

bool IOfile_interface::output_append()
{
	out_file.write((char*)rec, record_size);
}

