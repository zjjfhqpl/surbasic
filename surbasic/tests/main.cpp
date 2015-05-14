#include "surbasic/Type.h"
#include "surbasic/SDCurl.h"
#include "surbasic/AccessToken.h"
#include "surbasic/Public_Define.h"
#include "surbasic/CJsonParse.h"
#include "surbasic/SDContext.h"
#include "surbasic/ByteArray.h"
#include "surbasic/Digestor.h"
#include "surbasic/EciesHelper.h"
#include "surbasic/TokenManager.h"
#include "surbasic/FileInfo.h"
#include "surbasic/FileUploaderEnc.h"
#include "surbasic/TreeEntity.h"
#include "surbasic/FileUploaderFactory.h"
#include "surbasic/FileDownloaderEnc.h"
#include "surbasic/FileDownloaderFactory.h"
#include "surbasic/surbasic.h"
#include "surbasic/DateObjects.h"

using namespace SDBasic;
using namespace SDBasic::FileInfo;
using namespace SDBasic::download;
using namespace SDBasic::tree;
using namespace SDBasic::upload;
using namespace SDBasic::tokenmg;
using namespace SDBasic::context;
using namespace SDBasic::curl;
using namespace SDBasic::json;
using namespace SDBasic::digestor;
using namespace SDBasic::bytearray;
using namespace SDBasic::ecc;
const char *help_msg = 
"command list:  \n"
"  a           a <file> upload file \n"
"  d           download file  \n"
"  exit        exit program \n"
"  help        print this help \n";
//function
SD_CODE loginfunTest();
void fileTest();
void TokenManageTest();
void uploadfileTest(const uint64_t & key,const std::string & file,std::string &id);
void uploadfactoryTest(const uint64_t &key,const std::string & file,std::string &id,TreeEntity &nod);
uint64_t  loginTest();
uint64_t  loginPerTest();
void downloadTest(const uint64_t &key,const std::string &id,const std::string &path);
void downloadfactoryTest(const uint64_t &key,const std::string &id,const std::string &path);
void ecies_test();

//main
//
int PersonalKeyDecryptTest();
int main(int argc, char *argv[ ])
{
        //printf("argc:%d \n argv:%s \r\n",argc,argv[1]);
	//printf("use help command to get help information\n");
 	std::string file;	
//entry:
	/*std::string line;
	while(1){
	  printf("please input:");
	  std::getline(std::cin, line, '\n');
	  if (line.empty())continue;
	  if(line=="help"){
	  printf("%s", help_msg);
	  continue; 
	  }else if(line=="exit"){
	  return 0;	
	  }
          if (line.length() >= 1 && (line.substr(0, 1) == "a"||line.substr(0,1)=="d"))
          {
                int i;
                for (i=1; i < line.length() && line[i] == ' '; ++i)
                if (i == line.length()) continue;
                file = line.substr(i);
	        goto start;
          }
	 }//while*/
//start:
	file = argv[1];
	//if(file.empty())goto entry;
	printf("file:%s\r\n",file.c_str());
	if(file.empty())return 0;
	//goto entry;
	std::string id;
	uint64_t key;
	SDBasic::Init();
	//1:login get key
	key=loginPerTest();
	//2:get user info
	UserSpaceInfo info;
	SDBasic::GetUserSpaceInfo(key,info);
	//3:upload file
	id = info.rootDirId;
	TreeEntity node;
	uploadfactoryTest(key,file,id,node);
	//4:downlaod	
	id = node.id;
	std::string name = SDBasic::FileInfo::FileName(node.name);
	std::string tmp = "/tmp/surdoc/";
	std::string path =SDBasic::FileInfo::AppendPath(tmp,name);
	downloadfactoryTest(key,id,path);


	//other test
	//loginfunTest();
	//fileTest();
	//TokenManageTest();	
	//uploadfileTest(key,file);
	//downloadTest(key); //show upload fun
	//PersonalKeyDecryptTest();
	//return 0;
	//ecies_test();      
	//loginTest();
	
	SDBasic::UnInit();
return 1;
}
/*void ecies_test() {
  SDBasic::ecc::EciesHelper e("1111111111111111111111111111111111");
  SDBasic::bytearray::ByteArray p1 = SDBasic::bytearray::ByteArray::fromRawStr("1111");
  SDBasic::bytearray::ByteArray c1;
  SDBasic::bytearray::ByteArray p2;
  e.Encrypt(p1, c1);
  e.Decrypt(c1, p2);

  printf("CIPHER: %s\n", c1.toHexStr().c_str());
  printf("PLAIN: %s\n", p2.toRawStr().c_str());
}*/
void ecies_test() {
  SDBasic::ecc::EciesHelper e("1111111111111111111111111111111111");
  SDBasic::bytearray::ByteArray p1 = SDBasic::bytearray::ByteArray::fromRawStr("1111");
  SDBasic::bytearray::ByteArray c1;
  e.Encrypt(p1, c1);
  printf("CIPHER: %s\n", c1.toHexStr().c_str());

  SDBasic::bytearray::ByteArray c2 = 
      SDBasic::bytearray::ByteArray::fromHexStr("308201383082010c3081c706072a8648ce3d02013081bb0201013012020200e906092a8648ce3d0102030202014a3040041e000000000000000000000000000000000000000000000000000000000001041e0066647ede6c332c7f8c0923bb58213b333b20e9ce4281fe115f7d8f90ad043d0400fac9dfcbac8313bb2139f1bb755fef65bc391f8b36f8f8eb7371fd558b01006a08a41903350678e58528bebf8a0beff867a7ca36716f7e01f81052021e01000000000000000000000000000013e974e72f8a6922031d2603cfe0d7020102034000043d04014e57daa39c9fedcdac7c362623fa3ff64dafc5820455fdd287c87bcab100bcf0cd3bca087a79ddda7a71557fb72943b86e19a5be0ccc524cd6d957041065b39ee6aba8993f37a36560e4f7363104141ccf9a8228b2ecfb779af52f51a719b87e708f92");

  SDBasic::bytearray::ByteArray p2;
  e.Decrypt(c2, p2);
  printf("PLAIN: %s\n", p2.toRawStr().c_str());
}

int PersonalKeyDecryptTest() {
	const char* password = "aA111111";
	SDBasic::bytearray::ByteArray privkey_bin;
	const char* personal_key = "MIIBWDCCAQwwgccGByqGSM49AgEwgbsCAQEwEgICAOkGCSqGSM49AQIDAgIBSjBABB4AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEEHgBmZH7ebDMsf4wJI7tYITszOyDpzkKB_hFffY-QrQQ9BAD6yd_LrIMTuyE58bt1X-9lvDkfizb4-Otzcf1ViwEAagikGQM1BnjlhSi-v4oL7_hnp8o2cW9-AfgQUgIeAQAAAAAAAAAAAAAAAAAAE-l05y-KaSIDHSYDz-DXAgECA0AABD0EAWVjSreJOA2vomVI9fId6fPWhEKTrCvXTUswWxLtAZEUxPaf1c6Yd8v1HfDEDwJlr4CjMk8PGWLY1FMLBDCX59_y2wFLUkZy0FUOZDUWYBjRUJdkFBXFqWD6w8pdE7xZum2iEdXKZ2Dju7FmcCUEFEudo0qISCaNrRX5fqlZOoHnb7Xk";
	int ret = SDBasic::ecc::EciesHelper::TryParsePrivKey(
			"pySN8+XnS+bQMmDPEl7toeGra7XH0Qc2eeEuG/TWTpo=",
			password, &privkey_bin);
	if (ret != 1) {
		std::cout << "failed to parse private key" << endl;
		return -1;
	} 

	std::cout << "priv key (hex): " << privkey_bin.toHexStr() << endl;

	std::cout << "personal key length: " << strlen(personal_key) << endl;
	SDBasic::ecc::EciesHelper e(privkey_bin.toHexStr());

	ByteArray storage_key;
	ret = e.DecryptStorageKeyFromPersonalKey(
			personal_key,
			&storage_key);
	if (ret != 1) {
		std::cout << "failed to decrypt: " << ret << endl;
		return -1;
	} else {
		std::cout << "Success" << endl;
	}

	SDBasic::ecc::EciesHelper* e2 = 
		SDBasic::ecc::EciesHelper::CreateFromPrivateKeyReturnedByServer(
				"Abhw8tmVgt8ZicAv2ICY1N25iUiO9PxXJQyDrzul");
	ByteArray storage_key2;
	ret = e2->DecryptStorageKeyFromPersonalKey(
			personal_key,
			&storage_key2);
	if (ret != 1) {
		std::cout << "failed to decrypt: " << ret << endl;
		return -1;
	} else {
		std::cout << "Success" << endl;
	}

	return 0;
}

void downloadfactoryTest(const uint64_t &key,const std::string &id,const std::string &path){
  FileDownloaderEnc* d = FileDownloaderFactory::get().CreateDownloader(key);
	d->Download(key,id,path);
  delete d;
}
void downloadTest(const uint64_t &key,const std::string & id,const std::string& path)
{
	FileDownloadCallback cb;
	FileDownloaderEnc *ptr = new FileDownloaderEnc(key,cb);
//	std::string id="CN12ON22";
//	std::string path = "/tmp/surdocdownload/aa.txt";
	SD_CODE ret = ptr->Download(key,id,path);
	if(ret == SD_SUCCESSED)
	   printf("download successed\r\n");
	else
	   printf("download failed\r\n");
}
uint64_t loginTest()
{
    AccessToken token;
    std::string server = "https://e.surdoc.net";
	std::string account = "mac1@cc.cc";
    std::string plainpwd ="aA111111";
    printf("privatkey:%s\r\n",token.privatekey.c_str());
	return  SDBasic::LoginEnt(server,account,plainpwd,token);
}
uint64_t  loginPerTest()
{
	AccessToken token;
	std::string server = "https://open.surdoc.com";
	std::string account = "surdoctestooJUvF20@cc.cc";
	std::string plainpwd ="aA111111";
	printf("privatekey:%s\r\n",token.privatekey.c_str());
	return  SDBasic::Login(server,account,plainpwd,token);
}
void uploadfactoryTest(const uint64_t &key,const std::string & file,std::string &id,TreeEntity & node)
{
	std::string parentid = id;//"US17AKE7";//= "CN20G7MB";
  std::string name = FileInfo::FileName(const_cast<std::string&>(file));
	SDBasic::Upload(key,file,parentid, file, Timestamp::Now(), node);
	  std::string tmp = "/tmp/surdoc/";
	  tmp+=node.name;
	  printf("downlaod file path:%s\r\n",tmp.c_str());
	  id = node.id;
}
void TokenManageTest(){
	AccessToken token;
	uint64_t key;
	TokenManager::get().GetToken(key,token);	
	printf("privatkey= %s \r\n",token.privatekey.c_str());
}
void uploadfileTest(const uint64_t & key,const std::string & file,std::string &id){
	std::string parentid = "CN20G7MB";
        
	TreeEntity node;
	FileUploadCallback cb;
	FileUploaderEnc * ptr = new FileUploaderEnc(key,cb);

  std::string name = FileInfo::FileName(const_cast<std::string&>(file));
	SD_CODE ret = ptr->UploadFileByParentId(key,file,parentid,
                                          name,
                                          Timestamp::Now(),
                                          code::SHARE_NONE,node);
	if(ret == SD_SUCCESSED)
	 {
	  printf("upload successed\r\n");
	  id = node.id;
	 }	
	else
	   printf("upload failed\r\n");	 

}
void fileTest()
{
	std::string s1 = "/etc/";
	std::string s2 = "11";
//	std::string path = FileInfo::AppendPath(s1,s2);
//	printf("path = %s\r\n",path.c_str());
	 std::string str = "/home/Demo/test.c";
        FileStatus st = FileInfo::StatFile(str);
        FileStatus st1;
        printf("%s size:%s default:%s\r\n",
               str.c_str(), NumStr(st.size).c_str(), NumStr(st1.size).c_str());

}
SD_CODE loginfunTest()
{
	Request::CURL_Init();
	std::string response;
	AccessToken token;
	Request curl;
	curl.Init();
	std::string account = "mac1@cc.cc";
	std::string plainpwd ="aA111111";
	std::string md5pwd = Digestor::HashPassword(plainpwd);
	printf("pwd:%s,md5pwd:%s\r\n",plainpwd.c_str(),md5pwd.c_str());
	SD_CODE ret = curl.Ent_Login("https://e.surdoc.net",account,md5pwd,15000,response);
	if(ret != SD_SUCCESSED){
	printf("\r\nEnt_login error\r\n");
	Request::CURL_UnInit();	
	return ret;}
	else{
	printf("\r\nEnt_login successed.\r\n");}		
	
	AccessToken temp_token;
	SDContext *ctx = &CTX;
 	CJsonParse parse(response);
 	if(!parse.IsValid()|| !parse.Enterprise_GetAccessToken(&temp_token, ctx))
	    printf("failed to parse login response.\r\n");	
	printf("privatekey:%s\r\n",temp_token.privatekey.c_str());
	printf("rootid:%s\r\n",ctx->root_dir_id.c_str());
	
	uint64_t key=TokenManager::get().SetToken(temp_token);
	AccessToken tm_token;
	 TokenManager::get().GetToken(key,tm_token);
	printf("TokenManage privatekey:%s\r\n",tm_token.privatekey.c_str());
	
	ByteArray pk;
	int iret = EciesHelper::TryParsePrivKey(temp_token.privatekey,plainpwd,&pk);
	if(iret == 1)
	{
           token.privatekey = pk.toUrlBase64Str();		
	   printf("token.privatekey:%s\r\n",token.privatekey.c_str());
	}else{
		printf("failed to parse user`s private key\r\n");
		return SD_FAILED;
	}
	ByteArray admin_pk;
	iret = EciesHelper::TryParsePrivKey(temp_token.admin_privkey, plainpwd, &admin_pk);
	if (iret == 1) {
		token.admin_privkey = admin_pk.toUrlBase64Str();
		printf("token.admin_privkey:%s\r\n",token.admin_privkey.c_str());
	} else {
		printf("failed to parse admin's private key\r\n");
		return SD_FAILED;
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

	//uint64_t key = TokenManager::get().SetToken(token);

	Request::CURL_UnInit();
  return SD_SUCCESSED;
}

