//Author:zhaojunjiang 2015-4-7
#ifndef FILEINFO_H
#define FILEiNFO_H
#include "Type.h"
#include "timestamp.h"
#include <sys/stat.h>
#include <unistd.h>
#include <string>
using namespace std;
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

std::string SplitRight(std::string & p,char c){
	if(p.empty())return p;
	size_t pos = p.find_last_of(c);
	if (pos == string::npos)return p;
	if(pos==p.size()-1)return "";
	return p.substr(pos+1);
}
std::string SplitLeft(std::string &p,char c){
	if(p.empty())return p;
	size_t pos = p.find_last_of(c);
	if(pos == string::npos)return p;
	if(pos == p.size()-1)return "";
	return p.substr(0,pos);
}
//c:\1.txt ->1.txt
std::string FileName(std::string &path){
	return SplitRight(path,'/');
}
//c:\1.txt ->1
std::string BaseName(std::string &path){
      std::string filename = FileName(path);
      assert(!filename.empty());
      return SplitLeft(filename,'.');
}
//c:\1.txt ->txt
std::string Extension(std::string &path){
     std::string tmp = FileName(path);
     return SplitRight(tmp,'.'); 
}
//c:\1.txt ->c:
std::string ParentPath(std::string &path){
     return SplitLeft(path,'/');
}
// append path
std::string AppendPath(std::string &pathLeft,std::string & pathRight){
    return pathLeft+"/"+pathRight;
}


FileStatus StatFile(const std::string &path){
	FileStatus st;

	struct stat buf;
	if(stat(path.c_str(), &buf) == -1){ 
		perror("fail to stat");
	}		
	if(S_ISREG(buf.st_mode)) {
		st.type = FileInfo::Regular;
		printf("regular file \n");}
	else if(S_ISDIR(buf.st_mode)) {
		st.type = FileInfo::Directory;
		printf( "directory file\n");}
	else if(S_ISCHR(buf.st_mode)){
		st.type = FileInfo::Character;
		printf( "character special file\n");}
	else if(S_ISBLK(buf.st_mode)){
		st.type = FileInfo::Block;
		printf( "block special file\n");}
	else if(S_ISFIFO(buf.st_mode)){
		st.type = FileInfo::Fifo;
		printf( "FIFO file\n");}
	else if(S_ISSOCK(buf.st_mode)){
		st.type = FileInfo::Socket;
		printf( "socket file\n");}
	else if(S_ISLNK(buf.st_mode)) {
		st.type = FileInfo::Link;
		printf( "symbolic link file\n");}
	else{
		st.type = FileInfo::Error;
		printf( "unkonw\n");}
	st.size = buf.st_size;
	st.modifytime = Timestamp(buf.st_mtime);
	return st;
}
}}//namespace 
#endif
