/*
 * Copyright (C) 2018 francis_hao <francis_hao@126.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <iostream>
#include <fstream>
#include <string.h>
#include "haoml.h"
using namespace std;
using namespace haoml;
using std::make_shared;

///namespace haoml
namespace haoml{

shared_ptr<base> make_base(void)
{
	return make_shared<base>();
}
shared_ptr<table> make_table(void)
{
	return make_shared<table>();
}
shared_ptr<mapp> make_mapp(void)
{
	return make_shared<mapp>();
}
shared_ptr<arrayy> make_arrayy(void)
{
	return make_shared<arrayy>();
}

}
/***********parser***************/
/**
parser and construct
@param filename the filename which should be parsed
@return a shared pointer of table, 
\verbatim
		inside, if object is mapp, it will return shared_ptr<mapp> when use as_mapp()
				if object is arrayy, it will return shared_ptr<arrayy> when use as_arrayy()
				if object is null, it will return nullptr either use as_mapp() or as_arrayy()
\endverbatim
@see class table
@throws haoexception : haoml_error_code::file_error   open file error
@throws	haoexception : haoml_error_code::format_error
*/
shared_ptr<table> parser::build(const char *filename)
{
	ifstream fs(filename);
	if (!fs.is_open()){
		throw haoexception(haoml_error_code::file_error, (string)"open file error, " + filename + ": " + strerror(errno));
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
		/*********here is end of a block or nothing************/	
		if (0 == line.size()){
			/****************config file head**********************/
			if (root->empty() && !annot.empty()){
				root->set_annot(annot);
			}
			if (!_table.empty()){
				if (root->is_null(_table)){
					root->insert(_table, make_base());
				}
				base_ptr->set_annot(tab_annot);
				_table.clear();
				tab_annot.clear();
				base_ptr = nullptr;

			}
			annot.clear();

			if (!fs.good()){
				break;
			}else{
				continue;
			}
		}

		switch (line[start=line.find_first_not_of("\t ")]){
		case '#':
			annot += line + '\n';
			break;
		case '[':
			end = line.rfind(']');
			if (string::npos == end){
				throw haoexception(haoml_error_code::format_error, (string)"format error: " + line);
			}
			if (!_table.empty()){
				throw haoexception(haoml_error_code::format_error, (string)"sorry! table can not be nested, current table is: " + _table);
			}
			_table = line.substr(start+1, end-start-1);
			if (root->find(_table) != root->end()){//exsit
				throw haoexception(haoml_error_code::format_error, (string)"table is repeat : " + _table);
			}
			tab_annot = annot;
			annot.clear();
			break;
		default:
			/*mixed type would return nullptr*/
			if (nullptr == parser_data(line, annot, base_ptr)){
				annot.clear();
				continue;
			}
			annot.clear();

			// data without a section([xxx]) ahead
			if (_table.empty()){
				base_ptr.reset();
				continue;
			}
			// if this line is the first one after a section
			if(root->is_null(_table)){
				root->insert(_table, base_ptr);
			}
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

	//delim = data.find('|');
	//if (string::npos != delim){//array mode

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
///strip the space(' ') and tab('\t') of a string
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
/**
	split a string which delimited by a specified delimiter
	@param  delimit : delimiter
	@param  str : raw string
	@return a vector after split
*/
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
/**************comment***************/
void comment::set_annot(const string &annot)
{
	if (annot.empty()){
		return;
	}
	annotation = ('#'==annot.front())?annot:("#" + annot);
	annotation += ('\n'==annot.back())?"":"\n";
}
string &comment::get_annot(void)
{
	return annotation;
}
/**************base****************/
const bool base::is_mapp(void)
{
	return false;
}
const bool base::is_arrayy(void)
{
	return false;
}
string base::get_data(const string &table_name)
{
	string base_out = get_annot();
	base_out += "[" + table_name + "]" + '\n';
	base_out += '\n';
	return base_out;
}
/**
 * @retrun shared_ptr<mapp> if is_mapp() is true, or nullptr else 
 */
shared_ptr<mapp> base::as_mapp(void)
{
	if (is_mapp()){
		return static_pointer_cast<mapp>(shared_from_this());
	}
	return nullptr;
}
/**
 * @retrun shared_ptr<arrayy> if is_arrayy() is true, or nullptr else 
 */
shared_ptr<arrayy> base::as_arrayy(void)
{
	if (is_arrayy()){
		return static_pointer_cast<arrayy>(shared_from_this());
	}
	return nullptr;
}

/**************mapp***************/
const bool mapp::is_mapp(void)
{
	return true;
}
string mapp::get_data(const string &table_name)
{
	string mapp_out = get_annot();
	mapp_out += "[" + table_name + "]" + '\n';
	for (auto &i : _mapp){
		mapp_out += i.second.get_annot();
		mapp_out += i.first + "=" + i.second._mapvalue + '\n';
	}
	mapp_out += '\n';

	return mapp_out;
}
string &mapp::operator[](const string &key)
{
	return _mapp[key]._mapvalue;
}

void mapp::insert(const string &key, const string &value, const string &annot)
{
	_mapp[key]._mapvalue  = value;
	_mapp[key].set_annot(annot);
}
void mapp::erase(const string &key)
{
	_mapp.erase(key);
}
bool mapp::empty(void)
{
	return _mapp.empty();
}
void mapp::show_mapp(const string &table_name)
{
	cout<<get_data(table_name);
}

/***********array*************/
const bool arrayy::is_arrayy(void)
{
	return true;
}
string arrayy::get_data(const string &table_name)
{
	string arrayy_out = get_annot();
	arrayy_out += "[" + table_name + "]" + '\n';

	for (auto &i : _arrayy){
		arrayy_out += i.get_annot();
		for (auto &j : i._arrayvalue){
			arrayy_out += j;
			arrayy_out += '|';
		}
		arrayy_out.pop_back();
		arrayy_out += '\n';
	}
	arrayy_out += '\n';

	return arrayy_out;
}
arrayvalue &arrayy::operator[](const size_t pos)
{
	return _arrayy[pos];	
}
void arrayy::append(vector<string> &data, const string &annot)
{
	arrayvalue value;
	value.set_annot(annot);
	value._arrayvalue = data;
	_arrayy.push_back(value);
}
vector<arrayvalue>::iterator arrayy::erase(vector<arrayvalue>::iterator pos)
{
	return _arrayy.erase(pos);
}
void arrayy::show_arrayy(const string &table_name)
{
	cout<<get_data(table_name);
}
bool arrayy::empty(void)
{
	return _arrayy.empty();
}
vector<arrayvalue>::iterator arrayy::begin(void)
{
	return _arrayy.begin();
}
vector<arrayvalue>::iterator arrayy::end(void)
{
	return _arrayy.end();
}

/*************table**************/
///erase all elements
void table::clear(void)
{
	_table.clear();
}
void table::erase(const string &key)
{
	_table.erase(key);
}
base &table::operator[](const string &key)
{
	return *_table[key];
}
shared_ptr<base> table::index(const string &key)
{
	return _table[key];
}
bool table::is_null(const string &key)
{
	return ((nullptr == _table[key])?true:false);
}
void table::insert(const string &key, shared_ptr<base> ptr_base)
{
	_table[key]=ptr_base;
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
string table::get_data(void)
{
	string table_out = get_annot();
	table_out += table_out.empty()? "" : "\n";
	for (auto &i : _table){
		table_out += i.second->get_data(i.first);
	}
	return table_out;
}

void table::show_table(void)
{
	cout<<get_data();
}
/**
	write the whole table to a file, the file would be truncated first
	@param filename : the file name
	@return none
*/
void table::write(const char *filename)
{
	string out = get_data();
	ofstream ofs(filename, ofstream::out|ofstream::trunc);
	if (!ofs.is_open()){
		cerr<<"open file error"<<endl;
		//throw 
		throw haoexception(haoml_error_code::format_error, (string)"open file error," + filename + ": " + strerror(errno));
	}
	ofs.write(out.c_str(), out.size());
	ofs.close();
}

