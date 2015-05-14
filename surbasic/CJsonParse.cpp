#include"CJsonParse.h"
#include "surbasic/Type.h"
#include "surbasic/StrUtils.hpp"

using namespace SDBasic::json;
using namespace SDBasic::fmt;
using namespace SDBasic;

int GetJsonInt(const Json::Value& _jsValue)
{
	if ( _jsValue.type() == Json::intValue)
		return _jsValue.asInt();
	else if (_jsValue.type() == Json::stringValue)
		return atoi(_jsValue.asCString());
	else if (_jsValue.isBool())
		return (int)_jsValue.asBool();
	return 0;
}
unsigned int GetJsonUInt(const Json::Value& _jsValue)
{
	return _jsValue.type() == Json::uintValue ? _jsValue.asUInt():0;
}
double GetJsonDouble(const Json::Value& _jsValue)
{
	return (_jsValue.type() == Json::realValue||
		_jsValue.type() == Json::intValue||
		_jsValue.type() == Json::uintValue) ? _jsValue.asDouble():0;
}
std::string GetJsonString(const Json::Value& _jsValue)
{
	return _jsValue.type() == Json::stringValue ? _jsValue.asString():"";
}
int64_t GetJsonlonglong(const Json::Value& _jsValue)
{
	return (_jsValue.type() == Json::realValue||
		_jsValue.type() == Json::intValue||
		_jsValue.type() == Json::uintValue) ? static_cast<int64_t>(_jsValue.asDouble()):0;
}
bool GetJsonBool(const Json::Value& _jsValue)
{
	return _jsValue.type() == Json::booleanValue ? _jsValue.asBool():false;
}


CJsonParse::CJsonParse(std::string& jsonstream):valid_(false)
{  if(CheckString(jsonstream)){
  Json::Reader jsReader;
  if(jsReader.parse(jsonstream,value_))valid_=true;}

	mapType.insert(map<string,fileType>::value_type(".SEP",SEP));
	mapType.insert(map<string,fileType>::value_type(".UNCONVERT",UNCONVERT));
	mapType.insert(map<string,fileType>::value_type(".DOC",DOC));
	mapType.insert(map<string,fileType>::value_type(".DOCX",DOCX));
	mapType.insert(map<string,fileType>::value_type(".XLS",XLS));
	mapType.insert(map<string,fileType>::value_type(".XLSX",XLSX));
	mapType.insert(map<string,fileType>::value_type(".PPT",PPT));
	mapType.insert(map<string,fileType>::value_type(".PPTX",PPTX));
	mapType.insert(map<string,fileType>::value_type(".TXT",TXT));
	mapType.insert(map<string,fileType>::value_type(".UNKNOW",UNKNOW));
	mapType.insert(map<string,fileType>::value_type(".JPG",JPG));
	mapType.insert(map<string,fileType>::value_type(".PNG",PNG));
	mapType.insert(map<string,fileType>::value_type(".GIF",GIF));
	mapType.insert(map<string,fileType>::value_type(".PDF",PDF));
	mapType.insert(map<string,fileType>::value_type(".TTF",TIF));
	mapType.insert(map<string,fileType>::value_type(".TIFF",TIFF));
	mapType.insert(map<string,fileType>::value_type(".PSD",PSD));
	mapType.insert(map<string,fileType>::value_type(".LOG",LOG));
	mapType.insert(map<string,fileType>::value_type(".MPEG",MPEG));
	mapType.insert(map<string,fileType>::value_type(".MP3",MP3));
	mapType.insert(map<string,fileType>::value_type(".MP4",MP4));
	mapType.insert(map<string,fileType>::value_type(".WMA",WMA));
	mapType.insert(map<string,fileType>::value_type(".WOV",WOV));
	mapType.insert(map<string,fileType>::value_type(".WMV",WMV));
	mapType.insert(map<string,fileType>::value_type(".WAV",WAV));
	mapType.insert(map<string,fileType>::value_type(".ZIP",ZIP));
	mapType.insert(map<string,fileType>::value_type(".RAR",RAR));
	mapType.insert(map<string,fileType>::value_type(".JAVA",JAVA));
	mapType.insert(map<string,fileType>::value_type(".HTML",HTML));
	mapType.insert(map<string,fileType>::value_type(".XML",XML));
}
bool CJsonParse::GetUserSpaceInfo(UserSpaceInfo &info)
{
		if(!valid_) return false;
		const Json::Value &val = value_;
		if (val.type() == Json::objectValue && !val.empty())
		{
			info.allSpace = GetJsonlonglong(val["allSpace"]);
			info.allNonFileSpace = GetJsonlonglong(val["allNonFileSpace"]);
			info.singleNonFileSize = GetJsonlonglong(val["singleNonFileSize"]);
			info.usedSpace = GetJsonlonglong(val["usedSpace"]);
			if (info.usedSpace < 0){
			printf("server return negative usedSpace %s", NumStr(info.usedSpace).c_str());
			info.usedSpace = 0;}
			info.creator = GetJsonString(val["creator"]);
			info.regTime = Timestamp::FromISO8601(GetJsonString(val["regTime"]));
			info.rootDirId = GetJsonString(val["rootDirId"]);
			info.userID = GetJsonlonglong(val["userID"]);
			info.userID36 = GetJsonString(val["userID36"]);
		}
		return true;
}


bool CJsonParse::CheckString(std::string & jsonstream){
  if(jsonstream.empty())return false;
  char cBegin=jsonstream[0];
  char cEnd = jsonstream[jsonstream.size()-1];
  switch(cBegin)
  {
    case'{':
	if(cEnd=='}')return true;
     break;
    case '[':
	if(cEnd==']');return true;
     break;
   }
   return false;
}

bool CJsonParse::Enterprise_GetAccessToken( AccessToken* token, SDContext* ctx )
{
//	printf("json ent\r\n");
	if(!valid_) return false;
	const Json::Value &val = value_;
	if (!val.isObject())
		return false;
	token->server = "https://" + GetJsonString(val["servername"]);
	token->webserver = "https://" + GetJsonString(val["webservername"]); 
	token->dbserver = "https://" + GetJsonString(val["dbservername"]); 
	token->svgserver = "https://" + GetJsonString(val["svgservername"]);
	token->privatekey = GetJsonString(val["privatekey"]); 
	token->admin_privkey = GetJsonString(val["adminprivatekey"]); 

	token->access_token = GetJsonString(val["access_token"]); 
	token->refresh_token = GetJsonString(val["refresh_token"]); 
	token->resource_server = GetJsonString(val["resource_server"]); 
	token->expires_in = GetJsonInt(val["expires_in"]); 

	if (ctx != NULL) {
		ctx->root_dir_id = GetJsonString(val["rootFolderId"]); 
		ctx->share_root_id = GetJsonString(val["shareFolderRootId"]); 
	}
	return true;	
}
bool CJsonParse::ParseNode(TreeEntity & node){
       if(!valid_) return false;
       if (!value_.isObject())return false;
       if (!ParseOpenApiDocJsonNode(value_, node))return false;
       return true;
}
std::string StrToupper(const std::string &_strValue)
{
	string strRet = "";
	for(int i=0;i<_strValue.size();i++)
	{
  	strRet = strRet + (char)toupper(_strValue.at(i));
	}
	return strRet;
}
CJsonParse::fileType CJsonParse::ParseFileType(const std::string &ext)
{
	std::string upperExt = StrToupper(ext);
	std::string utf8ext = upperExt;
	map<string,fileType>::iterator found = mapType.find(utf8ext);

	if (found == mapType.end())
		return UNKNOW;

	return found->second;
}
bool CJsonParse::ParseOpenApiDocJsonNode(const Json::Value &val, TreeEntity &entity)
{
	if (!val.isObject() || val.empty())
		return false;

	entity.isdoc = true;
	entity.id = GetJsonString(val["id"]);
	entity.name = GetJsonString(val["name"]);
	entity.extension = FileInfo::Extension(entity.name);
	if (!entity.extension.empty())
		entity.extension = "." + entity.extension;
	entity.parentid = GetJsonString(val["parent"]["id"]);
//	entity.m_strAllPathId = GetJsonString(val["allParentid"]);
//	entity.m_iLayer      = std::count(entity.m_strAllPathId.begin(), entity.m_strAllPathId.end(), ',')-1;
	entity.filesize = GetJsonlonglong(val["size"]);
	entity.del = GetJsonInt(val["delete"]);

	Timestamp modifyTime = Timestamp::FromISO8601(GetJsonString(val["modifiedTime"]));
	//entity.m_dTmodifyTime = modifyTime.toUnixTimeMs();
	entity.modifytime = modifyTime;

	entity.createtime = Timestamp::FromISO8601(GetJsonString(val["createTime"]));
	entity.pagecount = GetJsonInt(val["svgPageCount"]);
//	entity.m_istar = GetJsonInt(val["star"]);
	entity.creator = GetJsonString(val["creator"]);
	entity.nodetype = FTFile;
	if (entity.pagecount < 0) entity.pagecount = 0;

	entity.metaV = CAN_NOT_CONVERT;
	int metaV = GetJsonInt(val["metaV"]);
	if (metaV >= 1 && metaV <= 6)
		entity.metaV = (ConvertState)metaV;
	entity.gifnum = ParseFileType(entity.extension);
	entity.md5digest = GetJsonString(val["digest"]);
	entity.version = GetJsonInt(val["version"]);
	if (entity.parentid == CTX.recycle_bin_id) {
		entity.nodetype = FTRecycleFile;
	}
	return true;
}
