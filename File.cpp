//
//  NSData_Aes256.h
//  SurDoc
//
//  Created by zhaojunjiang on 14/12/22.
//  Copyright (c) 2014年 sursen. All rights reserved.
//
#include "surbasic/File.h"
namespace SDBasic{
namespace file{
File::File()
: fp_(NULL)
{
}

File::~File(void)
{
    if (fp_ != NULL)
        fclose(fp_);
}
void File::Close()
{
    assert (IsOpened());
    fclose(fp_);
    fp_= NULL;
}
bool File::Open(const std::string & path)
{
    assert (!this->IsOpened());
    fp_ = fopen( path.c_str(), "r" );
    if(fp_==NULL) return false;
    return true;
}
bool File::OpenForRead(const std::string &path) {
    fp_ = fopen( path.c_str(), "rb+" );
    if(fp_==NULL) return false;
    return true;
}

bool File::OpenForWrite(const std::string &path) {
    fp_ = fopen( path.c_str(), "ab+" );
    if(fp_==NULL) return false;
    return true;
}

bool File::IsOpened() const {
    return fp_ != NULL;
}

bool File::Read(void *buf, size_t buf_len, unsigned long *read_bytes) {
    assert(IsOpened());
    if(!IsOpened())return false;
    *read_bytes = fread( buf, 1, buf_len, fp_ );
    return true;
}

bool File::Write(const void *buf, size_t buf_len, unsigned long* write_bytes) {
    assert(IsOpened());
    if(!IsOpened())return false;
    fseek ( fp_, 0, SEEK_END );
    long size = 0;
    size = ftell ( fp_ );
    rewind ( fp_ );
   *write_bytes =fwrite ( buf, 1, buf_len, fp_ );
    return true;
}
bool File::SeekTo(Whence whence, int64_t offset, int64_t *pos_new)
{
   assert(IsOpened());
   if(!IsOpened())return false;
   return (fseek(fp_,offset,SEEK_SET)==0)?true:false;
}

bool File::Flush()
{
  assert(IsOpened());
  if(fflush(fp_)==0)return true;
  return false;
}

}}//namespace
