#include "StdAfx.h"

#include "WriteRead.h"
#include <math.h>

using namespace xd_Json;

#define MAX_INT64                      (0x7fffffffffffffff)

CjsonBuffer::CjsonBuffer(int n)
{
	m_size = 0;
	m_pBuf = (CHAR *)malloc(n* sizeof(CHAR));

	if (m_pBuf)
	{
		m_buffersize = n;
	}
}

CjsonBuffer::~CjsonBuffer()
{
	if (m_pBuf)
	{
		free(m_pBuf);
	}
}

bool CjsonBuffer::resize(int addlen)
{
	if (m_buffersize == 0 && addlen < 32)
	{
		addlen = 32;
	}
	else if (addlen < (m_buffersize>>1))
	{
		addlen = (m_buffersize>>1);//1.5倍,最少
	}
	int ReallocLen = m_buffersize + addlen;

	CHAR* _TmpBuff = (CHAR *)realloc(m_pBuf, ReallocLen + sizeof(CHAR));//最后一个放'\0'
	if (_TmpBuff)
	{
		m_pBuf = _TmpBuff;
		m_buffersize = ReallocLen;
		return true;
	}
	else
	{
		return false;
	}
}

bool CjsonBuffer::ensure_size(int nEnsureLen,bool bResize)
{
	if (m_buffersize - m_size > nEnsureLen)
	{
		return true;
	}
	else if (bResize)
	{
		return resize(nEnsureLen);
	}
	else
	{
		return false;
	}
}

CjsonBuffer::CHAR * CjsonBuffer::GetCString()
{
	*(m_pBuf + m_size) = CHAR(0);
	return m_pBuf;
}

CjsonBuffer::CHAR *& CjsonBuffer::GetRefCString()
{
	*(m_pBuf + m_size) = CHAR(0);
	return m_pBuf;
}

CjsonBuffer::CHAR * CjsonBuffer::append(CHAR c, int n)
{
	if (n <= 0)
	{
		return NULL;
	}
	if (ensure_size(n))
	{
		memset(end(), c, n);
		Step(n);
	}
	return end();
}

CjsonBuffer::CHAR * CjsonBuffer::append(const CHAR* pc, int nlen)
{
	if (nlen == -1)
	{
		nlen = strlen(pc);
	}

	if (ensure_size(nlen))
	{
		memcpy(end(), pc, nlen);
		Step(nlen);
	}
	return end();
}





#define ERROR_WR(b,w) do {if(!b) return setErr(w);} while (false);
#define WARNING_WR(b,w) do {if(!b) setErr(w);} while (false);

bool CjsonWR::setErr(const char * p, int npLen)
{
	if (!p)
	{
		return false;
	}
	int nLen = strlen(p);
	if (npLen <= 0 || nLen < npLen)
	{
		npLen = nLen;
	}

	if (m_ErrWord.ensure_size(npLen + 1))
	{
		m_ErrWord.append(p, npLen);
		m_ErrWord.append('\n');
	}
	return false;
}


const int CjsonWR::MAXNUM_LEN = 32;

CjsonWrite::CjsonWrite()
{
	strncpy(doubleFormat, "%.10lg", 8);
	strncpy(int64Format, "%I64d", 8);
}

void CjsonWrite::setDoubleFormat(const char* lpDoubleFormat)
{
	strncpy(doubleFormat, lpDoubleFormat, 8);
}

void CjsonWrite::setInt64Format(const char* lpInt64Format)
{
	strncpy(int64Format, lpInt64Format, 8);
}

bool CjsonWrite::write	(CJsonElem& jsonElem,CjsonBuffer& buffer,int nPetty)
{
	int nstepLen = 0;
	switch (jsonElem.GetType())
	{
	case json_NULL:
		{
			ERROR_WR(buffer.append("null"),"write: null;");
			return true;
		}

	case json_BOOL:
		{
			if (jsonElem.getBoolean())
			{
				ERROR_WR(buffer.append("true"),"write: true;");
			}
			else
			{
				ERROR_WR(buffer.append("false"),"write: false;");
			}
			return true;
		}
	case json_INT64:
		{
			ERROR_WR(buffer.ensure_size(MAXNUM_LEN), "write: int64;");
			nstepLen = _snprintf(buffer.end(), MAXNUM_LEN, int64Format, jsonElem.getInt64());
			buffer.Step(nstepLen);
			return true;
		}
	case json_NUMBER:
		{
			ERROR_WR(buffer.ensure_size(MAXNUM_LEN), "write: number;");
			nstepLen = _snprintf(buffer.end(),MAXNUM_LEN,doubleFormat,jsonElem.getNumber());
			buffer.Step(nstepLen);
			return true;
		}
	case json_STRING:
		ERROR_WR(writeString(jsonElem, buffer),"write: string;");
		return true;
	case json_ARRAY:
		ERROR_WR(writeArray(jsonElem, buffer, nPetty),"write: array;");
		return true;
	case json_OBJECT:
		ERROR_WR(writeObject(jsonElem, buffer, nPetty),"write: object;");
		return true;
	}
	return true;
}

bool CjsonWrite::writeString(xd_JsonStringRef str,CjsonBuffer& buffer)
{
	buffer.append('"');
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
	{
		switch (*it)
		{
		case '"':         buffer.append("\\\"");   break;
		case '\\':        buffer.append("\\\\");   break;
//		case '/':		  buffer.append("\\/");	   break;//反人类，搞不懂
		case '\b':        buffer.append("\\b");    break;
		case '\f':        buffer.append("\\f");    break;
		case '\n':        buffer.append("\\n");    break;
		case '\r':        buffer.append("\\r");    break;
		case '\t':        buffer.append("\\t");    break;
		default:          buffer.append(*it);      break;
		}
	}
	buffer.append('"');
	return true;
}

bool CjsonWrite::writeString(CJsonElem& jsonElem,CjsonBuffer& buffer)
{
	ERROR_WR(jsonElem.is(json_STRING),"writeString: type wrong;");
	return writeString(jsonElem.getString(), buffer);
}

bool CjsonWrite::writeArray	(CJsonElem& jsonElem,CjsonBuffer& buffer,int nPetty)
{
	ERROR_WR(jsonElem.is(json_ARRAY),"writeArray: type wrong;");

	buffer.append('[');
	if (nPetty != -1)
	{
		buffer.append('\n');
		++nPetty;
	}
	
	for (CJsonElem::iterator_arr it = jsonElem.begin_arr(); it != jsonElem.end_arr();)
	{
		buffer.append('\t', nPetty);
		ERROR_WR(write(*it, buffer, nPetty), "writeArray: write wrong;");

		++it;
		if (it != jsonElem.end_arr())
		{
			buffer.append(',');
			
		}
		buffer.append('\n');
	}

	if (nPetty != -1)
	{
		--nPetty;
	}
	buffer.append('\t', nPetty);
	buffer.append("]");
	return true;
}

bool CjsonWrite::writeObject(CJsonElem& jsonElem,CjsonBuffer& buffer,int nPetty)
{
	ERROR_WR(jsonElem.is(json_OBJECT),"writeObject: type wrong;");

	buffer.append('{');
	if (nPetty != -1)
	{
		buffer.append('\n');
		++nPetty;
	}
	
	for (CJsonElem::iterator_obj it = jsonElem.begin_obj(); it != jsonElem.end_obj();)
	{
		buffer.append('\t', nPetty);
		ERROR_WR(writeString(it->m_sKey,buffer),"writeObject: write name wrong;");
		buffer.append(" :\t",3);
		if (nPetty != -1 && (it->m_eJsonElem.is(json_OBJECT) || it->m_eJsonElem.is(json_ARRAY)) && it->m_eJsonElem.size() != 0)
		{
			buffer.append('\n');
			buffer.append('\t', nPetty);
		}
		ERROR_WR(write(it->m_eJsonElem, buffer, nPetty), "writeObject: write wrong;");
		
		++it;
		if (it != jsonElem.end_obj())
		{
			buffer.append(',');
			
		}
		buffer.append('\n');
	}
	
	if (nPetty != -1)
	{
		--nPetty;
	}
	buffer.append('\t', nPetty);
	buffer.append('}');
	return true;
}

bool CjsonWrite::write(CJsonElem& jsonElem,CjsonBuffer& buffer,bool prettify)
{
	return write(jsonElem, buffer, prettify ? 0 : -1);
}





#define UNEXCEPT_END(c) ERROR_WR(c!='\0',"Unexcept end;")

bool CjsonRead::read(CJsonElem& jsonElem,CjsonReadBuffer& buffer)
{
	EatSpace(buffer);
	UNEXCEPT_END(buffer.peek());

	switch(buffer.peek())
	{
	case 'n':
		{
			ERROR_WR(strncmp(buffer.end(),"null", 4)==0, "read : null;");
			buffer.move(4);
			jsonElem.changeto(json_NULL);
			return true;
		}
	case 't':
		{
			ERROR_WR(strncmp(buffer.end(),"true", 4)==0, "read : true;");
			buffer.move(4);
			jsonElem.changeto(json_BOOL);
			jsonElem = true;
			return true;
		}
	case 'f':
		{
			ERROR_WR(strncmp(buffer.end(),"false", 5)==0, "read : false;");
			buffer.move(5);
			jsonElem.changeto(json_BOOL);
			jsonElem = false;
			return true;
		}
	case '"':
		ERROR_WR(readString(jsonElem,buffer), "read : call readString;");
		return true;
	case '[':
		ERROR_WR(readArray(jsonElem,buffer), "read : call readArray;");
		return true;
	case '{':
		ERROR_WR(readObject(jsonElem,buffer), "read : call readObject;");
		return true;
	default:
		ERROR_WR(readNumber(jsonElem,buffer), "read : call readNumber;");
		return true;
	}
}

bool i64_pow(__int64& num, int nPow)
{
	__int64 _Tmpnum = num;
	if (nPow < 0)
	{
		while(nPow != 0)
		{
			if(_Tmpnum % 10 != 0)
			{
				return false;
			}
			_Tmpnum /= 10;
			++nPow;
		}
	}
	else
	{
		while(nPow != 0)
		{
			if(_Tmpnum > (MAX_INT64/10))
			{
				return false;
			}
			_Tmpnum *= 10;
			--nPow;
		}
	}
	num = _Tmpnum;
	return true;
	
};

bool CjsonRead::readNumber(CJsonElem& jsonElem,CjsonReadBuffer& buffer)
{
	ERROR_WR((isNumber(buffer.peek()) || buffer.peek() == '-'), "readNumber : start error;");

	bool blessthen0 = false;
	__int64 i64Num = 0;
	double dfNum = 0;
	bool bdouble = false;
	int nInt64ExpFlag = 0;
	bool bElessthen0 = false;

	if (buffer.peek() == '-')
	{
		blessthen0 = true;
		buffer.move();
	}

	if (buffer.peek() == '0')
	{
		buffer.move();
		if (isBreak(buffer.peek()))
		{
			jsonElem.changeto(json_INT64);
			jsonElem = 0;
			return true;
		}
		ERROR_WR((buffer.peek() == '.' || buffer.peek() == 'E' || buffer.peek() == 'e'), "readNumber : Numbers cannot have leading zeroes: 013;");
	}
	else
	{
		while(isNumber(buffer.peek()))
		{
			if (bdouble)
			{
				WARNING_WR((buffer.peek() == '0'),"WARNING: number is too accurate. maybe ;");
				++nInt64ExpFlag;
			}
			else if (i64Num < (MAX_INT64/10))
			{
				i64Num = i64Num * 10 + char2int(buffer.peek());
			}
			else
			{
				WARNING_WR((buffer.peek() == '0'),"WARNING: number is too accurate. maybe ;");
				++nInt64ExpFlag;
				bdouble = true;
			}
			buffer.move();
		}
	}
	
	if (buffer.peek() == '.')
	{
		buffer.move();
		ERROR_WR(isNumber(buffer.peek()),"readNumber : Unexcept word;");

		while(isNumber(buffer.peek()))
		{
			if (bdouble)
			{
				WARNING_WR((buffer.peek() == '0'),"WARNING: number is too accurate. maybe ;");
			}
			else if (i64Num < (MAX_INT64/10))
			{
				i64Num = i64Num * 10 + char2int(buffer.peek());
				--nInt64ExpFlag;
			}
			else
			{
				WARNING_WR((buffer.peek() == '0'),"WARNING: number is too accurate. maybe ;");
				bdouble = true;
			}
			buffer.move();
		}
	}

	if (buffer.peek() == 'E' || buffer.peek() == 'e')
	{
		buffer.move();
		if (buffer.peek() == '-' || buffer.peek() == '+')
		{
			bElessthen0 = buffer.peek() == '-';
			buffer.move();
		}

		ERROR_WR(isNumber(buffer.peek()),"readNumber : Except number here;");

		int eflag = 0;
		while(isNumber(buffer.peek()))
		{
			ERROR_WR(nInt64ExpFlag + eflag > 308,"readNumber : Number is too large;");
			eflag = eflag * 10 + char2int(buffer.peek());
			buffer.move();
		}
		nInt64ExpFlag = bElessthen0 ? nInt64ExpFlag - eflag : nInt64ExpFlag + eflag;
	}

	ERROR_WR(isBreak(buffer.peek()),"readNumber : Unexcept word;");
	
	if (!bdouble && !i64_pow(i64Num,nInt64ExpFlag))
	{
		bdouble = true;
	}
	if (bdouble)
	{
		dfNum = i64Num;
		dfNum *= pow(10, nInt64ExpFlag);
		jsonElem.changeto(json_NUMBER);
		if (blessthen0)
			dfNum = -dfNum;
		
		jsonElem = dfNum;
	}
	else
	{
		jsonElem.changeto(json_INT64);
		if (blessthen0)
			i64Num = -i64Num;
		jsonElem = i64Num;
	}
	return true;
}

bool CjsonRead::readString(xd_JsonStringRef str,CjsonReadBuffer& buffer)
{
	ERROR_WR((buffer.get()== '"'),"readString : start error;");

	const char * p = buffer.end(); 
	CjsonBuffer strbuffer(32);

	while(*p != '"' && *p != '\0')
	{
		ERROR_WR((!isESC(*p)),"readString : except '\"' for end;")
		if (*p == '\\')
		{
			++p;
			switch (*p) {
			case '\0':     UNEXCEPT_END(*p);
			case '"':      strbuffer.append('"');     break;
			case '\\':     strbuffer.append('\\');    break;
			case '/':      strbuffer.append('/');     break;
			case 'b':      strbuffer.append('\b');    break;
			case 'f':      strbuffer.append('\f');    break;
			case 'n':      strbuffer.append('\n');    break;
			case 'r':      strbuffer.append('\r');    break;
			case 't':      strbuffer.append('\t');    break;
			case 'u':      strbuffer.append("\\u",2); break;//不支持\u,只是保证不出错，但是改变了数据的内容
			default:	   ERROR_WR((false),"readString : Illegal backslash escape: \\x15;");
			}
		}
		else
		{
			strbuffer.append(*p) ;
		}
		++p;
	}
	ERROR_WR((*p == '"'),"readString : except '\"' for end;")
	buffer.moveTo(++p);
	str.swap(strbuffer.GetRefCString());

	return true;

}
bool CjsonRead::readString(CJsonElem& jsonElem,CjsonReadBuffer& buffer)
{
	jsonElem.changeto(json_STRING);
	ERROR_WR(readString(jsonElem.getString(),buffer),"");
	return true;
}

bool CjsonRead::readArray(CJsonElem& jsonElem,CjsonReadBuffer& buffer)
{
	ERROR_WR((buffer.get()== '['),"readArray : start error;");
	EatSpace(buffer);
	UNEXCEPT_END(buffer.peek());
	jsonElem.changeto(json_ARRAY);

	if (buffer.peek() == ']')
	{
		buffer.move();
	}
	else
	{
		do
		{
			CJsonElem _TempjsonElem;
			ERROR_WR(read(_TempjsonElem, buffer),"readArray : read elem Failed;");
			
			jsonElem.Insert(_TempjsonElem);
			
			EatSpace(buffer);
			UNEXCEPT_END(buffer.peek());
			
			ERROR_WR((buffer.peek() == ']' || buffer.peek() == ','), "readArray : Failed, except ',' or ']' ;");
		}
		while(buffer.get() != ']');
	}
	return true;
}

bool CjsonRead::readObject(CJsonElem& jsonElem,CjsonReadBuffer& buffer)
{
	ERROR_WR((buffer.peek()== '{'),"readObject : start error;");
	buffer.move();
	EatSpace(buffer);
	UNEXCEPT_END(buffer.peek());

	jsonElem.changeto(json_OBJECT);
	
	if (buffer.peek() == '}')
	{
		buffer.move();
	}
	else
	{
		do
		{
			EatSpace(buffer);
			UNEXCEPT_END(buffer.peek());

			xd_JsonStringData _TempstrKey;
			_TempstrKey.init();
			ERROR_WR(readString(_TempstrKey, buffer),"readObject : read string Key Failed;");

			EatSpace(buffer);
			UNEXCEPT_END(buffer.peek());

			ERROR_WR((buffer.peek() == ':'),"readObject : except ':' ;");
			buffer.move();

			CJsonElem _TempjsonElem;
			ERROR_WR(read(_TempjsonElem, buffer),"readObject : read elem Failed;");
			
			jsonElem.Insert(xd_JsonStringRef(_TempstrKey), _TempjsonElem);
			
			EatSpace(buffer);
			UNEXCEPT_END(buffer.peek());
			
			ERROR_WR((buffer.peek() == '}' || buffer.peek() == ','), "readObject : Failed, except ',' or '}' ;");
		}
		while(buffer.get() != '}');
	}
	return true;
}

bool CjsonRead::read(CJsonElem& jsonElem,const char* buffer)
{
	CjsonReadBuffer readbuff(buffer);
	if (!read(jsonElem, readbuff))
	{
		setErr(readbuff.end(), 32);
		return false;
	}
	EatSpace(readbuff);
	if (readbuff.peek() != '\0')
	{
		setErr("fail7");
		setErr(readbuff.end(), 32);
		return false;
	}
	return true;
}

void CjsonRead::EatSpace(CjsonReadBuffer& buff)
{
	while(isSpace(buff.peek()))
	{
		buff.move();
	}
}

