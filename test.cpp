#include <iostream>
#include "haoml.h"
using namespace std;
using namespace haoml;
int main(void)
{
	parser config; 
	auto ptr = config.build("config.haoml");
	ptr->show_table();
	cout<<"---------------------------"<<endl;
	auto &a = *ptr;
	
	cout<<a["table_a"]->as_mapp()->index("a")<<endl;
	auto &b = a["array_b"]->as_arrayy()->index(0)[1];
	cout<<b<<endl;
	b = "20";
	ptr->show_table();
	return 0;
}
