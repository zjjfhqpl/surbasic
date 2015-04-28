//Author:zhaojunjiang 2015-4-13
#include "surbasic/Type.h"
#ifndef UPLOADPROGESSINFO_H
#define UPLOADPROGESSINFO_H
namespace SDBasic{
namespace uploadprg{


enum UploadProgressMsgType
{
	FILE_BK_START,
	FILE_BK_HASHING_START,
	FILE_BK_HASHING_END,
	FILE_BK_UPLOAD_START,
	FILE_BK_UPLOAD_PROGRESS,
	FILE_BK_UPLOAD_END,
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



}}//namespace
#endif
