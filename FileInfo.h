//Author:zhaojunjiang 2015-4-7
#ifndef FILEINFO_H
#define FILEINFO_H
#include "surbasic/Type.h"
#include "surbasic/timestamp.h"
#include <sys/stat.h>
#include <unistd.h>
#include <string>
using namespace SDBasic::time;

namespace SDBasic{
namespace FileInfo{

enum FileType{
        Error=0,
        NotExisted,
        Regular,  //file
        Directory,//dir
        Character,//Character special file
        Block,//block special file
        Fifo,//fifo file
        Socket,//socket file
        Link,//character link file              
};

struct FileStatus{
        FileType    type;
        int64_t     size;
       Timestamp   modifytime;
        FileStatus():type(FileInfo::Error),size(0),modifytime(Timestamp::Now())
        {}
};

std::string SplitRight(std::string & p,char c);
std::string SplitLeft(std::string &p,char c);
//c:\1.txt ->1.txt
std::string FileName(std::string &path);
//c:\1.txt ->1
std::string BaseName(std::string &path);
//c:\1.txt ->txt
std::string Extension(std::string &path);
//c:\1.txt ->c:
std::string ParentPath(std::string &path);
// append path
std::string AppendPath(std::string &pathLeft,std::string & pathRight);
//get file attribute
FileStatus StatFile(const std::string &path);
//del file or dir
bool Remove(const std::string &path);
//rename
bool Rename(const std::string & src,const std::string &dst);
//mkdir
bool CreateDir(const std::string path);
}}//namespace
#endif
