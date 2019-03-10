#include <iostream>
#include "haoml/haoml.h"
using namespace std;
using namespace haoml;

void access_and_data_modify(void)
{
	parser config; 
	shared_ptr<table> ptr;
	try {
		ptr = config.build("config.haoml");
	} catch (haoexception  e) {
		if (haoml_error_code::file_error == e.code()) {
			cout<<e.what()<<endl;	
			return;
		}	
	}
	auto &root = *ptr;
	/*access*/
	cout<<root["students"]["wang"]<<endl;
	cout<<root["teacher"][0][0]<<endl;
	/*data modify*/
	try {
		root["students"]["li"] = "31"; 
		/* there is no element named "wang" in root["teacher"], 
		 * and root["teacher"] is an arrayy, so, here should 
		 * raise an exception
		 */
		root["teacher"]["wang"]= "80";
	} catch (haoexception &e) {
		cout<<"access error"<<endl;
	}
	/*show whole table*/	
	cout<<"whole file-----"<<endl;
	root.show_table();
}

void access_and_structure_modify(void)
{
	parser config; 
	auto ptr = config.build("config.haoml");
	auto &root = *ptr;

	if (root["students"].is_arrayy()) {
		cout<<"students is arrayy"<<endl;
	}
	auto &arra = *root["students"].as_arrayy();
	for (auto i=arra.begin(); i!=arra.end(); ) {
		if ("100" != i->index(2)) {
			i=arra.erase(i);
		} else {
			i++;
		}
	}
	auto &mapa = *root["teacher"].as_mapp();
	mapa.erase("li");
	root.erase("reserved");
	cout<<"-----whole file-----"<<endl;
	root.show_table();
}

void generator(void)
{
	table root;
	root.set_annot("head declare");
	auto &students = *root.to_arrayy("students");

	students.set_annot("here is students,format is name|grade");
	auto &a0 = students.append("information of frank");
	a0.append("frank");
	a0.append("12");
	auto &a1 = students.append();
	a1.append("evan");
	a1.append("10");

	auto &teacher = *root.to_mapp("teacher");
	teacher.set_annot("format is name=age");
	teacher["li"]   = "40";
	teacher["wang"] = "32";

	
	auto &master = *root.to_mapp("master");;
	master.set_annot("master format is name=age");
	master.insert("feng", "33");
	master.insert("chen", "44", "chen is 44 years old");
	cout<<"-----whole file"<<endl;
	root.show_table();
	root.write("generator.haoml");
}
int main(void)
{
	cout<<"-------------access_and_data_modify--------------"<<endl;
	access_and_data_modify();
	//cout<<"------------access_and_structure_modify---------------"<<endl;
	//access_and_structure_modify();
	cout<<"------------generator---------------"<<endl;
	generator();
	
	return 0;
}
