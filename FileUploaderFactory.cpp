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

FileUploaderEnc* FileUploaderFactory::CreateUploader(const uint64_t key) {
  return new FileUploaderEnc(key, cb_);
}

