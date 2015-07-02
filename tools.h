#pragma once

#include "exception.h"
#include <CSTDLIB>
#include <UTILITY>
using namespace std::rel_ops;

namespace xd_Json
{
	template <typename T>
	struct block
	{
		size_t m_size;	//容量
		T m_data[1];	//数据
	};
	template <typename T>
	class xd_JsonVector
	{
	public://定义
		typedef T* iterator;
		typedef const T* const_iterator;
	protected://数据
		block<T> * m_pData;	//数据内存块
		size_t  m_size;		//size
	protected:
		void _malloc(int n = 0);
		void _ensureMem(int n);
		void _checkIt(iterator);
		void _checkRangeIt(iterator begIt, iterator endIt);
	public:
		void init(){m_pData = NULL; m_size = 0;};
		void init(int nsize){init(); _malloc(nsize);};
		void init(const xd_JsonVector& v){init(); copy(v);};

		void copy(const xd_JsonVector&);
		void swap(xd_JsonVector &);

		T& operator [](int i);
		const T& operator [](int i) const;

		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;

		void insert(iterator p,const T&);
		void insert_swap(iterator p, T&);
		void insert(iterator p, iterator, iterator);//拷贝 [beg-end) 
		void insert_swap(iterator p, iterator, iterator);

		iterator erase(iterator p);
		iterator erase(iterator, iterator);

		void push_back(const T&);
		void push_back_swap(T&);

		void clear();
		int size() const	{return m_size;}
		T& back()	{if (m_size == 0){ throw json_err_range();} return m_pData->m_data[m_size -1];}
		const T& back() const	{if (m_size == 0){ throw json_err_range();} return m_pData->m_data[m_size -1];}
	};

	template< typename T>
	void xd_JsonVector<T>::_malloc(int n)
	{
		if (n <= 0)
		{
			if (m_pData)
			{
				n = m_pData->m_size * 2;
			}
			else
			{
				n = 4;
			}
		}
		if (m_pData && n > m_pData->m_size)
		{
			block<T> * tmpMem = (block<T> *) realloc(m_pData, sizeof(block<T>) + (n - 1) * sizeof(T));
			if (tmpMem)
			{			
				m_pData = tmpMem;
				m_pData->m_size = n;
			}
		}
		if (m_pData == NULL)
		{
			m_pData = (block<T> *) malloc(sizeof(block<T>) + (n - 1) * sizeof(T));
			
			if (!m_pData)
			{
				throw json_err_memory();
			}
			m_pData->m_size = 0;
			_malloc(n);
		}
	}

	template< typename T>
	void xd_JsonVector<T>::_ensureMem(int n)
	{
		if (m_pData == NULL || (n + m_size) > m_pData->m_size)
		{
			_malloc();
		}
	}

	template< typename T>
	void xd_JsonVector<T>::clear()
	{
		if (m_pData)
		{
			while(m_size--)
			{
				m_pData->m_data[m_size].~T();
			}
			free(m_pData);
			m_pData = NULL;
		}
	};
	template< typename T>
	void xd_JsonVector<T>::copy(const xd_JsonVector& v)
	{
		clear();
		if (v.m_pData && v.m_size > 0)
		{
			_malloc(v.m_size);
			memset(m_pData->m_data, 0 ,v.m_size* sizeof(T));
			while(m_size != v.m_size)
			{
				m_pData->m_data[m_size].copy(v.m_pData->m_data[m_size]);
				++m_size;
			}
		}
	}
	template< typename T>
	void xd_JsonVector<T>::swap(xd_JsonVector & v)
	{
		std::swap(v.m_size, m_size);
		std::swap(v.m_pData, m_pData);
	}
		
	template< typename T>
	T& xd_JsonVector<T>::operator [](int i)
	{
		static T t;
		if (i < m_size && i >= 0)
		{
			return m_pData->m_data[i];
		}
		else
		{
			throw json_err_range();
			return t; 
		}
	}
	template< typename T>
	const T& xd_JsonVector<T>::operator [](int i) const
	{
		return this->operator [](i);
	}
	
	template< typename T>
	xd_JsonVector<T>::iterator xd_JsonVector<T>::begin()
	{
		if (m_pData)
		{
			return m_pData->m_data;
		}
		else
		{
			return NULL;
		}
	}
	template< typename T>
	xd_JsonVector<T>::iterator xd_JsonVector<T>::end()
	{
		if (m_pData)
		{
			return m_pData->m_data + m_size;
		}
		else
		{
			return NULL;
		}
	}
	template< typename T>
	xd_JsonVector<T>::const_iterator xd_JsonVector<T>::begin() const
	{
		if (m_pData)
		{
			return m_pData->m_data;
		}
		else
		{
			return NULL;
		}
	}
	template< typename T>
	xd_JsonVector<T>::const_iterator xd_JsonVector<T>::end() const
	{
		if (m_pData)
		{
			return m_pData->m_data + m_size;
		}
		else
		{
			return NULL;
		}
	}

	template< typename T>
	void xd_JsonVector<T>::_checkIt(xd_JsonVector::iterator pIt)
	{
		if (pIt < begin() || end() < pIt)
		{
			throw json_runtime("maybe this iterator is out-of-date, or it is an iterator of other object!");
		}
	}
	template< typename T>
	void xd_JsonVector<T>::_checkRangeIt(xd_JsonVector::iterator begIt, xd_JsonVector::iterator endIt)
	{
		if (endIt <= begIt)
		{
			throw json_runtime("end iterator smaller then begin iterator");
		}
	}
	template< typename T>
	void xd_JsonVector<T>::insert(xd_JsonVector::iterator pIt,const T& t)
	{
		_checkIt(pIt);
		int nIt = pIt - begin();
		_ensureMem(1);
		pIt = begin() + nIt;
		memmove(pIt + 1, pIt, end() - pIt);
		new (pIt) T(t);
		++m_size;
		int n = sizeof(T);
	}
	template< typename T>
	void xd_JsonVector<T>::insert_swap(xd_JsonVector::iterator pIt, T& t)
	{
		_checkIt(pIt);
		int nIt = pIt - begin();
		_ensureMem(1);
		pIt = begin() + nIt;
		if (pIt == NULL)
		{
			pIt = begin();
		}
		iterator itend = end();
		memmove(pIt + 1, pIt, end() - pIt);
		memset(pIt, 0, sizeof(T));
		pIt->swap(t);
		++m_size;
	}
	template< typename T>
	void xd_JsonVector<T>::insert(iterator pIt, iterator begIt, iterator endIt)
	{
		_checkIt(pIt);
		_checkRangeIt(begIt, endIt);
		int addsize = endIt - begIt;
		int nIt = pIt - begin();
		_ensureMem(addsize);
		pIt = begin() + nIt;
		if (pIt == NULL)
		{
			pIt = begin();
		}
		memmove(pIt + addsize, pIt, end() - pIt);
		for (iterator it = begIt ; it != endIt ; ++it, ++pIt)
		{
			new (pIt) T(*it);
		}
		m_size += addsize;
	}
	template< typename T>
	void xd_JsonVector<T>::insert_swap(iterator pIt, iterator begIt, iterator endIt)
	{
		_checkIt(pIt);
		_checkRangeIt(begIt, endIt);
		int addsize = endIt - begIt;
		int nIt = pIt - begin();
		_ensureMem(addsize);
		pIt = begin() + nIt;
		if (pIt == NULL)
		{
			pIt = begin();
		}
		memmove(pIt + addsize, pIt, (end() - pIt) * sizeof(T));
		memmove(pIt, begIt, addsize * sizeof(T));
		memset(begIt, 0, addsize * sizeof(T));
		m_size += addsize;
	}
	template< typename T>
	void xd_JsonVector<T>::push_back(const T& t)
	{
		insert(end(), t);
	}
	template< typename T>
	void xd_JsonVector<T>::push_back_swap(T& t)
	{
		insert_swap(end(), t);
	}
	template< typename T>
	xd_JsonVector<T>::iterator xd_JsonVector<T>::erase(iterator pIt)
	{
		_checkIt(pIt);
		if (m_pData == NULL || pIt == NULL)
		{
			throw json_runtime("erase wrong iterator");
		}
		pIt->~T();
		memmove(pIt, pIt + 1, end() - pIt);
		--m_size;
		return pIt;
	};
	template< typename T>
	xd_JsonVector<T>::iterator xd_JsonVector<T>::erase(iterator begIt,iterator endIt)
	{
		_checkIt(begIt);
		_checkIt(endIt);
		_checkRangeIt(begIt, endIt);
		int addsize = endIt - begIt;

		if (m_pData == NULL && begIt == NULL && endIt == NULL)
		{
			return begin();
		}
		if (m_pData == NULL || begIt == NULL || endIt == NULL)
		{
			throw json_runtime("erase wrong iterator");
		}
		for (iterator it = begIt; it != endIt; ++it)
		{
			it->~T();
		}
		memmove(begIt, begIt + addsize, end() - endIt);
		m_size -= addsize;
		return begIt;
	};



	template <typename T>
	void xd_swap(T& t1, T& t2)
	{
		T tem = t1;
		t1 = t2;
		t2 = tem;
	};

	template <typename T>
	class xd_SimpleVector : public xd_JsonVector<T>
	{
	public:
		xd_SimpleVector(int nsize):m_pData(NULL){_malloc(nsize)};
		xd_SimpleVector(const xd_SimpleVector& v):m_pData(NULL){copy(v);};
		~xd_SimpleVector(){clear();};
		
		xd_SimpleVector& operator =(const xd_SimpleVector& v){copy(v);return *this;};
	};


	struct xd_JsonStringData
	{
		size_t m_size;
		char*  m_pStr;
		
		void init()	/*只能构造时使用*/		{m_size = 0; m_pStr = NULL;};
		void init(const char* pcstrdata)	{init(); copy(pcstrdata);};
		void init(const xd_JsonStringData& strdata)	{init(); copy(strdata);};
		void clear()						{if (m_pStr) {free(m_pStr); m_pStr = NULL; m_size=0;}};
		
		void copy(const char* pstr)			{if (m_pStr == pstr) return; clear(); m_size= strlen(pstr); m_pStr = (char*)malloc(m_size + 1); memcpy(m_pStr, pstr, m_size + 1);};
		void copy(const char* pstr, size_t _size){if (m_pStr == pstr) return; clear(); m_size= _size; m_pStr = (char*)malloc(m_size + 1); memcpy(m_pStr, pstr, m_size + 1);};
		void copy(const xd_JsonStringData& str){if (this == &str) return; clear(); m_size= str.m_size; m_pStr = (char*)malloc(m_size + 1); memcpy(m_pStr, str.m_pStr, m_size + 1);};
		
		void swap(char*& pstr)				{m_size= strlen(pstr); xd_swap(m_pStr,pstr);};
		void swap(xd_JsonStringData& str)	{m_size= str.m_size; xd_swap(m_pStr,str.m_pStr);};
		operator const char*() const		{return m_pStr;}
		bool operator ==(const char* p) const {return strcmp(m_pStr, p) == 0;}
	};

	class xd_JsonStringRef
	{
	public:
		typedef char * iterator;
		typedef const char * const_iterator;
	private:
		xd_JsonStringData& m_Str;
	public:
		xd_JsonStringRef(xd_JsonStringData& Str):m_Str(Str)		{};
		xd_JsonStringRef(xd_JsonStringRef& str):m_Str(str.m_Str){};
		xd_JsonStringRef& operator =(const char* pstr)			{copy(pstr); return *this;}
		xd_JsonStringRef& operator =(const xd_JsonStringRef& str){copy(str); return *this;}

		void clear()						{m_Str.clear();};
		
		void copy(const char* pstr)			{m_Str.copy(pstr);};
		void copy(const xd_JsonStringRef& str){m_Str.copy(str.m_Str);};
		void swap(char*& pstr)				{m_Str.swap(pstr);};
		void swap(xd_JsonStringRef& str)	{m_Str.swap(str.m_Str);};

		iterator begin()				{return m_Str.m_pStr;};
		iterator end()					{return m_Str.m_pStr ? m_Str.m_pStr + m_Str.m_size : NULL;};
		const_iterator begin() const	{return m_Str.m_pStr;};
		const_iterator end() const		{return m_Str.m_pStr ? m_Str.m_pStr + m_Str.m_size : NULL;};

		size_t size(){return m_Str.m_size;}

		bool operator ==(const char* p) const {return strcmp(m_Str.m_pStr, p) == 0;}
		bool operator < (const char* p) const {return strcmp(m_Str.m_pStr, p) < 0;}
		bool operator ==(const xd_JsonStringRef& str) const {return m_Str.m_size == str.m_Str.m_size && strcmp(m_Str.m_pStr, str.m_Str.m_pStr) == 0;}
		bool operator < (const xd_JsonStringRef& str) const {return m_Str.m_size < str.m_Str.m_size || (m_Str.m_size == str.m_Str.m_size && strcmp(m_Str.m_pStr, str.m_Str.m_pStr) < 0);}
	
 		xd_JsonStringRef& operator +=(const char* p) {append(p); return *this;}
// 		const char* operator +(const char* p)	  {}
		operator const char*() const {return m_Str.m_pStr;}


		void append(const char* p)
		{
			int nSize = strlen(p);
			if (nSize == 0) 
				return ;
			char *pTmp = (char*)realloc(m_Str.m_pStr, m_Str.m_size + nSize);
			if (pTmp)
			{
				m_Str.m_pStr = pTmp;
				memcpy(end(), p, nSize + 1);
			}
		}
	};

}

