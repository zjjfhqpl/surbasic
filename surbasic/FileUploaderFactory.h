//Author:zhaojunjiang 205-4-15
#ifndef FILEUPLOADERFACTORY_H
#define FILEUPLOADERFACTORY_H
#include "surbasic/Type.h"
#include "surbasic/TreeEntity.h"
#include "surbasic/Mutex.hpp"
#include "surbasic/nocopyable.hpp"
#include "surbasic/FileUploaderEnc.h"
#include "surbasic/Public_Define.h"
using namespace SDBasic::lock;
using namespace SDBasic::code;
using namespace SDBasic::upload;
using namespace SDBasic::tree;

class FileUploaderFactory:public SDBasic::nocopyable
{
public:
  FileUploaderFactory(void);
  virtual ~FileUploaderFactory(void);
  static FileUploaderFactory& get();

  FileUploaderEnc* CreateUploader(const uint64_t key);
private:
  static FileUploaderFactory *ins_;	
  FileUploaderEnc * uploadenc_;
  FileUploadCallback cb_;
};

#endif
