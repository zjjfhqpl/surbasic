//Author:zhaojunjiang 2015-4-7
#include "surbasic/FileInfo.h"

namespace SDBasic{
namespace FileInfo{

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
  if(stat(path.c_str(), &buf) == -1) { 
    // perror("fail to stat");
    if (errno == ENOENT) {
      st.type = NotExisted;
      return st;
    } else {
      st.type = Error;
      return st;
    }
  }

	if(S_ISREG(buf.st_mode)) {
		st.type = Regular;
		printf("regular file \n");}
	else if(S_ISDIR(buf.st_mode)) {
		st.type = Directory;
		printf( "directory file\n");}
	else if(S_ISCHR(buf.st_mode)){
		st.type = Character;
		printf( "character special file\n");}
	else if(S_ISBLK(buf.st_mode)){
		st.type = Block;
		printf( "block special file\n");}
	else if(S_ISFIFO(buf.st_mode)){
		st.type = Fifo;
		printf( "FIFO file\n");}
	else if(S_ISSOCK(buf.st_mode)){
		st.type = Socket;
		printf( "socket file\n");}
	else if(S_ISLNK(buf.st_mode)) {
		st.type = Link;
		printf( "symbolic link file\n");}
	else{
		st.type = Error;
		printf( "unknown\n");}
	st.size = buf.st_size;
	st.modifytime = Timestamp(buf.st_mtime);
	return st;
}
bool Remove(const std::string &path){
	FileStatus st = StatFile(path);
	if(st.type == Directory){
	  if(rmdir(path.c_str())==-1)
		printf("del dir error \r\n");
	 printf("del dir successed\r\n");
	}
	else{
	   if(remove(path.c_str())==-1)
		printf("del file error\r\n");
	printf("del file successed \r\n");
	}

  return true;
}
bool Rename(const std::string &src,const std::string &dst){
	int ret = ::rename(src.c_str(),dst.c_str());
	if(ret!=0)return false;	
	return true;
}
bool CreateDir(const std::string path){
	if(mkdir(path.c_str(),0644)){
	if(errno!=EEXIST) return false;}
	return true;
}
}}//namespace 
