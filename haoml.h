#ifndef _HAOML_H_
#define _HAOML_H_

#include <string>
#include <map>
using std::string;
using std::map;
using std::enable_shared_from_this;

namespace haoml{

class value{
public:
	value(const char *filename);
	map<string, map<string,string>> storage;
};//end class value



class base : public enable_shared_from_this<base>{
public:
	virtual const bool is_table(void);
	virtual const bool is_mapp(void);
	virtual const bool is_array(void);
	virtual const bool is_mapvalue(void);
	virtual const bool is_arrayvalue(void);
	shared_ptr<table> as_table(void);
	shared_ptr<value> as_mapp(void);
	shared_ptr<array> as_array(void);
	shared_ptr<array> as_value(void);
private:
	string annotation;
};//end of base

class mapvalue : public base{
public:
	virtual const bool is_mapvalue(void) override;
private:
	string _mapvalue
};//end of mapvalue 

class arrayvalue : public base{
public:
	virtual const bool is_arrayvalue(void) override;
private:
	vector<string> _arrayvalue
};//end of arrayvalue 

class mapp  : public base{
public:
	virtual const bool is_mapp(void) override;
private:
	map<string, string> _map;
};//end of mapp

class array : public base{
public:
	virtual const bool is_array(void) override;
private:
	vector<arrayvalue> _array;
};//end of array

class table : public base{
public:
	virtual const bool is_table(void) override;
	const shared_ptr<base> get(string &key);	
private:
	map<string, shared_ptr<base>> _table;
};//end of table





}//end namespace haoml

#endif
