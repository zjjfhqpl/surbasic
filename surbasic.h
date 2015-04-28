//Author:zhaojunjiang 2015-4-15
#ifndef LOGIN_H
#define LOGIN_H
#include "surbasic/SDCurl.h"
#include "surbasic/Type.h"
#include "surbasic/TreeEntity.h"
#include "surbasic/AccessToken.h"
#include "surbasic/TokenManager.h"

using namespace SDBasic::tokenmg;
using namespace SDBasic::token;
using namespace SDBasic::curl;
using namespace SDBasic::tree;
namespace SDBasic{

void Init();
void UnInit();
//user:user name,pwd:password,token:out
uint64_t LoginEnt(const std::string & server,const std::string & user,const std::string & plainpwd,AccessToken & token);
//
bool GetUserSpaceInfo(const uint64_t & key,UserSpaceInfo & info);
uint64_t Login(const std::string & server,const std::string & user,const std::string & plainpwd,AccessToken & token);
//return token
AccessToken GetToken(const uint64_t &key);
//parentid:parent id ,filepath: upload file path,nod:out 
bool Upload(const uint64_t & key,const std::string & parentid,const std::string & filepath,TreeEntity & node); 
//id:doc id ,path:file out path
bool Download(const uint64_t & key,const std::string & id,const std::string & path);
}//namespace
#endif
