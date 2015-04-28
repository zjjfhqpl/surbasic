#include "surbasic/FileUploaderFactory.h"

using namespace SDBasic::code;
using namespace SDBasic::lock;

FileUploaderFactory * FileUploaderFactory::ins_ = NULL;
FileUploaderFactory::FileUploaderFactory(void):uploadenc_(NULL) {}
FileUploaderFactory::~FileUploaderFactory(void) {}

FileUploaderFactory& FileUploaderFactory::get() {
  static MutexLock lockins_;
  SD_Lock lock(lockins_);
  if (ins_ == NULL) {
    ins_ = new FileUploaderFactory();
 }
  return (*ins_);
}

bool FileUploaderFactory::Upload(const uint64_t & key,const std::string & parentid,const std::string &filepath,TreeEntity & node){
         if(uploadenc_==NULL){
    	 uploadenc_ = new FileUploaderEnc(key,cb_);};
	 SD_CODE code = uploadenc_->UploadFileByParentId(key,filepath,parentid,SHARE_NONE,node);
        return (code == SD_SUCCESSED)?true:false;
}

