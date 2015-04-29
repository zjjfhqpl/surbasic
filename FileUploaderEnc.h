//Author:zhaojunjiang 2015-4-8
#ifndef FILEUPLOADERENC_H
#define FILEUPLOADERENC_H
#include "surbasic/Type.h"
#include "surbasic/Public_Define.h"
#include "surbasic/EciesHelper.h"
#include "surbasic/CurlHeaders.h"
#include "surbasic/Base64Helper.h"
#include "surbasic/ByteArray.h"
#include "surbasic/StoppingFlag.hpp"
#include "surbasic/Callback.h"
#include "surbasic/AESHelper.h"
#include "surbasic/TokenManager.h"
#include "surbasic/SDCurl.h"
#include "surbasic/TreeEntity.h"
#include "surbasic/BackupFile.h"
#include "surbasic/FileInfo.h"
#include "surbasic/Digestor.h"
#include "surbasic/StrUtils.hpp"
#include "surbasic/DateObjects.h"
using namespace SDBasic::fmt;
using namespace SDBasic::digestor;
using namespace SDBasic::FileInfo;
using namespace SDBasic::tree;
using namespace SDBasic::curl;
using namespace SDBasic::tokenmg;
using namespace SDBasic;
using namespace SDBasic::code;
using namespace SDBasic::ecc;
using namespace SDBasic::headers;
using namespace SDBasic::bytearray;
using namespace SDBasic::base64;
using namespace SDBasic::aes;

namespace SDBasic{
namespace upload{


enum UploadProgressMsgType
{
	FILE_BK_START,
	FILE_BK_HASHING_START,
	FILE_BK_HASHING_END,
	FILE_BK_UPLOAD_START,
	FILE_BK_UPLOAD_PROGRESS,
	//FILE_BK_UPLOAD_END,
	FILE_BK_END,
}; 

struct UploadProgressInfo
{
  UploadProgressMsgType type;
  std::string path;
  SD_CODE code;
  int64_t size;
  int64_t cur_size;
  int64_t offset;
  double upload_speed;

  std::string doc_id;
  std::string storage_key;
  
  void Reset()
  {
    type = FILE_BK_START;
    path.clear();
    code = SD_SUCCESSED;
    size = 0;
    cur_size = 0;
    offset = 0;
    upload_speed = 0.0;
		doc_id.clear();
		storage_key.clear();
  }

};

typedef void(*OnPerFileUploadProgress)(const UploadProgressInfo &prog, void *userdata);
typedef Callback<OnPerFileUploadProgress> FileUploadCallback;


class FileUploaderEnc 
{
public:
	FileUploaderEnc(const uint64_t &key, FileUploadCallback cb);	
	virtual ~FileUploaderEnc();

	SD_CODE UploadFileByParentId(const uint64_t &key,
                               const std::string &fullpath, 
                               const std::string &parentid,
                               const std::string& name, 
                               Timestamp modifytime,
                               ShareType share_type,
                               TreeEntity &node);

    static int UploadFileProgress(const CurlProgressInfo &info, void *userdata);
private:
	static int UploadFileProgess(const CurlProgressInfo &info,void *userdata);
	std::string GeneratePersonalKey(const ByteArray &storage_key);
	//openapi 	
	SD_CODE Enc_CheckIfFileExist(const AccessToken &token);
	SD_CODE Enc_RapidUploadFile(TreeEntity &node);
	SD_CODE Enc_UploadSmallFile(const AccessToken &token,TreeEntity &node);
	SD_CODE Enc_CheckIfShareKeyMatched(const SameHashFileInfo *key, const ByteArray &storageKey);
	SD_CODE Enc_GZipAndEncFile(const ByteArray &storageKey);
	SD_CODE Enc_UploadBigFile(TreeEntity &node);
private:	
	EciesHelper *ecies_;
	bool is_big_file_;
	bool upload_encypt_file_;
	FileUploadCallback cb_;
	UploadProgressInfo prog_;
	Request request_;
	BackupFile file_;

};

}}//namespace
#endif
