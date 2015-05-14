//Author: zhaojunjiang 201-3-12
#ifndef CJSONPARSE_H
#define CJSONPARSE_H
#include <json/json.h>
#include "surbasic/Type.h"
#include "surbasic/FileInfo.h"
#include "surbasic/AccessToken.h"
#include "surbasic/nocopyable.hpp"
#include "surbasic/timestamp.h"
#include "surbasic/SDContext.h"
#include "surbasic/TreeEntity.h"
using namespace SDBasic::FileInfo;
using namespace SDBasic::tree;
using namespace SDBasic;
using namespace SDBasic::token;
using namespace SDBasic::context;
namespace SDBasic{
namespace json{

class CJsonParse : public SDBasic::nocopyable
{	
public:
	CJsonParse(std::string & jsonstream);
	bool CheckString(std::string & jsonstream);	
	bool IsValid(){return valid_;}
	enum fileType{
	X1, X2, X3, X4, X5, X6, X7, X8,
	SEP, UNCONVERT, DOC, DOCX, XLS,
	XLSX, PPT, PPTX, TXT, UNKNOW, JPG,
	PNG, GIF, PDF, TIF, TIFF, PSD, LOG,
	MPEG, MP3, MP4, WMA, WOV, WMV, WAV,
	ZIP, RAR, JAVA, HTML, XML, };	
	
	//open api

	bool GetUserSpaceInfo(UserSpaceInfo &info);
	bool GetOpenApiToken(AccessToken &token);
	bool GetUserSpaceUpdateTime(Timestamp &ts);		
	bool ParseNode(TreeEntity & node);		
	bool Enterprise_GetAccessToken(AccessToken* token, SDContext* ctx);
private:
	fileType ParseFileType(const std::string &ext);
	bool ParseOpenApiDocJsonNode(const Json::Value &val, TreeEntity &entity);
private:
	Json::Value value_;
	bool valid_;
	std::map<std::string,fileType> mapType;

};
}}//namespace
#endif
