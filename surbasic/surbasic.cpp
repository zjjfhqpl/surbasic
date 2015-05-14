#include "surbasic/surbasic.h"
#include "surbasic/Digestor.h"
#include "surbasic/Public_Define.h"
#include "surbasic/EciesHelper.h"
#include "surbasic/FileUploaderFactory.h"
#include "surbasic/FileDownloaderFactory.h"
#include "surbasic/XmlHelper.h"
using namespace SDBasic;
using namespace SDBasic::ecc;
using namespace SDBasic::code;
using namespace SDBasic::digestor;
namespace SDBasic{
void Init(){
	Request::CURL_Init();
}
void UnInit(){
	Request::CURL_UnInit();
}
bool GetUserSpaceInfo(const uint64_t & key,UserSpaceInfo & info){
	std::string response;
	Request curl;
	curl.Init();
	AccessToken token;
	TokenManager::get().GetToken(key,token);
	SD_CODE ret = curl.OpenAPI_GetUserSpaceInfo(token,response);
	if(ret!=SD_SUCCESSED){
	return false;}
	CJsonParse json(response);
	if (json.GetUserSpaceInfo(info) == false)
		return false;
	return true;
}
uint64_t  Login(const std::string & server,const std::string &user,const std::string &plainpwd,AccessToken &token){
        std::string response;
        Request curl;
        curl.Init();
	    std::string md5pwd = Digestor::HashPassword(plainpwd);
        printf("plainpwd:%s,md5pwd:%s\r\n",plainpwd.c_str(),md5pwd.c_str());
		SD_CODE ret = curl.Personal_Login(server,user,md5pwd,15000,response);
		if(ret != SD_SUCCESSED){
        printf("\r\nEnt_login error\r\n");
        return 0;}
        else{
        printf("\r\nEnt_login successed.\r\n");}

        AccessToken temp_token;
        SDContext *ctx = &CTX;
		if (xml::ParseCheckAccountXml(response.c_str(),temp_token) == false)
      return 0;

        printf("privatekey:%s\r\n",temp_token.privatekey.c_str());
        printf("rootid:%s\r\n",ctx->root_dir_id.c_str());
		printf("TokenManage privatekey:%s\r\n",temp_token.privatekey.c_str());
        ByteArray pk;
        int iret = EciesHelper::TryParsePrivKey(temp_token.privatekey,plainpwd,&pk);
        if(iret == 1)
        {
           token.privatekey = pk.toUrlBase64Str();
           printf("token.privatekey:%s\r\n",token.privatekey.c_str());
        }else{
                printf("failed to parse user`s private key\r\n");
                return 0;
        }
		token.server = temp_token.server;
        token.dbserver = temp_token.dbserver;
        token.svgserver = temp_token.svgserver;
        token.webserver = temp_token.webserver;

        token.user_department = temp_token.user_department;
        token.user_email = temp_token.user_email;
        token.user_name = temp_token.user_name;
        token.user_role = temp_token.user_role;
        token.user_space = temp_token.user_space;

        if (!temp_token.access_token.empty())
        {
                token.access_token = temp_token.access_token;
                token.refresh_token = temp_token.refresh_token;
                token.resource_server = temp_token.resource_server;
                token.expires_in = temp_token.expires_in;
        }

      return  TokenManager::get().SetToken(token);
}

uint64_t  LoginEnt(const std::string & server,const std::string &user,const std::string &plainpwd,AccessToken &token){
        std::string response;
        Request curl;
        curl.Init();
	    std::string md5pwd = Digestor::HashPassword(plainpwd);
        printf("plainpwd:%s,md5pwd:%s\r\n",plainpwd.c_str(),md5pwd.c_str());
        //SD_CODE ret = curl.Ent_Login("https://e.surdoc.net",user,md5pwd,15000,response);
        SD_CODE ret = curl.Ent_Login(server,user,md5pwd,15000,response);
		if(ret != SD_SUCCESSED){
        printf("\r\nEnt_login error\r\n");
        return 0;}
        else{
        printf("\r\nEnt_login successed.\r\n");}

        AccessToken temp_token;
        SDContext *ctx = &CTX;
        CJsonParse parse(response);
        if(!parse.IsValid()|| !parse.Enterprise_GetAccessToken(&temp_token, ctx))
            printf("failed to parse login response.\r\n");
        printf("privatekey:%s\r\n",temp_token.privatekey.c_str());
        printf("rootid:%s\r\n",ctx->root_dir_id.c_str());
        printf("TokenManage privatekey:%s\r\n",temp_token.privatekey.c_str());
        ByteArray pk;
        int iret = EciesHelper::TryParsePrivKey(temp_token.privatekey,plainpwd,&pk);
        if(iret == 1)
        {
           token.privatekey = pk.toUrlBase64Str();
           printf("token.privatekey:%s\r\n",token.privatekey.c_str());
        }else{
                printf("failed to parse user`s private key\r\n");
                return 0;
        }
        token.server = temp_token.server;
        token.dbserver = temp_token.dbserver;
        token.svgserver = temp_token.svgserver;
        token.webserver = temp_token.webserver;

        token.user_department = temp_token.user_department;
        token.user_email = temp_token.user_email;
        token.user_name = temp_token.user_name;
        token.user_role = temp_token.user_role;
        token.user_space = temp_token.user_space;

        if (!temp_token.access_token.empty())
        {
                token.access_token = temp_token.access_token;
                token.refresh_token = temp_token.refresh_token;
                token.resource_server = temp_token.resource_server;
                token.expires_in = temp_token.expires_in;
        }

       return TokenManager::get().SetToken(token);
}
AccessToken GetToken(const uint64_t & key){
	AccessToken token;
	TokenManager::get().GetToken(key,token);
return token;
}

bool Upload(const uint64_t & key,
            const std::string & filepath,
            const std::string & parentid,
            const std::string& name,
            Timestamp modifytime,
            TreeEntity & node)
{
  FileUploaderEnc* uploader = FileUploaderFactory::get().CreateUploader(key);
  SD_CODE ret = uploader->UploadFileByParentId(
      key, filepath, parentid, name, modifytime, SHARE_NONE, node);

  delete uploader;
  return (ret = SD_SUCCESSED);
}

bool Download(const uint64_t & key,const std::string & id,const std::string & path){
  FileDownloaderEnc* d = FileDownloaderFactory::get().CreateDownloader(key);
  SD_CODE ret = d->Download(key,id,path);
  delete d;
  return (ret==SD_SUCCESSED?true:false);
}

bool RenameFile(uint64_t key, const std::string& id, const std::string& new_name,
                TreeEntity& node)
{
  std::string response;
  Request curl;
  curl.Init();
  AccessToken token;
  TokenManager::get().GetToken(key,token);
  SD_CODE ret = curl.OpenAPI_RenameFile(token, id, new_name, response);
  if(ret!=SD_SUCCESSED) {
    return false;
  }

  CJsonParse json(response);
  if (json.ParseNode(node) == false)
    return false;
  return true;
}

bool DeleteFile(uint64_t key, const std::string& id, TreeEntity& node)
{
  std::string response;
  Request curl;
  curl.Init();
  AccessToken token;
  TokenManager::get().GetToken(key,token);
  SD_CODE ret = curl.OpenAPI_DeleteFile(token, id, response);

  return (ret == SD_SUCCESSED);
}

bool MoveFile(uint64_t key, const std::string& id, const std::string& new_pid,
              TreeEntity& node)
{
  std::string response;
  Request curl;
  curl.Init();
  AccessToken token;
  TokenManager::get().GetToken(key,token);
  SD_CODE ret = curl.OpenAPI_MoveFile(token, id, new_pid, response, OVERWRITE);
  if(ret!=SD_SUCCESSED) {
    return false;
  }

  CJsonParse json(response);
  if (json.ParseNode(node) == false)
    return false;
  return true;
}

bool CreateFolder(uint64_t key, 
                  const std::string& pid, 
                  const std::string& name,
                  TreeEntity& node)
{
  std::string response;
  Request curl;
  curl.Init();
  AccessToken token;
  TokenManager::get().GetToken(key,token);
  SD_CODE ret = curl.OpenAPI_CreateFolder(token, pid, name, response);
  if(ret!=SD_SUCCESSED) {
    return false;
  }

  CJsonParse json(response);
  if (json.ParseNode(node) == false)
    return false;
  return true;
}

bool DeleteFolder(uint64_t key,
                  const std::string& id,
                  bool recursive)
{
  std::string response;
  Request curl;
  curl.Init();
  AccessToken token;
  TokenManager::get().GetToken(key,token);
  SD_CODE ret = curl.OpenAPI_DeleteFolder(token, id, recursive, response);

  return (ret == SD_SUCCESSED);
}

bool RenameFolder(uint64_t key, const std::string& id, const std::string& new_name,
                TreeEntity& node)
{
  std::string response;
  Request curl;
  curl.Init();
  AccessToken token;
  TokenManager::get().GetToken(key,token);
  SD_CODE ret = curl.OpenAPI_RenameFolder(token, id, new_name, response);
  if(ret!=SD_SUCCESSED) {
    return false;
  }

  CJsonParse json(response);
  if (json.ParseNode(node) == false)
    return false;
  return true;
}

}//namespace
