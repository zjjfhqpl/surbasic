//Author:zhaojunjiang 2015-4-13
#ifndef TREEENTITY_H
#define TREEENTITY_H
#include "surbasic/Public_Define.h"
#include "surbasic/Type.h"
#include "surbasic/timestamp.h"
#include <string>
using namespace SDBasic;
using namespace SDBasic::code;
using namespace SDBasic::time;
namespace SDBasic{
namespace tree{
enum ConvertState
{
	CAN_NOT_CONVERT = 1,
	CAN_CONVERT = 2,
	CONVERTING = 3,
	CONVERT_FAIL = 4,
	CONVERT_SUCCEED = 5,
	CONVERT_SUCCEED_SEP4 = 6,
};

class TreeEntity{
	enum FileDigest
	{
		None = 0,
		Fail,
		Success
	};
public:
	TreeEntity();
	~TreeEntity();
public:
	std::string id;
	std::string name;
	std::string extension;
	std::string parentid;
	int64_t     filesize;
	int         gifnum;
	Timestamp   createtime;
	Timestamp   modifytime;
	Timestamp   lastupdatetime;
	bool 	    isdoc;
	FolderType  nodetype;
	int 	    pagecount;
	std::string creator;
	//openapi
	int         del;
	ConvertState metaV;
	std::string md5digest;
	int 	    version;
	//Advanced Encryption
	std::string stroagekey;
	ShareType   sharetype;
	std::string sharerootid;
	
};
}}//namespace
#endif
