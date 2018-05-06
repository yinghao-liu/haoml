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
#ifndef _HAOML_H_
#define _HAOML_H_

#include <string>
#include <map>
#include <vector>
#include <memory>
using std::string;
using std::map;
using std::vector;
using std::shared_ptr;
namespace haoml{

class base;
class mapp;
class arrayy;
class table;


class parser{
public:
	parser(void){};
	parser& operator=(const parser &pa) = delete;
	shared_ptr<table> build(const char *filename);
protected:
	shared_ptr<base> make_base(void);
	shared_ptr<table> make_table(void);
	shared_ptr<mapp> make_mapp(void);
	shared_ptr<arrayy> make_arrayy(void);
	shared_ptr<base> parser_data(string &data, string &annot, shared_ptr<base> &base_ptr);

	string strip(string &str);
	vector<string> split(char delimit, const string &str);
private:
};//end class value



class base : public std::enable_shared_from_this<base>{
public:
	virtual const bool is_mapp(void);
	virtual const bool is_arrayy(void);
	virtual string get_data(const string &table_name);
	
	void set_annot(string &annot);
	string &get_annot(void);
	shared_ptr<mapp>  as_mapp(void);
	shared_ptr<arrayy> as_arrayy(void);
private:
	string annotation;
};//end of base

struct mapvalue{
	string annotation;
	string _mapvalue;
};//end of mapvalue 


class mapp  : public base{
public:
	virtual const bool is_mapp(void) override;
	virtual string get_data(const string &table_name);
	void insert(string &key, string &value, string &annot);
	void erase(string key);
	void show_mapp(const string &table_name);

	string &index(const string &key);
	string &operator[](const string &key);
private:
	map<string, mapvalue> _mapp;
};//end of mapp

struct arrayvalue{
	string annotation;
	vector<string> _arrayvalue;
	string &operator[](size_t pos)
	{
		return _arrayvalue[pos];
	}
};//end of arrayvalue 

class arrayy : public base{
public:
	virtual const bool is_arrayy(void) override;
	virtual string get_data(const string &table_name);
	void append(vector<string> &data, const string &annot);
	void erase(vector<arrayvalue>::iterator pos);

	void show_arrayy(const string &table_name);
	vector<string> &index(size_t pos);
	vector<string> &operator[](size_t pos);
	vector<arrayvalue>::iterator begin(void);
	vector<arrayvalue>::iterator end(void);

private:
	vector<arrayvalue> _arrayy;
};//end of array

class table : public base{
public:
	bool empty(void);
	void show_table(void);
	void clear(void);
	void write(const char *filename);
	shared_ptr<base> &operator[](const string &key);
	map<string, shared_ptr<base>>::iterator find(string &key);
	map<string, shared_ptr<base>>::iterator end(void);
	map<string, shared_ptr<base>>::iterator begin(void);
protected:
	virtual string get_data(void);
private:
	map<string, shared_ptr<base>> _table;
};//end of table






}//end namespace haoml

#endif
