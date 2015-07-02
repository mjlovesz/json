#include "StdAfx.h"
#include "tools.h"

using namespace xd_Json;

#include <string>
#include "xd_json.h"
class test
{
	
	void testDeque()
	{
/*	xd_SimpleDeque<CJsonElem> g_nihao;*/
// 		CJsonElem nihao_NULL;
// 		g_nihao.Append(nihao_NULL);
// 		CJsonElem nihao_NUM(5.63);
// 		g_nihao.Insert(g_nihao.begin(),nihao_NUM);
// 		CJsonElem nihao_BOL(false);
// 		g_nihao.Insert(g_nihao.begin(),nihao_BOL);
// 		CJsonElem nihao_ARR(json_ARRAY);
// 		g_nihao.Insert(g_nihao.end(),nihao_ARR);
// 		CJsonElem nihao_STR("str");
// 		g_nihao.Insert(g_nihao.begin(),nihao_STR);
// 		g_nihao.Erase(g_nihao.begin());
// 		CJsonElem nihao_Obj(json_OBJECT);
// 		g_nihao.Insert(g_nihao.end(),nihao_Obj);
	};
	
	void testVector()
	{
		xd_JsonVector<CJsonElem> g_nihao;
		g_nihao.init();
		CJsonElem temp("nihao");
		CJsonElem temp1("nihao");
		CJsonElem temp2("nihao");
		CJsonElem temp3("nihao");
		CJsonElem temp4("nihao");
		CJsonElem temp5("nihao");
		CJsonElem temp6("nihao");
		CJsonElem temp7("nihao");
		CJsonElem temp8("nihao");
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
		g_nihao.push_back(temp);
	};
	
	void testString()
	{	
// 		char * nihao = (char *) malloc(5);
// 		strcpy(nihao, "jius");
// 		//		nihao[4] = '\0';
// 		xd_JsonString str;
// 		str.init(nihao);
// 		strcpy(nihao, "haha");
// 
// 		xd_JsonString strCopy;
// 		strCopy.init(nihao);
// 
// 		str.swap(strCopy);
// 		if (strCopy != str)
// 		{
// 			str = strCopy;
// 		}
// 		free(nihao);
	};
public:
	test()
	{
//		testVector();
//		testString();
	}
};
test g_test;

