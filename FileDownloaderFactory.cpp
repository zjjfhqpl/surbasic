#include "surbasic/FileDownloaderFactory.h"

FileDownloaderFactory * FileDownloaderFactory::ins_ = NULL;

FileDownloaderFactory::FileDownloaderFactory():downloadenc_(NULL){}

FileDownloaderFactory::~FileDownloaderFactory(){}

FileDownloaderFactory& FileDownloaderFactory::get(){
	static MutexLock lockins_;
	SD_Lock lock(lockins_);
	if(ins_ == NULL){
	ins_ = new FileDownloaderFactory();}
	return (*ins_);
}

bool FileDownloaderFactory::Download(const uint64_t &key,const std::string & id,const std::string &path){
	if(downloadenc_==NULL){
	downloadenc_ = new FileDownloaderEnc(key,cb_);}
	SD_CODE ret = downloadenc_->Download(key,id,path);
	return (ret==SD_SUCCESSED?true:false);
}
