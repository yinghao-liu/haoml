#include <iostream>
#include "haoml.h"
using namespace std;
using namespace haoml;
int main(void)
{
	parser config; 
	auto ptr = config.build("config.haoml");
	auto &root = *ptr;

	ptr->show_table();
	cout<<"---------------------------"<<endl;
	root["table_a"]->as_mapp()->index("a") = "1234a";
	root["table_a"]->as_mapp()->erase("b");
	auto &b = root["array_b"]->as_arrayy()->index(0)[1];
	
	auto &arra = *root["array_b"]->as_arrayy();
	for(auto i=arra.begin(); i!=arra.end(); ){
		i=arra.erase(i);
	}
	ptr->write("config.haoml");
	return 0;
}
