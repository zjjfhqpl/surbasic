#include "surbasic/FileDownloaderEnc.h"

namespace SDBasic{
namespace download{

FileDownloaderEnc::FileDownloaderEnc(const uint64_t &key,FileDownloadCallback cb)
:cb_(cb)
{
	 AccessToken token;
	TokenManager::get().GetToken(key,token);
	ecies_=EciesHelper::CreateFromPrivateKeyReturnedByServer(token.privatekey);
}
FileDownloaderEnc::~FileDownloaderEnc(){
	if(ecies_!=NULL){
	delete ecies_;}
}
void FileDownloaderEnc::SendProgressMsg(DownloadProgressInfo::MsgType type){
	proginfo_.type = type;
	if (cb_.func)
		cb_.func(proginfo_, cb_.userdata);
}
std::string FileDownloaderEnc::GenUniqueTmpFile(const std::string &origpath){
	std::string tmpfile = origpath + ".sdown";
	FileStatus st = FileInfo::StatFile(tmpfile);
	if (st.type == FileInfo::NotExisted || st.type == FileInfo::Error)
		return tmpfile;

	for (int i = 1; ;++i)
	{
		tmpfile = origpath + "." + NumFmt("(%d)", i).str() + ".sdown";
		FileStatus st = FileInfo::StatFile(tmpfile);
		if (st.type == FileInfo::NotExisted || st.type == FileInfo::Error)
			return tmpfile;
	}
}
SD_CODE FileDownloaderEnc::Download(const uint64_t & key,const std::string & id,const std::string &path){
	
	ByteArray storagekey;
	request_.Init();
	proginfo_.Reset();

	const std::string infoid = Digestor::Md5str(id+path);
	int offset = 0;
	proginfo_.size = 0;
	proginfo_.offset = offset;
	SendProgressMsg(DownloadProgressInfo::FILE_DOWN_START);
	std::string tmpfile = GenUniqueTmpFile(path);
	File fp;
	bool blret = fp.OpenForWrite(tmpfile);
	if (!blret) {
		printf("OpenForWrite() failed: %s", tmpfile.c_str());
		FileInfo::Remove(tmpfile.c_str());
		return SD_LOCAL_FILE_ERROR;
	}	
	//blret = fp.SeekTo(File::FROM_END,offset);
	std::string personal_key;
	std::string etag;
	std::string plain_file_path;
	AccessToken token;
	TokenManager::get().GetToken(key,token);
	SD_CODE ret = request_.Enc_DownloadFile(token, id, etag, offset, &fp, personal_key);
	fp.Close();
	printf("\r\nEenc_key:%s\r\n",personal_key.c_str());
	if (ret == SD_OPENAPI_SERVER_FILE_CHANGED) { 
		printf("[%s] server file changed, we must re-download this file", path.c_str());
		FileInfo::Remove(tmpfile);
		return SD_FAILED;
	} 	
	
	FileStatus st = FileInfo::StatFile(tmpfile);
	if (st.type != FileInfo::Regular) {
		printf("file downloaded, but we can't find it any more: %s", tmpfile.c_str());
		FileInfo::Remove(tmpfile);
		return SD_FAILED;
	}
	if (ret != SD_SUCCESSED) {
		if (st.size == 0)
			FileInfo::Remove(tmpfile);
		printf("[%s] failed to download file: %s", path.c_str(), tmpfile.c_str());
		return ret;
	}
	SendProgressMsg(DownloadProgressInfo::FILE_DOWN_DONE);
	//general file
	if (personal_key.empty()) {
		plain_file_path = tmpfile;
		goto decrypt_done;
	}	
	//decrypt tmpfile
	blret = ecies_->DecryptStorageKeyFromPersonalKey(personal_key, &storagekey);
	if (!blret) {
		printf("can not decrypt personal key to storage key, Reason:");
		FileInfo::Remove(tmpfile);
		SendProgressMsg(DownloadProgressInfo::FILE_CHECKSUM_ERROR);
		return SD_FAILED;
	}	

 	plain_file_path = tmpfile + ".tmp";
	blret = AESHelper::DecAndUncompressFile(storagekey, tmpfile, plain_file_path);
	if (!blret) {
		printf("failed to decrypt and unzip file, reason: %s", plain_file_path.c_str());
		FileInfo::Remove(tmpfile);
		SendProgressMsg(DownloadProgressInfo::FILE_CHECKSUM_ERROR);
		return SD_FAILED;
	}
	SendProgressMsg(DownloadProgressInfo::FILE_CHECKSUM_START);
decrypt_done:
	SendProgressMsg(DownloadProgressInfo::FILE_CHECKSUM_OK);

	if (FileInfo::Rename(plain_file_path, path) == false) {
		printf("failed to rename %s to %s", plain_file_path.c_str(), path.c_str());
		FileInfo::Remove(plain_file_path);
		SendProgressMsg(DownloadProgressInfo::FILE_ERROR);
		return SD_LOCAL_FILE_ERROR;
	}
	this->proginfo_.type = DownloadProgressInfo::FILE_DONE;
	SendProgressMsg(DownloadProgressInfo::FILE_DONE);
	//clear resume info
	FileInfo::Remove(tmpfile);
	return SD_SUCCESSED;
}


}}//namespace
