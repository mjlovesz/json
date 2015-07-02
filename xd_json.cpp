//#include "StdAfx.h"
#include "xd_json.h"
using namespace xd_Json;

CJsonElem::CJsonElem(JsonType type):m_type(type)		
{
	m_data.m_number = 0;
	switch (m_type)
	{
	case json_STRING:
		m_data.m_string.init();
		break;
	case json_ARRAY:
		m_data.m_array.init();
		break;
	case json_OBJECT:
		m_data.m_object.init();
		break;
	}
};

CJsonElem::~CJsonElem()
{
	switch (m_type)
	{
	case json_STRING:
		m_data.m_string.clear();
		break;
	case json_ARRAY:
		m_data.m_array.clear();
		break;
	case json_OBJECT:
		m_data.m_object.clear();
		break;
	}
}

void CJsonElem::swap(CJsonElem & elem)
{
	std::swap(elem.m_type, m_type);
	std::swap(elem.m_data, m_data);
}


void CJsonElem::copy(const CJsonElem& elem)
{
	m_type = elem.m_type;
	m_data = elem.m_data;

	switch (m_type)
	{
	case json_STRING:
		m_data.m_string.init(elem.m_data.m_string);
		break;
	case json_ARRAY:
		m_data.m_array.init(elem.m_data.m_array);
		break;
	case json_OBJECT:
		m_data.m_object.init(elem.m_data.m_object);
		break;
	}
};

bool CJsonElem::changeto(JsonType type)
{
	if (!is(type)) 
	{
		CJsonElem _Tem(type);
		swap(_Tem);
		return true;
	} 
	else 
	{
		return false;
	}
}

void CJsonElem::clear()
{
	if (is(json_OBJECT))
	{
		m_data.m_object.clear();
		return ;
	}
	else if (is(json_ARRAY))
	{
		m_data.m_array.clear();
		return ;
	}
	throw json_err_badcast();
}

int CJsonElem::size() 
{
	if (is(json_OBJECT))
	{
		return m_data.m_object.size();
	}
	else if (is(json_ARRAY))
	{
		return m_data.m_array.size();
	}
	throw json_err_badcast();
}

const double CJsonElem::getNumber() const
{
	sure(json_NUMBER); 
	if (is(json_INT64))
	{ 
		return (double)m_data.m_int64;
	} 
	return m_data.m_number;
};

double& CJsonElem::getNumber()
{
	sure(json_NUMBER); 
	if (is(json_INT64))
	{
		m_type = json_NUMBER; 
		m_data.m_number = m_data.m_int64;
	} 
	return m_data.m_number;
};


const CJsonElem& CJsonElem::operator [](const char* strKey) const	
{
	static CJsonElem s_null;	
	sure(json_OBJECT);
/*
	json_Obj::iterator it = m_data.m_object->find(strKey);
	if (it != m_data.m_object->end())
	{
		return it->second;
	}
	else
	{
		return s_null;
	}
/*/
	for (const_iterator_obj it = m_data.m_object.begin(); it != m_data.m_object.end(); ++it)
	{
		if (it->m_sKey == strKey)
		{
			return it->m_eJsonElem;
		}
	}
	return s_null;
//*/
};

CJsonElem& CJsonElem::operator [](const char* strkey)
{
	_sure(json_OBJECT);	
	changeto(json_OBJECT);	
/*
	return m_data.m_object->operator [](strkey);
/*/
	for (json_Obj::iterator it = m_data.m_object.begin(); it != m_data.m_object.end(); ++it)
	{
		if (it->m_sKey == strkey)
		{
			return it->m_eJsonElem;
		}
	}
	jsonObjElem m(strkey);
	m_data.m_object.push_back(m);
	return m_data.m_object.back().m_eJsonElem;
//*/
};

const CJsonElem& CJsonElem::operator [](int n) const	
{
	static CJsonElem s_null;	
	sure(json_ARRAY); 
	if (n >= 0 && n < m_data.m_array.size())
	{
		return m_data.m_array[n];
	}
	throw json_err_overrange();
	return s_null;
};
CJsonElem& CJsonElem::operator [](int n)
{
	static CJsonElem s_null;	
	sure(json_ARRAY); 
	if (n >= 0 && n < m_data.m_array.size())
	{
		return m_data.m_array[n];
	}
	throw json_err_overrange();
	return s_null;
};

CJsonElem::iterator_obj CJsonElem::find(const char* strKey) 
{
	sure(json_OBJECT); 
	json_Obj::iterator it = m_data.m_object.begin();
	for (; it != m_data.m_object.end(); ++it)
	{
		if (it->m_sKey == strKey)
		{
			return it;
		}
	}
	return it;
};


void CJsonElem::Insert(CJsonElem& vlu)
{
	_sure(json_ARRAY);
	changeto(json_ARRAY);
	m_data.m_array.push_back_swap(vlu);
}
void CJsonElem::Insert(const char* key, CJsonElem& vlu)
{
	_sure(json_OBJECT);
	changeto(json_OBJECT);
	m_data.m_object.push_back(jsonObjElem(key));
	m_data.m_object.back().m_eJsonElem.swap(vlu);
// 	CJsonElem & v = (*m_data.m_object)[key];
// 	v.swap(vlu);
}
void CJsonElem::Insert(const CJsonElem& vlu)
{
	_sure(json_ARRAY);
	changeto(json_ARRAY);
	m_data.m_array.push_back(CJsonElem());
	m_data.m_array.back().copy(vlu);
}
void CJsonElem::Insert(const char* key,const CJsonElem& vlu)
{
	_sure(json_OBJECT);
	changeto(json_OBJECT);
	m_data.m_object.push_back(jsonObjElem(key));
	m_data.m_object.back().m_eJsonElem.copy(vlu);
// 	CJsonElem & v = (*m_data.m_object)[key];
// 	v.copy(vlu);
}

void CJsonElem::Insert(xd_JsonStringRef strkey, CJsonElem& vlu)
{
	_sure(json_OBJECT);
	changeto(json_OBJECT);
	jsonObjElem _Temp(strkey, vlu);
	m_data.m_object.push_back_swap(_Temp);
//	m_data.m_object.back().m_eJsonElem.swap(vlu);
}

void CJsonElem::Insert(xd_JsonStringRef strkey,const CJsonElem& vlu)
{
	_sure(json_OBJECT);
	changeto(json_OBJECT);
	m_data.m_object.push_back(jsonObjElem(strkey));
	m_data.m_object.back().m_eJsonElem.copy(vlu);
}


