#include <iostream>
#include <fstream>
#include "haoml.h"
using namespace std;
using namespace haoml;
using std::make_shared;

/***********parser***************/
shared_ptr<table> parser::make_table(void)
{
	return make_shared<table>();
}
shared_ptr<mapp> parser::make_mapp(void)
{
	return make_shared<mapp>();
}
shared_ptr<arrayy> parser::make_arrayy(void)
{
	return make_shared<arrayy>();
}

shared_ptr<table> parser::build(const char *filename)
{
	ifstream fs(filename);
	if (!fs.is_open()){
		cerr<<"open file error"<<endl;
		//throw 
	}
	shared_ptr<table> root = make_table();
	shared_ptr<base> base_ptr;

	string line;

	string _table;
	string annot;
	string tab_annot;
	size_t start;
	size_t end;
	while (1){
		getline(fs, line);
		if (!fs.good()){
			break;
		}
		/*********here is end of a block or nothing************/	
		if (0 == line.size()){
			/****************config file head**********************/
			if (root->empty() && !annot.empty()){
				root->set_annot(annot);
				annot.clear(); 
			}
			if (!_table.empty() && !tab_annot.empty() && nullptr != (*root)[_table]){
				(*root)[_table]->set_annot(tab_annot);
				_table.clear();
				annot.clear();
				tab_annot.clear();
				base_ptr = nullptr;

			}
			cout<<"null line"<<endl;
			continue;
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
			if (!_table.empty()){
				cerr<<"sorry! table can not be nested, current table is: "<<_table<<endl;
				//throw
			}
			_table = line.substr(start+1, end-start-1);
			if (root->find(_table) != root->end()){//exsit
				cerr<<"table is repeat : "<<_table<<endl;
				//throw
			}
			tab_annot = annot;
			annot.clear();
			break;
		default:
			if (nullptr == parser_data(line, annot, base_ptr)){
				return nullptr;
			}
			if (nullptr == (*root)[_table]){
				(*root)[_table] = base_ptr;
			}
			annot.clear();
		}
	}
	return root;	
}
shared_ptr<base> parser::parser_data(string &data, string &annot, shared_ptr<base> &base_ptr)
{
	size_t delim;

	delim = data.find('=');
	if (string::npos != delim){//key-value mode
		string key;
		string value;
		key = data.substr(0, delim);
		value = data.substr(delim+1);
		key = strip(key);
		value = strip(value);
		if (nullptr == base_ptr){
			base_ptr = make_mapp();
		}
		if (!base_ptr->is_mapp()){
			cerr<<"mixed type"<<endl;
			return nullptr;
		}
		shared_ptr<mapp> map_ptr = base_ptr->as_mapp();
		map_ptr->insert(key, value, annot);	
		return base_ptr;
	}

	delim = data.find('|');
	if (string::npos != delim){//array mode
		
		if (nullptr == base_ptr){
			base_ptr = make_arrayy();
		}
		if (!base_ptr->is_arrayy()){
			cerr<<"mixed type"<<endl;
			return nullptr;
		}
		shared_ptr<arrayy> array_ptr = base_ptr->as_arrayy();
		auto vect_data = split('|', data);
		array_ptr->append(vect_data, annot);
		return base_ptr;	
	}
	return nullptr;
}
string parser::strip(string &str)
{
	size_t first;
	size_t last;
	first = str.find_first_not_of(" \t");
	last  = str.find_last_not_of(" \t");
	if (string::npos != first){
		return str.substr(first, last-first+1);
	}
	return str.substr(0, 0);
}

vector<string> parser::split(char delimit, const string &str)
{
	size_t start=0;
	size_t end=0;
	string sub;
	vector<string> result;
	while (string::npos != (end=str.find(delimit, start))){
		sub = str.substr(start, end-start);
		result.push_back(sub);
		start = end+1;
	}   
	/*the last part after the last delimit*/
	sub = str.substr(start);
	result.push_back(sub);
	return result;
}

/**************base****************/
const bool base::is_table(void)
{
	return false;
}
const bool base::is_mapp(void)
{
	return false;
}
const bool base::is_arrayy(void)
{
	return false;
}
shared_ptr<table> base::as_table(void)
{
	if (is_table()){
		return static_pointer_cast<table>(shared_from_this());
	}
	return nullptr;
}
shared_ptr<mapp> base::as_mapp(void)
{
	if (is_mapp()){
		return static_pointer_cast<mapp>(shared_from_this());
	}
	return nullptr;
}
shared_ptr<arrayy> base::as_arrayy(void)
{
	if (is_arrayy()){
		return static_pointer_cast<arrayy>(shared_from_this());
	}
	return nullptr;
}

void base::set_annot(string &annot)
{
	annotation = annot;
}
string & base::get_annot(void)
{
	return annotation;
}
/**************mapp***************/
const bool mapp::is_mapp(void)
{
	return true;
}
void mapp::insert(string &key, string &value, string &annot)
{
	_mapp[key]._mapvalue  = value;
	_mapp[key].annotation = annot;
}
void mapp::show_mapp(const string &table_name)
{
	cout<<get_annot();
	cout<<"["<<table_name<<"]"<<endl;

	for (auto &i : _mapp){
		cout<<i.second.annotation;
		cout<<i.first<<"="<<i.second._mapvalue<<endl;
	}
	cout<<endl;
}

string &mapp::index(const string &key)
{
	return _mapp[key]._mapvalue;
}
/***********array*************/
const bool arrayy::is_arrayy(void)
{
	return true;
}
void arrayy::append(vector<string> &data, string &annot)
{
	arrayvalue value;
	value.annotation  = annot;
	value._arrayvalue = data;
	_arrayy.push_back(value);
}
void arrayy::show_arrayy(const string &table_name)
{
	cout<<get_annot();
	cout<<"["<<table_name<<"]"<<endl;
	for (auto &i : _arrayy){
		cout<<i.annotation;
		string data;
		for (auto &j : i._arrayvalue){
			data+=j;
			data+='|';
		}
		data.pop_back();
		cout<<data<<endl;
	}
	cout<<endl;
}
vector<string> &arrayy::index(size_t num)
{
	return _arrayy[num]._arrayvalue;
}
/*************table**************/
const bool table::is_table(void)
{
	return true;
}
shared_ptr<base> &table::operator[](const string &key)
{
	return _table[key];
}
bool table::empty(void)
{
	return _table.empty();
}
map<string, shared_ptr<base>>::iterator table::find(string &key)
{
	return _table.find(key);
}
map<string, shared_ptr<base>>::iterator table::end(void)
{
	return _table.end();
}
map<string, shared_ptr<base>>::iterator table::begin(void)
{
	return _table.begin();
}

void table::show_table(void)
{
	cout<<get_annot()<<endl;;
	for (auto &i : _table){
		if (i.second->is_mapp()){
			i.second->as_mapp()->show_mapp(i.first);
		}
		if (i.second->is_arrayy()){
			i.second->as_arrayy()->show_arrayy(i.first);
		}
	}
}
