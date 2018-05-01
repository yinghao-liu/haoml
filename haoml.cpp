#include <iostream>
#include <fstream>
#include "haoml.h"
using namespace std;
using namespace haoml;

value::value(const char *filename)
{
	ifstream fs(filename);
	if (!fs.is_open()){
		cerr<<"open file error"<<endl;
		//throw 
	}

	string line;

	string table;
	string annot;
	string chunk;
	size_t start;
	size_t end;
	while (1){
		getline(fs, line);
		if (!fs.good()){
			break;
		}
		
		if (0 == line.size()){
			cout<<"null line"<<endl;
		}
		switch (line[start=line.find_first_not_of("\t ")]){
		case '#':
			line  += '\n';
			annot += line;
			break;
		case '[':
			end = line.rfind(']');
			if (string::npos == end){
				cerr<<"format error: "<<line<<endl;
				//throw
			}
			if (0 != table.size()){
				cerr<<"sorry! table can not be nested, current table is: "<<table<<endl;
				//throw
			}
			table = line.substr(start, end-start);
			if (0 != annot.size()){
				storage[table]["#"]=annot;
			}
		//default:
			
		
		}
		chunk+=line;
	}
	
}


/**************base****************/
bool base::is_table(void)
{
	return false;
}
bool base::is_mapp(void)
{
	return false;
}
bool base::is_array(void)
{
	return false;
}
bool base::is_mapvalue(void)
{
	return false;
}
bool base::is_arrayvalue(void)
{
	return false;
}
/**************mapvalue****************/
bool mapvalue::is_mapvalue(void)
{
	return true;
}
/**************arrayvalue****************/
bool arrayvalue::is_arrayvalue(void)
{
	return true;
}
/**************mapp***************/
bool mapp::is_mapp(void)
{
	return true;
}
/***********array*************/
bool array::is_array(void)
{
	return true;
}
/*************table**************/
bool table::is_table(void)
{
	return true;
}







