#ifndef FILEDOWNLOADERFACTORY_H
#define FILEDOWNLOADERFACTORY_H
#include "surbasic/Type.h"
#include "surbasic/Mutex.hpp"
#include "surbasic/Public_Define.h"
#include "surbasic/nocopyable.hpp"
#include "surbasic/FileDownloaderEnc.h"

using namespace SDBasic::lock;
using namespace SDBasic::code;
using namespace SDBasic::download;

class FileDownloaderFactory: public SDBasic::nocopyable
{
public:
	FileDownloaderFactory();
	virtual ~FileDownloaderFactory();
	static FileDownloaderFactory & get();
	bool Download(const uint64_t & key,const std::string & id,const std::string & path);

  FileDownloaderEnc* CreateDownloader(uint64_t key);
private:
	static FileDownloaderFactory * ins_;
	FileDownloadCallback cb_;

};


#endif
