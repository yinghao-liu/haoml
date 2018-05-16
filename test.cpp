#include <iostream>
#include "haoml.h"
using namespace std;
using namespace haoml;
int main(void)
{
	parser config; 
	auto ptr = config.build("config.haoml");
	auto &root = *ptr;
	auto &stu = *root["students"];
	auto &tea = *root["teacher"];
	auto &res = *root["reserved"];

	cout<<stu[0][0]<<endl;
	cout<<tea["wang"]<<endl;
	if (stu.is_arrayy()){
		cout<<"stu is arrayy"<<endl;
	}


	cout<<"---------------------------"<<endl;
	root.show_table();
	cout<<"---------------------------"<<endl;

	/*
	auto &arra = *root["students"].as_arrayy();
	for(auto i=arra.begin(); i!=arra.end(); ){
		i=arra.erase(i);
	}
	*/
	root.write("config.haoml");
	return 0;
}
