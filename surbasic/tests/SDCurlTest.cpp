#include<stdio.h>
#include "surbasic/SDCurl.h"
#include "surbasic/AccessToken.h"
#include "surbasic/Public_Define.h"

using namespace SDBasic::CURL;

int main()
{
	Request::CURL_Init();
	std::string response;
	Request curl;
	curl.Init();
	std::string account = "mac1@cc.cc";
	SD_CODE ret = curl.Ent_Login("https://e.surdoc.net",account,"55af724c22b820c85ef9a6c073b437bf",15000,response);
	if(ret != SD_SUCCESSED){
	printf("Ent_login error");
	Request::CURL_UnInit();	
	return ret;}
		
	Request::CURL_UnInit();
    return 1;
}
