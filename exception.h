#pragma once

#include "string.h"

namespace xd_Json
{
	class json_exception
	{
	public:
		virtual char* what(){return "";};
	};
	class json_runtime : public json_exception
	{
		char m_err[128];
	public:
		json_runtime(char* pErr)
		{
			strncpy(pErr, m_err, 128);
		}
		virtual char* what(){return m_err;};
	};

	class json_err_range: public json_exception
	{
	public:
		virtual char* what(){return "error range";};
	};

	class json_err_overrange : public json_exception
	{
	public:
		virtual char* what(){return "over range";};
	};

	class json_err_memory : public json_exception
	{
	public:
		virtual char* what(){return "memory";};
	};

	class json_err_badcast : public json_exception
	{
	public:
		virtual char* what(){return "bad cast";};
	};

}

