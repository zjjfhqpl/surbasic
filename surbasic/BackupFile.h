//Author:zhaojunjiang 

#ifndef BACKUPFILE_H
#define BACKUPFILE_H
#include "surbasic/Public_Define.h"
#include <string>
#include "surbasic/timestamp.h"
using namespace SDBasic;
using namespace SDBasic::code;
using namespace std;
enum OnDupOption
{
	OVERWRITE = 0,
	DISCARD,
	RENAME,
};

class BackupFile{
public:
	BackupFile(void);
	virtual ~BackupFile(){}
	static bool Compless(const BackupFile *left,const BackupFile *right)
	{
		return left->size < right->size;
	}
	std::string name;
	std::string id;
	std::string parentid;
	std::string fullpath;
	std::string rootpath;
  SDBasic::time::Timestamp uploadtime;

	OnDupOption ondup;
	FileConvertType convert;

	BACKUP_TYPE type;
	BACKUP_STATUS status;
	CREATE_TYPE createtype;
	std::string support;  // what does mean
	
  SDBasic::time::Timestamp modifytime;
	int64_t size;
	std::string digest;

	// resume upload
	std::string uploadid;//large file temp ID
	int64_t offset;


	 //anvanced encryption
	std::string sha512;
	std::string encFilePath;
	int64_t encFileSize;
	std::string encSha1;
	std::string sharekey;
	std::string personalkey;
   	 std::string storagekey; //storage key  (urlbase64)

	//enterprises 
	ShareType sharetype;
	std::string sharerootid;	

};
inline BackupFile::BackupFile(void){
	uploadtime = SDBasic::time::Timestamp::Now();
	type = BACKUP_SRC;
	status = NO_BACKUP;
	createtype = FILE_CREATE_NEW;
	modifytime = SDBasic::time::Timestamp::Now();
	size = 0;
	offset = 0;
	encFileSize = 0;
	ondup = OVERWRITE;
	convert = NONDOC;
	sharetype = SHARE_NONE;
}
#endif
