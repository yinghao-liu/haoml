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
#include <stdexcept>

using std::exception;
using std::string;
using std::map;
using std::vector;
using std::shared_ptr;
namespace haoml{
/**
 * file_error = 0 : file relevant exception code\n
 * format_error : format relevant exception code\n
 * access_error : operate [] access error of base
 */
enum class haoml_error_code:uint8_t{
	file_error=0,
	format_error,
	access_error
};

class base;
class mapp;
class arrayy;
class table;
struct arrayvalue;


extern shared_ptr<base> make_base(void);
extern shared_ptr<table> make_table(void);
extern shared_ptr<mapp> make_mapp(void);
extern shared_ptr<arrayy> make_arrayy(void);

class haoexception : public exception{
	public:
		haoexception(const haoml_error_code &ecc, const string &err) : ec(ecc),message(err){}
		virtual const char* what(void) const noexcept{return message.c_str();}
		const haoml_error_code code(void) const noexcept{return ec;}
	private:
		haoml_error_code ec;
		string message;
};

class parser{
public:
	parser(void){};
	parser& operator=(const parser &pa) = delete;
	shared_ptr<table> build(const char *filename);
protected:
	shared_ptr<base> parser_data(string &data, string &annot, shared_ptr<base> &base_ptr);
	string strip(string &str);
	vector<string> split(char delimit, const string &str);
};//end class value

class comment{
public:
	void set_annot(const string &annot);
	string &get_annot(void);
private:
	string annotation;
};

class base : public comment, public std::enable_shared_from_this<base>{
public:
	virtual const bool is_mapp(void);
	virtual const bool is_arrayy(void);
	virtual string get_data(const string &table_name);
	virtual string &operator[](const string &key){throw haoexception(haoml_error_code::access_error, (string)"no data can be accessed by ["+key+"]");}
	virtual arrayvalue &operator[](const size_t pos){throw haoexception(haoml_error_code::access_error, "no data can be accessed by ["+std::to_string(pos)+"]");}

	shared_ptr<mapp>  as_mapp(void);
	shared_ptr<arrayy> as_arrayy(void);
};//end of base

struct mapvalue : public comment{
	string _mapvalue;
};//end of mapvalue


class mapp  : public base{
public:
	virtual const bool is_mapp(void) override;
	virtual string get_data(const string &table_name) override;
	virtual string &operator[](const string &key) override;

	void insert(const string &key, const string &value, const string &annot="");
	void erase(const string &key);
	bool empty(void);
	void show_mapp(const string &table_name);

private:
	map<string, mapvalue> _mapp;
};//end of mapp

struct arrayvalue : public comment{
	vector<string> _arrayvalue;
	string &operator[](const size_t pos)
	{
		return _arrayvalue[pos];
	}
	string &index(const size_t pos)
	{
		return _arrayvalue[pos];
	}
};//end of arrayvalue

class arrayy : public base{
public:
	virtual const bool is_arrayy(void) override;
	virtual string get_data(const string &table_name) override;
	virtual arrayvalue &operator[](const size_t pos) override;

	void append(vector<string> &data, const string &annot="");
	vector<arrayvalue>::iterator erase(vector<arrayvalue>::iterator pos);

	void show_arrayy(const string &table_name);
	bool empty(void);
	vector<arrayvalue>::iterator begin(void);
	vector<arrayvalue>::iterator end(void);
private:
	vector<arrayvalue> _arrayy;
};//end of array

class table : public comment{
public:
	bool empty(void);
	void show_table(void);
	void erase(const string &key);
	void clear(void);
	void write(const char *filename);
	base &operator[](const string &key);
	shared_ptr<base> index(const string &key);
	bool is_null(const string &key);
	void insert(const string &key, shared_ptr<base> ptr_base);
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
