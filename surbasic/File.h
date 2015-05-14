//
//  NSData_Aes256.h
//  SurDoc
//
//  Created by zhaojunjiang on 14/12/22.
//  Copyright (c) 2014年 sursen. All rights reserved.
//
#ifndef File_H
#define File_H
#include "surbasic/Type.h" 
namespace SDBasic{
namespace file{

class  File
{
public:
    enum Whence {
        FROM_BEGIN   = 0,
        FROM_CURRENT = 1,
        FROM_END     = 2
    };
    
    File();
    ~File(void);
    bool Open(const std::string & path);
    bool OpenForRead(const std::string &path);
    bool OpenForWrite(const std::string &path);
    bool IsOpened() const;
    bool Read(void *buf, size_t buf_len, unsigned long *read_bytes);
    bool Write(const void *buf, size_t buf_len, unsigned long *write_bytes);
    bool SeekTo(Whence whence, int64_t offset, int64_t *pos_new = NULL);
    bool Flush();
    void Close();
    
private:
    //std::wstring path_;
    FILE *fp_;
};
}}//namespace
#endif
