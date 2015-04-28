

#ifndef DATAOBJECTS_H
#define DATAOBJECTS_H
#include "surbasic/Type.h"
#include "surbasic/timestamp.h"
#include "surbasic/AccessToken.h"

//服务器返回的初始化设置
struct InitParametersFromServer
{
	int numUploadThread; // 上传线程数
	
	InitParametersFromServer()
		: numUploadThread(-1)
	{ }
};

//服务器返回的相同摘要的所有文件的信息
struct SameHashFileInfo
{
	std::string shareKey;
	std::string enc_hash;
	int64_t  size;
	int64_t enc_size;
};

struct  UserSpaceInfo
{
	int64_t allSpace;
	int64_t allNonFileSpace;
	int64_t singleNonFileSize;
	int64_t usedSpace;
	std::string creator;
  SDBasic::time::Timestamp regTime;
	std::string rootDirId;
	int64_t userID;
	std::string userID36;
	
	UserSpaceInfo()
		:allSpace(0), allNonFileSpace(0), singleNonFileSize(0), 
		usedSpace(0), regTime(SDBasic::time::Timestamp::Now()), userID(0)
	{ }
};

// 注册接口返回的信息
struct SignupUserInfo
{
	int64_t userID;
	std::string email;
	std::string password;
	std::string roleID;
	std::string rootDirID;
	std::string delDirID;
	std::string name;
	int age;
	int professional;
	std::string phone;
	std::string companyName;
	int learn;
	int language;
	std::string mobile;
	std::string msn;
	std::string address;
	std::string nationality;
	int state;
  SDBasic::time::Timestamp createTime;
	std::string backupDirID;
	std::string code;
	int third;
	std::string receivedID;
  SDBasic::time::Timestamp loginTime;
	std::string first_name;
	std::string family_name;
	std::string publicDirID;
	std::string syncDirID;
	int64_t allSize;
	int64_t pageSum;
	int64_t fileSum;
	int64_t allSizeMax;
	std::string servername;
	int64_t nonfilesize;
	std::string from;
	std::string realemail;
	std::string userID36;
};

struct SurLinkPerFileInfo {
	bool valid;
	std::string doc_id;
	std::string name;
	int64_t  size;
	std::string hex_storage_key_cipher;  // 十六进制编码的存储密钥密文
};

#endif
