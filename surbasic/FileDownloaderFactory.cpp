#include "surbasic/FileDownloaderFactory.h"

FileDownloaderFactory * FileDownloaderFactory::ins_ = NULL;

FileDownloaderFactory::FileDownloaderFactory() {}

FileDownloaderFactory::~FileDownloaderFactory(){}

FileDownloaderFactory& FileDownloaderFactory::get(){
	static MutexLock lockins_;
	SD_Lock lock(lockins_);
	if(ins_ == NULL){
	ins_ = new FileDownloaderFactory();}
	return (*ins_);
}

FileDownloaderEnc*  FileDownloaderFactory::CreateDownloader(uint64_t key) {
  return new FileDownloaderEnc(key, cb_);
}
