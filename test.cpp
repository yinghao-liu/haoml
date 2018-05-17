#include <iostream>
#include "haoml.h"
using namespace std;
using namespace haoml;

void access_and_data_modify(void)
{
	parser config; 
	auto ptr = config.build("config.haoml");
	auto &root = *ptr;

	/*access*/
	cout<<root["students"][0][0]<<endl;
	cout<<root["teacher"]["wang"]<<endl;
	/*data modify*/
	root["students"][0][0] = "francis"; 
	root["teacher"]["wang"]= "80";
    /*show whole table*/	
	cout<<"-----whole file-----"<<endl;
	root.show_table();
}

void access_and_structure_modify(void)
{
	parser config; 
	auto ptr = config.build("config.haoml");
	auto &root = *ptr;

	if (root["students"].is_arrayy()){
		cout<<"students is arrayy"<<endl;
	}
	auto &arra = *root["students"].as_arrayy();
	for(auto i=arra.begin(); i!=arra.end(); ){
		if ("100" != i->index(2)){
			i=arra.erase(i);
		}else{
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

	auto ptr_stu = make_arrayy();
	auto &students = *ptr_stu;

	students.set_annot("here is students,format is name|grade");
	vector<string> member;
	member.push_back("frank");
	member.push_back("12");
	students.append(member);
	member.clear();
	member.push_back("evan");
	member.push_back("10");
	students.append(member);
	root.insert("students", ptr_stu);


	auto ptr_tea = make_mapp();
	auto &teacher = *ptr_tea;
	students.set_annot("format is name=age");
	teacher.insert("li", "40");
	teacher.insert("wang", "32", "wang is 32 years old");
	root.insert("teacher", ptr_tea);

	cout<<"-----whole file-----"<<endl;
	root.show_table();
	root.write("generator.haoml");
}
int main(void)
{
	cout<<"-------------access_and_data_modify--------------"<<endl;
	access_and_data_modify();
	cout<<"------------access_and_structure_modify---------------"<<endl;
	access_and_structure_modify();
	cout<<"------------generator---------------"<<endl;
	generator();
	
	return 0;
}
