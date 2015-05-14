#include "surbasic/FileUploaderEnc.h"
#include "surbasic/ByteArray.h"
using namespace SDBasic::bytearray;
namespace SDBasic{
namespace upload{


#define BIG_FILE_THRESHHOLD (10*1024*1024)
const static std::string TmpFile = "/tmp/surdoc";
static std::string EncryptStorageKeyToShareKey(const std::string &sha512, const ByteArray &storageKey)
{
	ByteArray sha512bytes = ByteArray::fromHexStr(sha512);
	assert (sha512bytes.size() == 64);
	ByteArray aesKey;
	aesKey.insert(aesKey.end(), sha512bytes.begin()+16, sha512bytes.begin()+32);
	aesKey.insert(aesKey.end(), sha512bytes.begin()+48, sha512bytes.end());

  std::string base64_storage_key = Base64Helper::Encode(storageKey);
  ByteArray out = AESHelper::Encrypt256(aesKey, ByteArray::fromRawStr(base64_storage_key));
  return out.toUrlBase64Str();
}


static SD_CODE DecryptStorageKeyFromShareKey(const std::string &share_key, const ByteArray &share_enc_key, 
                                     ByteArray *storage_key)
{
  ByteArray storage_key_cipher;
  if (!UrlBase64Helper::Decode(share_key, storage_key_cipher)){ 
    printf( "can not url64decode share_key to get storage_key_cipher");
    return SD_FAILED;}
  ByteArray base64_storage_key = AESHelper::Decrypt256(share_enc_key, storage_key_cipher);
  if (base64_storage_key.size() != 44){ 
     printf("base64_storage_key should be 44 bytes");
     return SD_FAILED;}
  if (!Base64Helper::Decode(base64_storage_key, *storage_key)) {
     printf("base64_storage_key is not valid base64-encoded string");
     return SD_FAILED;}

  if (storage_key->size() != 32) {
     printf( "storage_key should be 32 bytes");
    return SD_FAILED; }

  return SD_SUCCESSED;
}

FileUploaderEnc::FileUploaderEnc(const uint64_t &key, FileUploadCallback cb)
:cb_( cb)
{
	AccessToken token;
	TokenManager::get().GetToken(key,token);
	ByteArray sk;
	ecies_ = EciesHelper::CreateFromPrivateKeyReturnedByServer(token.privatekey);
	assert(ecies_ != NULL);
	
	upload_encypt_file_ = true;
}

FileUploaderEnc::~FileUploaderEnc(void)
{
	if (ecies_ != NULL)
		delete ecies_;
}

int FileUploaderEnc::UploadFileProgress(const CurlProgressInfo &info, void *userdata)
{
	/*assert (userdata != NULL);
	FileUploaderEnc *self = static_cast<FileUploaderEnc*>(userdata);

	if (self->stopping_->IsStopping())
		return CURLE_ABORTED_BY_CALLBACK;

  	if (!self->uploading_content_)
    	return 0;

	if (self->last_prog_time_.elapsed() > 1000)
	{	
		self->prog_.type = FILE_BK_UPLOAD_PROGRESS;
    		self->prog_.cur_size = self->prog_.offset + info.upload_content_bytes;
    		self->prog_.upload_speed = info.ulspeed;

		// 将密文上传进度等比例的转换为明文大小
		if (self->upload_encypt_file_) {
			double percent = (double)self->prog_.cur_size / self->file_.encFileSize;
			self->prog_.cur_size = percent * self->file_.size;	
		}

		if (self->cb_.func != NULL)
			self->cb_.func(self->prog_, self->cb_.userdata);

		self->last_prog_time_.update();
	}*/

	return 0;
}

SD_CODE FileUploaderEnc::UploadFileByParentId(const uint64_t &key,
                                              const std::string &fullpath, 
                                              const std::string &parentid,
                                              const std::string& name, 
                                              Timestamp modifytime,
                                              ShareType share_type,
                                              TreeEntity &node)
{
	ByteArray   storagekey;
	ByteArray   enckey;
	this->prog_.Reset();
	request_.Init();	
	request_.SetProgressCallback(UploadFileProgress,this);
	
	node = TreeEntity();
	SD_CODE ret;
	FileStatus st = FileInfo::StatFile(fullpath);
	if(st.type != FileInfo::Regular)
		return SD_LOCAL_FILE_ERROR;	
	file_ = BackupFile();
	file_.size = st.size;
	file_.parentid=parentid;
	file_.sharetype=share_type;
	std::string tmpname = fullpath;	
	file_.fullpath = fullpath;
  file_.name = name;
  file_.modifytime = modifytime;
	ByteArray allhash; 
	if (Digestor::HashFile(fullpath, HASH_ALL, allhash) == false) {
		printf("failed to hash file: %s", file_.fullpath.c_str());
		return SD_LOCAL_FILE_ERROR;	
	}
	ByteArray digest, sha512;
	digest.insert(digest.end(), allhash.begin(), allhash.begin()+16+20);
	sha512.insert(sha512.end(), allhash.begin()+16+20, allhash.end());
	assert(allhash.size() == 16+20+512/8);
	assert(digest.size() == 16+20);

	file_.digest = digest.toHexStr() + NumFmt("%llx",st.size).c_str();
	file_.sha512 = sha512.toHexStr();	
	AccessToken token;
	TokenManager::get().GetToken(key,token);
	ret = this->Enc_CheckIfFileExist(token);
	switch(ret){
	case SD_FILE_NEED_UPLOAD:
		goto upload_file_content;
	case SD_FILE_DIGEST_EXIST: 			
		assert (!file_.storagekey.empty());
		assert (!file_.personalkey.empty());
	//	ret = Enc_RapidUploadFile(node);
		goto deal_with_return_code;
	default:
		goto deal_with_return_code;
	}

upload_file_content:
	if (storagekey.empty())
	{
		storagekey = ByteArray::genRandom(32); 
	//	storagekey = ByteArray::fromHexStr("fe7b75abf84ce14d950f97543ad17ffd5757e45da23b85dba7cd634598e302f9");
		if (Enc_GZipAndEncFile(storagekey) != SD_SUCCESSED)
			return SD_FAILED;
	} else {
		//storagekey = ByteArray::fromHexStr(resume_info_.storageKey); 
		//file_.encFilePath = resume_info_.encPath;
		//file_.encFileSize = resume_info_.encSize;
		//file_.encSha1 = resume_info_.encSha1;
		file_.sharekey = EncryptStorageKeyToShareKey(file_.sha512, storagekey);
		file_.storagekey = storagekey.toUrlBase64Str();
		file_.personalkey = GeneratePersonalKey(storagekey);
	}

  /*
	if (file_.encFileSize >= BIG_FILE_THRESHHOLD)
	{
		//ret = Enc_UploadBigFile(node);
	} else {
	}
  */
  ret = Enc_UploadSmallFile(token,node);
	
	FileInfo::Remove(file_.encFilePath);
deal_with_return_code:	// deal with return code
	switch(ret){
	case SD_SUCCESSED:{
	printf("%s upload successed\r\n",file_.name.c_str());
	return ret;
	}
	default:
	  return SD_FAILED;
	}
}
SD_CODE FileUploaderEnc::Enc_CheckIfFileExist(const AccessToken &token){
	std::string response;
	SD_CODE ret = request_.Enc_CheckIfSameHashFileExists(token, file_.digest, response);
	switch(ret){
	case SD_OPENAPI_FILE_NOT_FOUND:{
		printf("%s need upload\r\n",file_.name.c_str());
		return SD_FILE_NEED_UPLOAD;}
	case SD_SUCCESSED:
		goto check_share_key_array;
	default:{
		printf("%s check file error\r\n",file_.name.c_str());
		return SD_FAILED;}
	}
check_share_key_array:
	printf("check share key array\r\n");
	/*CJsonParse parser(response);
	if (!parser.IsValid())
		return SVG_FAILED;
	std::vector<SameHashFileInfo> keys;
	if (parser.Enc_GetShareKeyArray(keys) == false)
		return SVG_FAILED;

	if (keys.empty())
		return SVG_FILE_NEED_UPLOAD;

	ByteArray sharekey_enc_key;
	ByteArray sha512 = ByteArray::fromHexStr(file_.sha512);
	sharekey_enc_key.insert(sharekey_enc_key.end(), sha512.begin()+16, sha512.begin()+32);
	sharekey_enc_key.insert(sharekey_enc_key.end(), sha512.begin()+48, sha512.end());		
*/
	return SD_FILE_NEED_UPLOAD;
}


SD_CODE FileUploaderEnc::Enc_UploadSmallFile(const AccessToken & token,TreeEntity &node)
{
	std::string response;
	SD_CODE ret = request_.Enc_UploadSmallFile(token, file_, response);
	switch (ret)
	{
	case SD_SUCCESSED:
		goto parse_uploal_result;
	default:
		return ret;
	}

parse_uploal_result:
	CJsonParse parser(response);
	if (!parser.IsValid())
		return SD_FAILED;

	if (parser.ParseNode(node) == false)
		return SD_FAILED;

	// TODO: update ent to database
	FileInfo::Remove(file_.encFilePath);
	return SD_SUCCESSED;
}

SD_CODE FileUploaderEnc::Enc_GZipAndEncFile(const ByteArray &storageKey)
{
	printf("enc_gzipandencfile start\r\n");
	bool ret;
	std::string md5 = file_.digest.substr(0, 32); // md5
	std::string tmpdir = TmpFile;	
	if(FileInfo::StatFile(tmpdir).type!=FileInfo::Directory){
	FileInfo::CreateDir(tmpdir);}	
	std::string aesfile = FileInfo::AppendPath(tmpdir,md5) + ".aes"; 
	std::string encdigest;
	std::string src_md5;
	ret = AESHelper::CompressAndEncFile(storageKey, file_.fullpath, aesfile, &encdigest, &src_md5);
	if (!ret) {
		printf("failed to compress and encrypt file: %s", file_.fullpath.c_str());
		FileInfo::Remove(aesfile);
		return SD_FAILED;
	}
	if (src_md5.empty() || src_md5 != file_.digest.substr(0, 32)) {
		printf("file changed when we encryting it: %s\n", file_.fullpath.c_str());
		FileInfo::Remove(aesfile);
		return SD_FAILED;
	}

	FileStatus st = FileInfo::StatFile(aesfile);
	if (st.type != FileInfo::Regular) {
		printf("StatFile() failed: %s", aesfile.c_str());
		FileInfo::Remove(aesfile);
		return SD_FAILED;
	}

	file_.encFilePath = aesfile;
	file_.encFileSize = st.size;
	file_.encSha1 = encdigest;
	file_.sharekey = EncryptStorageKeyToShareKey(file_.sha512, storageKey);
	file_.storagekey = UrlBase64Helper::Encode(storageKey);
	file_.personalkey = GeneratePersonalKey(storageKey);
	printf("enc_gzipandencfile surccessed\r\n");
	return SD_SUCCESSED;
}


std::string FileUploaderEnc::GeneratePersonalKey(const ByteArray &storage_key)
{
	std::string base64_storage_key = Base64Helper::Encode(storage_key);
	const ByteArray plain = ByteArray::fromRawStr(base64_storage_key);
	ByteArray cipher;

	if (file_.sharetype == SHARE_NONE || file_.sharetype == SHARE_PERSONAL) {
		if (ecies_ == NULL)
			return "";
		ecies_->Encrypt(plain, cipher);
	} 
	return cipher.toUrlBase64Str();
}

}}//namespace

