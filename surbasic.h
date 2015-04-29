//Author:zhaojunjiang 2015-4-15
#ifndef LOGIN_H
#define LOGIN_H
#include "surbasic/SDCurl.h"
#include "surbasic/Type.h"
#include "surbasic/TreeEntity.h"
#include "surbasic/AccessToken.h"
#include "surbasic/TokenManager.h"
#include "surbasic/Public_Define.h"

namespace SDBasic{

using namespace SDBasic::tokenmg;
using namespace SDBasic::token;
using namespace SDBasic::curl;
using namespace SDBasic::tree;
using namespace SDBasic::code;

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
bool Upload(const uint64_t & key,
            const std::string & filepath,
            const std::string & parentid,
            const std::string& name,
            Timestamp modifytime,
            TreeEntity & node); 
//id:doc id ,path:file out path
bool Download(const uint64_t & key,const std::string & id,const std::string & path);

bool RenameFile(uint64_t key, const std::string& id, const std::string& new_name,
                TreeEntity& node);
bool DeleteFile(uint64_t key, const std::string& id, TreeEntity& node);
bool MoveFile(uint64_t key, const std::string& id, const std::string& new_pid,
              TreeEntity& node);

bool CreateFolder(uint64_t key, 
                  const std::string& pid, 
                  const std::string& name,
                  TreeEntity& node);
bool DeleteFolder(uint64_t key, const std::string& id, bool recursive);

bool RenameFolder(uint64_t key, const std::string& id, const std::string& new_name,
                TreeEntity& node);

}//namespace
#endif
