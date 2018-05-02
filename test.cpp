#include <iostream>
#include "haoml.h"
using namespace std;
using namespace haoml;
int main(void)
{
	parser config; 
	auto ptr = config.build("config.haoml");
	ptr->show_table();
	return 0;
}
