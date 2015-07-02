#pragma once



#include <string>
#include "tools.h"

namespace xd_Json
{
	
enum JsonType
{
	json_NULL,		//null
	json_BOOL,		//bool
	json_NUMBER,	//double
	json_INT64,		//__int64
	json_STRING,	//xd_JsonStringData
	json_ARRAY,		//xd_JsonVector<>
	json_OBJECT		//xd_JsonVector<>
};

//class iterator_obj;
class CJsonElem;

struct jsonObjElem;

class CJsonElem
{
	
public:
	typedef xd_JsonVector<CJsonElem>		json_Arr;
	typedef xd_JsonVector<jsonObjElem>		json_Obj;

	typedef json_Arr::iterator			iterator_arr;
	typedef json_Arr::const_iterator	const_iterator_arr;

	typedef json_Obj::iterator			iterator_obj;
	typedef json_Obj::const_iterator	const_iterator_obj;

	
private:

	union JOSN_DATA{
		bool	m_bool;
		double	m_number;
		__int64	m_int64;
		xd_JsonStringData m_string;
		json_Arr m_array;
		json_Obj m_object;
	}m_data;
	JsonType m_type;


public://构造函数,析构函数,拷贝函数
	CJsonElem():m_type(json_NULL){};
	CJsonElem(JsonType type);
	CJsonElem(double dfdata)		{m_type = json_NUMBER;	m_data.m_number = dfdata;};
	CJsonElem(__int64 lldata)	{m_type = json_INT64;	m_data.m_int64 = lldata;};
	CJsonElem(int lldata)		{m_type = json_INT64;	m_data.m_int64 = lldata;};
	CJsonElem(bool bdata)		{m_type = json_BOOL;	m_data.m_bool = bdata;};
	CJsonElem(const char* pcstrdata){m_type = json_STRING;	m_data.m_string.init(pcstrdata);};

	CJsonElem(const CJsonElem& elem):m_type(elem.m_type)			{copy(elem);};//COPY语义

	~CJsonElem();

public://拷贝函数
 	CJsonElem& operator = (const CJsonElem & elem)				{changeto(json_NULL); copy(elem);return *this;};//COPY语义

	CJsonElem& operator = (double dfdata)	 throw(json_err_badcast) {_sure(json_NUMBER);	m_type = json_NUMBER;	m_data.m_number = dfdata; return *this;};
	CJsonElem& operator = (__int64 lldata)	 throw(json_err_badcast) {_sure(json_INT64);	m_type = json_INT64;	m_data.m_int64 = lldata; return *this;};
	CJsonElem& operator = (int lldata)		 throw(json_err_badcast) {_sure(json_INT64);	m_type = json_INT64;	m_data.m_int64 = lldata; return *this;};
	CJsonElem& operator = (bool bdata)		 throw(json_err_badcast) {_sure(json_BOOL);	m_type = json_BOOL;		m_data.m_bool = bdata; return *this;};
	CJsonElem& operator = (const char* pcstrdata)throw(json_err_badcast) {_sure(json_STRING);	m_type = json_STRING;	m_data.m_string.copy(pcstrdata); return *this;};	

public:
	JsonType GetType()const {return m_type;};
	bool is(JsonType type) const {return type == m_type || (type == json_NUMBER && m_type == json_INT64);};
	void sure(JsonType type) const throw(json_err_badcast) {if (type!=m_type) throw json_err_badcast();}
	bool changeto(JsonType type);//本来就是相同的类型返回 false
	void clear() throw(json_err_badcast);
	int size() throw(json_err_badcast);

	bool& getBoolean()		throw(json_err_badcast){sure(json_BOOL);	return m_data.m_bool;	};
	bool getBoolean() const	throw(json_err_badcast){sure(json_BOOL);	return m_data.m_bool;	};
	double& getNumber()		throw(json_err_badcast);
	double getNumber() const throw(json_err_badcast);
	__int64& getInt64()		throw(json_err_badcast){sure(json_INT64);	return m_data.m_int64;	};
	__int64 getInt64() const throw(json_err_badcast){sure(json_INT64);	return m_data.m_int64;	};
	xd_JsonStringRef getString() throw(json_err_badcast){sure(json_STRING);	return m_data.m_string;};
	const char* getString() const throw(json_err_badcast){sure(json_STRING);	return m_data.m_string;};
	json_Arr& getArray()	throw(json_err_badcast){sure(json_ARRAY);	return m_data.m_array;	};
	json_Obj& getObject()	throw(json_err_badcast){sure(json_OBJECT);	return m_data.m_object;};

	void swap(CJsonElem & elem);
	void copy(const CJsonElem& elem);

	const CJsonElem& operator [](int n) const throw(json_err_badcast,json_err_overrange);
	CJsonElem& operator [](int n)	throw(json_err_badcast,json_err_overrange);

	const CJsonElem& operator [](const char* pstrKey) const throw(json_err_badcast);
	CJsonElem& operator [](const char* strkey)	throw(json_err_badcast);

	iterator_obj find(const char* strkey) throw(json_err_badcast);


	iterator_obj begin_obj() throw(json_err_badcast) {sure(json_OBJECT); return m_data.m_object.begin();};;
	iterator_obj end_obj() throw(json_err_badcast) {sure(json_OBJECT); return m_data.m_object.end();};;

	iterator_arr begin_arr() throw(json_err_badcast) {sure(json_ARRAY); return m_data.m_array.begin();};
	iterator_arr end_arr() throw(json_err_badcast)	{sure(json_ARRAY); return m_data.m_array.end();};

	const_iterator_obj begin_obj()	const throw(json_err_badcast) {sure(json_OBJECT); return m_data.m_object.begin();};
	const_iterator_obj end_obj()	const throw(json_err_badcast) {sure(json_OBJECT); return m_data.m_object.end();};

	const_iterator_arr begin_arr()	const throw(json_err_badcast) {sure(json_ARRAY); return m_data.m_array.begin();}
	const_iterator_arr end_arr()	const throw(json_err_badcast) {sure(json_ARRAY); return m_data.m_array.end();}

	
	void Insert(CJsonElem& vlu);						//swap语义
	void Insert(const CJsonElem& vlu);					//copy语义
	void Insert(const char* strkey, CJsonElem& vlu);	//swap语义
	void Insert(const char* strkey,const CJsonElem& vlu);//copy语义

	void Insert(xd_JsonStringRef strkey, CJsonElem& vlu);	//swap语义,key swap
	void Insert(xd_JsonStringRef strkey,const CJsonElem& vlu);//copy语义, key swap

private:
	void _sure(JsonType type) throw(json_err_badcast) {if (type != json_NULL) sure(type);};	//不判断null
};

struct jsonObjElem
{
	xd_JsonStringData m_sKey;
	CJsonElem	m_eJsonElem;
	jsonObjElem()
	{
		m_sKey.init();
	}
	jsonObjElem(const char * key)
	{
		m_sKey.init(key);
	}
	jsonObjElem(xd_JsonStringRef& key)//swap
	{
		m_sKey.init();
		xd_JsonStringRef _Tmp(m_sKey);
		_Tmp.swap(key);
	}
	jsonObjElem(const char * key, CJsonElem& jsonElem)
	{
		m_sKey.init(key);
		m_eJsonElem.swap(jsonElem);
	}
	jsonObjElem(xd_JsonStringRef& key, CJsonElem& jsonElem)//swap
	{
		m_sKey.init();
		xd_JsonStringRef _Tmp(m_sKey);
		_Tmp.swap(key);
		m_eJsonElem.swap(jsonElem);
	}
	jsonObjElem(const jsonObjElem& oe)// 没有右值引用的VC6使我很惆怅
	{
		m_sKey.init();
		copy(oe);
	}
	void swap(jsonObjElem& oe)
	{
		m_sKey.swap(oe.m_sKey); 
		m_eJsonElem.swap(oe.m_eJsonElem);
	}
	void copy(const jsonObjElem& oe)
	{
		m_sKey.copy(oe.m_sKey);
		m_eJsonElem.copy(oe.m_eJsonElem);
	}
	jsonObjElem& operator = (const jsonObjElem & oe) 
	{
		m_sKey.copy(oe.m_sKey); 
		m_eJsonElem.copy(oe.m_eJsonElem); 
		return *this;
	}
};
















}

