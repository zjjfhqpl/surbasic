//Author:zhaojunjiang 2015-3-12

#ifndef ACCESSTOKEN_H
#define ACCESSTOKEN_H
#include <string>
//#include <curl/curl.h>


//struct curl_httppost;
namespace SDBasic{
namespace token{
enum UserType
{
	Normal = 0,
	THRD_51 = 1,
	THRD_zhubajie = 2,
	THRD_google = 3,
	THRD_yahoo = 4,
	THRD_hotmail = 5,
	THRD_facebook = 6,
};

enum UserAPIType {
	USER_API_OPENAPI = 0,  //use openapi interface
	USER_API_ENC = 1,      //use enc interface
};


//just to save user login account、pwd,then can be extended to OAUTH
//just all user login infomation interface should be passed to this class,
//such as before with PK and without PK interface can not disinguish between.

struct AccessToken
{
	std::string account;
	UserType utype;

///	 token for old api
	std::string server;
	std::string webserver;
	std::string dbserver;
	std::string svgserver;
	std::string md5pwd;
	std::string privatekey;

	// open api token
	std::string access_token;
	long long    expires_in;
	std::string refresh_token;
	std::string resource_server;
	//企业盘数据
	std::string admin_privkey;    //下载部门共享文件时需要
	std::string user_name;     
	std::string user_email;    
	int 	    user_space;       //空间大小
	std::wstring user_department;  //部门
	std::string user_role;        //角色	


	AccessToken(): utype(Normal) {}
	void clear();
};

inline void AccessToken::clear()
{
	utype = Normal;
	server = "";
	webserver = "";
	dbserver = "";
	svgserver = "";
	account = "";
	md5pwd = "";
	privatekey = "";
	access_token = "";
	expires_in = 0;
	refresh_token = "";
	resource_server = "";
}

}}//namespace

#endif
