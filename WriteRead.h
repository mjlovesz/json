#pragma once

#include "xd_json.h"

namespace xd_Json
{

class CjsonBuffer
{
	typedef char CHAR;
	int m_size;
	int m_buffersize;	
	CHAR * m_pBuf;
private:
	bool resize(int addLen);//sizeof(CHAR)的大小
public:
	CjsonBuffer():m_pBuf(NULL),m_size(0),m_buffersize(0){};
	CjsonBuffer(int);
	~CjsonBuffer();
	int size()			{return m_size;};
	int max_size()		{return m_buffersize;};
	int remain_size()	{return m_buffersize - m_size;};
	void empty()		{m_size = 0;};
	int Step(int n)		{if (ensure_size(n, false)) m_size += n; return m_size;}

	CHAR * begin()		{return m_pBuf;};
	CHAR * end()		{return m_pBuf + m_size;};
	CHAR * GetCString();
	CHAR *& GetRefCString();

	bool ensure_size(int nEnsureLen,bool bResize= true);

	CHAR * append(CHAR c, int n = 1);
	CHAR * append(const CHAR* pc, int nlen = -1);
};

struct CjsonReadBuffer
{
public:
	typedef char CHAR;
private:
	const CHAR * m_pBuf;
public:
	const CHAR * m_p;
	CjsonReadBuffer(const CHAR * p):m_pBuf(p),m_p(p){};
	
	inline const CHAR * begin()	{return m_pBuf;};
	inline const CHAR * end()		{return m_p;};
	inline CHAR peek()		{return *m_p;};
	inline void move()		{++m_p;};
	inline void move(int n)	{m_p += n;};
	inline void moveTo(const CHAR * p)	{m_p = p;};
	inline void back()		{--m_p;};

	inline CHAR get()		{return *m_p++;};
};

class CjsonWR
{
protected:
	CjsonWR():m_ErrWord(64){};
	const static int MAXNUM_LEN;
	CjsonBuffer m_ErrWord;
	bool setErr(const char * p, int len = 0);
public:
	const char* GetLastError(){return m_ErrWord.GetCString();};
};

class CjsonWrite : public CjsonWR
{
	char doubleFormat[8];
	char int64Format[8];
private:
	bool write		(CJsonElem& jsonElem, CjsonBuffer& buffer, int nPetty);
	bool writeString(xd_JsonStringRef str, CjsonBuffer& buffer);
	bool writeString(CJsonElem& jsonElem, CjsonBuffer& buffer);
	bool writeArray	(CJsonElem& jsonElem, CjsonBuffer& buffer, int nPetty);
	bool writeObject(CJsonElem& jsonElem, CjsonBuffer& buffer, int nPetty);
public:
	CjsonWrite();
	void setDoubleFormat(const char* lpDoubleFormat);
	void setInt64Format(const char* lpInt64Format);

	
	bool write(CJsonElem& jsonElem,CjsonBuffer& buffer,bool bPetty);
//	write(const CJsonElem& jsonElem,CjsonBuffer& buffer);//有一个const 没有
};

class CjsonRead : public CjsonWR
{
	//0-9
	inline bool isNumber(char c){return (c >= '0' && c <= '9');}
	//0-9 a-z A-Z
	inline bool isWord(char c){return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');}
	//空格，换行等
	inline bool isSpace(char c){return c == '\n'  || c == '\t' || c == '\r' || c == ' ' || c == '\f' ;}
	//json中的终止单词的符号:	',' '}' ':' ']' 空格换行
	inline bool isBreak(char c){return isSpace(c) || c == '}' || c == ']' || c == ',' || c == ':' || c == '\0';}	
	//json中需要转义字符
	inline bool isESC(char c){return c == '\b' || c == '\f' || c == '\n' || c == '\r' || c == '\t';}

	inline void EatSpace(CjsonReadBuffer& buff);
	inline int	char2int(char c){return c - '0';};


	bool readNumber(CJsonElem& jsonElem,CjsonReadBuffer& buffer);
	bool readString(xd_JsonStringRef str,CjsonReadBuffer& buffer);
	bool readString(CJsonElem& jsonElem,CjsonReadBuffer& buffer);
	bool readObject(CJsonElem& jsonElem,CjsonReadBuffer& buffer);
	bool readArray(CJsonElem& jsonElem,CjsonReadBuffer& buffer);
	bool read(CJsonElem& jsonElem,CjsonReadBuffer& buffer);
public:
	bool read(CJsonElem& jsonElem,const char* buffer);
};














}

