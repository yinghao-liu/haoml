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
	virtual const bool is_table(void);
	virtual const bool is_mapp(void);
	virtual const bool is_arrayy(void);
	
	void set_annot(string &annot);
	string &get_annot(void);
	shared_ptr<table> as_table(void);
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
	void insert(string &key, string &value, string &annot);
	void show_mapp(const string &table_name);

	string &index(const string &key);
private:
	map<string, mapvalue> _mapp;
};//end of mapp

struct arrayvalue{
	string annotation;
	vector<string> _arrayvalue;
};//end of arrayvalue 

class arrayy : public base{
public:
	virtual const bool is_arrayy(void) override;
	void append(vector<string> &data, string &annot);
	void show_arrayy(const string &table_name);
	vector<string> &index(size_t num);
private:
	vector<arrayvalue> _arrayy;
};//end of array

class table : public base{
public:
	virtual const bool is_table(void) override;
	bool empty(void);
	void show_table(void);
	shared_ptr<base> &operator[](const string &key);

	map<string, shared_ptr<base>>::iterator find(string &key);
	map<string, shared_ptr<base>>::iterator end(void);
	map<string, shared_ptr<base>>::iterator begin(void);

	map<string, shared_ptr<base>> _table;
};//end of table






}//end namespace haoml

#endif