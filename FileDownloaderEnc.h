//Author:zhaojunjiang 2015-4-16
#ifndef FILEDOWNLOADERENC_H
#define FILEDOWNLOADERENC_H
#include "surbasic/Type.h"
#include "surbasic/File.h"
#include "surbasic/SDCurl.h"
#include "surbasic/ByteArray.h"
#include "surbasic/Digestor.h"
#include "surbasic/FileInfo.h"
#include "surbasic/StrUtils.hpp"
#include "surbasic/EciesHelper.h"
#include "surbasic/AESHelper.h"
#include "surbasic/Public_Define.h"
#include "surbasic/TokenManager.h"
#include "surbasic/AccessToken.h"
#include "surbasic/Base64Helper.h"
using namespace SDBasic::base64;
using namespace SDBasic::aes;
using namespace SDBasic::FileInfo;
using namespace SDBasic::fmt;
using namespace SDBasic::file;
using namespace SDBasic::digestor;
using namespace SDBasic::bytearray;
using namespace SDBasic::tokenmg;
using namespace SDBasic::token;
using namespace SDBasic::curl;
using namespace SDBasic::ecc;
using namespace SDBasic::code;

namespace SDBasic{
namespace download{


struct  DownloadProgressInfo
{
	enum MsgType
	{
		FILE_DOWN_START = 0,
		FILE_DOWN_PROGRESS,
		FILE_DOWN_ERROR,
		FILE_DOWN_DONE,
		FILE_DECRYPT_START,
		FILE_DECRYPT_ERROR,
		FILE_DECRYPT_DONE,
		FILE_CHECKSUM_START,
		FILE_CHECKSUM_ERROR,
		FILE_CHECKSUM_OK,
		FILE_DONE,
		FILE_ERROR,
	} type;

	std::string id;
	SD_CODE code;
	int64_t size;
	int64_t offset;
	int64_t cur_size;
	std::string etag;
	
	void Reset()
	{
		type = FILE_DOWN_START;
		id.clear();
		code = SD_SUCCESSED;
		size = 0;
		offset = 0;
		cur_size = 0;
		etag.clear();
	}
};


typedef void(*OnPerFileDownloadProgress)(const DownloadProgressInfo &prog, void *userdata);
typedef Callback<OnPerFileDownloadProgress> FileDownloadCallback;
class FileDownloaderEnc
{
public:
	FileDownloaderEnc(const uint64_t &key,FileDownloadCallback cb);
	virtual ~FileDownloaderEnc();
	SD_CODE Download(const uint64_t &key,const std::string &id,const std::string& path);
private:
	void SendProgressMsg(DownloadProgressInfo::MsgType type);
	std::string GenUniqueTmpFile(const std::string &origpath);
private:
	EciesHelper *ecies_;
	EciesHelper *admin_ecies_;
	Request request_;
	FileDownloadCallback cb_;
	DownloadProgressInfo proginfo_;
};
}}//namespace
#endif
