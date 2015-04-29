#include"SDCurl.h"

#include <curl/curl.h>

#include "surbasic/CurlUtils.h"
#include "surbasic/CurlHeaders.h"
#include "surbasic/Public_Define.h"
#include "surbasic/ServerErrorCode.h"
#include "surbasic/Log.hpp"
#include "surbasic/DmvAPI.h"
#include "surbasic/format.h"

using namespace SDBasic::curl;
using namespace SDBasic::Utils;
using namespace SDBasic::json;
using namespace SDBasic::fmt;
using namespace SDBasic::headers;
using namespace dmvservers::enterprise::v1;
using namespace SDBasic::file;
#define ENT_KEY 1
PrefData curlpref;
const char *SurDocAppKey = "51c7a970199a";    
const char *SurDocAppSecret = "f64f76571a2760a657456775915cf9bd";
const std::string SurDocClientType = "pacssurdoc";

static bool ParseErrorRespones(const std::string &response, ServerErrorCode *code, std::string *msg) {
	*code = INVALID;
	msg->clear();

	Json::Value v;
	Json::Reader r;
	if (!r.parse(response, v, false))
	{ printf("parse error.\r\n");
		return false;}

	if (!v["errorCode"].isIntegral() || !v["message"].isString()) 
	{printf("parse error.\r\n");	return false;}

	*code = (ServerErrorCode)v["errorCode"].asInt();
	*msg = v["message"].asString();
	printf("parse successed.\r\n");
	return true;
}
static const char *ConvertTypeStr(FileConvertType type)
{
#ifdef ENC_WITHOUT_STORAGE_KEY 
	return "false";
#else
	switch (type)
	{
	case DOC_AND_CONVERTABLE:
		return "true";
	default:
		return "false";
	}
#endif
}


static const char * OnDupOptionStr(OnDupOption opt)
{
	const char *names[] = 
	{
		"overwrite",
		"discard",
		"rename",
	};

	return names[opt];
};
Request::Request():enableProgLog(false),easy_handle(NULL)
{

}
Request::~Request()
{

}

void Request::CURL_Init(){
	curl_global_init(CURL_GLOBAL_ALL);//初始化全局libcurl 
}
void Request::CURL_UnInit(){
	curl_global_cleanup();//清理curl_global_init
}
void Request::Init(){//获取curl interface 指针
	if(easy_handle==NULL){
	easy_handle = curl_easy_init();}
	else{
	curl_easy_reset(easy_handle);}
//	::curl_easy_setopt(handle_, CURLOPT_VERBOSE, (enabled ? 1U : 0U));
	curl_easy_setopt(easy_handle, CURLOPT_VERBOSE,  1U);	
	curl_easy_setopt(easy_handle, CURLOPT_ENCODING, "");
	curl_easy_setopt(easy_handle, CURLOPT_LOW_SPEED_LIMIT, 1 );
	curl_easy_setopt(easy_handle, CURLOPT_LOW_SPEED_TIME, 60 * 3 );
	curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 0);
	curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, g_CurlProgress);
	curl_easy_setopt(easy_handle, CURLOPT_PROGRESSDATA, this);	
	curl_easy_setopt(easy_handle, CURLOPT_FORBID_REUSE, 1); 
	Request::SetCurlPref(easy_handle);
	curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYHOST, 1L);
	// curl_easy_setopt(easy_handle, CURLOPT_PROXY, "10.0.1.114:8888");
	curl_easy_setopt(easy_handle, CURLOPT_MAX_SEND_SPEED_LARGE, curl_off_t(50*1024));
	curl_easy_setopt( easy_handle, CURLOPT_MAX_RECV_SPEED_LARGE, curl_off_t(20*1024));	
}

int Request::g_CurlProgress(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	assert (clientp != NULL);
	Request	 *self = static_cast<Request*>(clientp);	
	return self->DoCurlProgress(dltotal, dlnow, ultotal, ulnow);
}

int Request::DoCurlProgress(double dltotal, double dlnow, double ultotal, double ulnow)
{
	progInfo.dltotal = dltotal;
	progInfo.dlnow = dlnow;
	progInfo.ultotal = ultotal;
	progInfo.ulnow = ulnow;
	if (curl_easy_getinfo(easy_handle, CURLINFO_SPEED_DOWNLOAD, &progInfo.dlspeed) != CURLE_OK)
		progInfo.dlspeed = 0.0;
	if (curl_easy_getinfo(easy_handle, CURLINFO_SPEED_UPLOAD, &progInfo.ulspeed) !=  CURLE_OK)
	progInfo.ulspeed = 0.0;

	
	if (enableProgLog)
	{
		if (ulnow != ultotal)
		{
		printf("CURL_PROG: D %.1lfKb/%.1lfKb/+%.1lfKb/s U %.1lfKb/%.1lfKb/+%.1lfKb/s", 
				dlnow/1000, dltotal/1000, progInfo.dlspeed/1000,
				ulnow/1000, ultotal/1000, progInfo.ulspeed/1000);
		} else {
		printf("CURL_PROG: D %.1lfKb/%.1lfKb/+%.1lfKb/s U %.1lfKb/%.1lfKb/done",
				dlnow/1000, dltotal/1000, progInfo.dlspeed/1000,
				ulnow/1000, ultotal/1000);
		}
		//lastProgLogTime.update();
	}

	if (progress.func != NULL)
	{
		int ret = progress.func(progInfo, progress.userdata);
		if (ret != 0)
			return CURLE_ABORTED_BY_CALLBACK;
	}

	return 0;
}


void Request::CleanUp ()
{
	if  ( easy_handle != NULL )
	{
		curl_easy_cleanup(easy_handle);	
		easy_handle = NULL;
	}
}

void Request::ReInit()
{
	curl_easy_reset(easy_handle);
	this->progInfo.Reset();
	this->Init();
}
void Request::SetCurlPref(CURL * curl_handle,CurlType type)
{
	if(type == Upload)
	{
	    switch(curlpref.DownloadType)
		{
		case 1:
		curl_easy_setopt(curl_handle,CURLOPT_MAX_RECV_SPEED_LARGE,curl_off_t(curlpref.DownBandWidth*1024));
		 break;
		default:
		 break;
		}
	}
	else if(type == Download)
	{
		switch (curlpref.UploadType)
		{
		case 1:
			curl_easy_setopt(curl_handle, CURLOPT_MAX_SEND_SPEED_LARGE, curl_off_t(curlpref.UpBandWidth*1024));
			break;
		default:
			break;
		}
		
	}
	else if(type == Timeout)
	{
		curl_easy_setopt ( curl_handle, CURLOPT_CONNECTTIMEOUT_MS, 30000);
		curl_easy_setopt ( curl_handle, CURLOPT_TIMEOUT_MS,30000);
	}
	
}

void Request::SetWriteFunction (CURLWRITEFUNCTION* pWriterFunction, void* data)
{
	if ( easy_handle != NULL )
	{
		curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, data );
		curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, pWriterFunction );
	}
}
void Request::SetHeaderFunction( CURLHEADERFUNCTION* pHeaderFunction, void* data )
{
	if ( easy_handle != NULL )
	{
		curl_easy_setopt (easy_handle, CURLOPT_WRITEHEADER, data );
		curl_easy_setopt (easy_handle, CURLOPT_HEADERFUNCTION, pHeaderFunction );
	}
}
void Request::SetCurlBufferSize (long nSize)
{
	if ( easy_handle != NULL )
	{
		curl_easy_setopt(easy_handle, CURLOPT_BUFFERSIZE, nSize );
	}
}
void Request::SetLowSpeedTimeOut ( long nBytes, long nTime )
{
	if ( easy_handle != NULL )
	{
		curl_easy_setopt(easy_handle, CURLOPT_LOW_SPEED_LIMIT, nBytes );
		curl_easy_setopt(easy_handle, CURLOPT_LOW_SPEED_TIME, nTime );
	}
}
void Request::SetConnectTimeOut ( long nMSTime )
{
	if ( easy_handle != NULL )
	{
		curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT_MS, nMSTime );
	}
}
void Request::SetCurlNoProgress( bool bProgress )
{
	if ( easy_handle != NULL )
	{
		curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, bProgress);
	}
}

bool Request::Curl_connect(const std::string &url)
{
	if(easy_handle==NULL)curl_easy_init();
	if(easy_handle==NULL)return false;
	curl_easy_setopt ( easy_handle, CURLOPT_URL, url.c_str());
	curl_easy_setopt ( easy_handle, CURLOPT_CONNECT_ONLY, 1L );
	CURLcode code = curl_easy_perform ( easy_handle );
	if ( code != CURLE_OK )
		printf("connection failed, error code:%s",curl_easy_strerror (code));
	return true;
}


void Request::SetUrl(const std::string& url_orig) {
	std::string url = AppendClientTypeToUrl(url_orig);
	curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
}

void Request::SetHttpHeaders(CurlHeaders* headers) {
	headers->AddHeader("User-Agent", "windows");
	headers->AddHeader("Client-Type", SurDocClientType);
	curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers->GetList());
}

size_t Request::g_WriteToString(void *ptr, size_t size, size_t nmemb, void *stream)
{

	int nsize = size * nmemb;
	std::string *strdata = (std::string*) stream;
	strdata->append((const char *)ptr, nsize);
	return nsize;
}

long Request::GetHttpCode(CURL *handle)
{
	long code = 0;
	curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &code);
	return code;
}

SD_CODE Request::Personal_Login(const std::string& server, 
							 const std::string& account, 
							 const std::string& md5pwd, 
							 unsigned long timeout, 
							 std::string& response)
{
	assert(easy_handle != NULL);
	assert(!server.empty());
	std::string url = AppendUrl(server, dmvservers::strcheckAccountsGetServer);
	SetUrl(url.c_str () );

	CurlHeaders headers;
	SetHttpHeaders(&headers);

	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &response);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, g_WriteToString);
	curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, timeout);

	std::string form;
	APPEND_X3W_FIELD(form, "account", account);
	APPEND_X3W_FIELD(form, "apwd", md5pwd);
	APPEND_X3W_FIELD(form, "app", "pacesever");
	APPEND_X3W_FIELD(form, "appkey", SurDocAppKey);
	APPEND_X3W_FIELD(form, "appsecret", SurDocAppSecret);
	curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, form.c_str());

	CURLcode code = curl_easy_perform(easy_handle);
	long httpcode = GetHttpCode(easy_handle);
	printf("\r\n Reuest:%s?%s \r\n code:%d, httpcode:%d Response:%s\r\n",url.c_str(),form.c_str(),code,httpcode,response.c_str());
	if (httpcode == 404) {
		ServerErrorCode code;
		std::string msg;
	if (ParseErrorRespones(response, &code, &msg)) {
			if (msg == ("user is not exist.")) {
				return SD_USER_NOT_EXIST;}
		 	else if (msg == ("password error.")) {
			return SD_PWD_ERROR;}
		 else {
			printf("Unknown login response: %s\r\n", response.c_str());
			return SD_FAILED;}}
	}//httpcode==404 :

        SD_CODE ret = err_handle_openapi(code, httpcode, "", response);
	return ret;
}

SD_CODE Request::Ent_Login(const std::string& server, 
							 const std::string& account, 
							 const std::string& md5pwd, 
							 unsigned long timeout, 
							 std::string& response)
{
	assert(easy_handle != NULL);
	assert(!server.empty());
	std::string url = AppendUrl(server, dmvservers::enterprise::strLogin);
	SetUrl(url.c_str () );

	CurlHeaders headers;
	SetHttpHeaders(&headers);

	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &response);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, g_WriteToString);
	curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, timeout);

	std::string form;
	APPEND_X3W_FIELD(form, "email", account);
	APPEND_X3W_FIELD(form, "password", md5pwd);
	APPEND_X3W_FIELD(form, "app", "pacesever");
	APPEND_X3W_FIELD(form, "appkey", SurDocAppKey);
	APPEND_X3W_FIELD(form, "appsecret", SurDocAppSecret);
	curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, form.c_str());

	CURLcode code = curl_easy_perform(easy_handle);
	long httpcode = GetHttpCode(easy_handle);
	printf("\r\n Reuest:%s?%s \r\n code:%d, httpcode:%d Response:%s\r\n",url.c_str(),form.c_str(),code,httpcode,response.c_str());
	if (httpcode == 404) {
		ServerErrorCode code;
		std::string msg;
	if (ParseErrorRespones(response, &code, &msg)) {
			if (msg == ("user is not exist.")) {
				return SD_USER_NOT_EXIST;}
		 	else if (msg == ("password error.")) {
			return SD_PWD_ERROR;}
		 else {
			printf("Unknown login response: %s\r\n", response.c_str());
			return SD_FAILED;}}
	}//httpcode==404 :

        SD_CODE ret = err_handle_openapi(code, httpcode, "", response);
	return ret;
}



SD_CODE Request::err_handle_openapi(CURLcode curlcode, 
                                      long httpcode, 
                                      const std::string &access_token, 
                                      const std::string &response)
{
	if (curlcode != CURLE_OK)
	{
		printf( "[%s] curl error: %s", __FUNCTION__, curl_easy_strerror(curlcode));
		if (curlcode == CURLE_SEND_ERROR && !response.empty()) {
		printf("abort by server: %d %s", httpcode, response.c_str());
			goto parse_http_code_and_response;
		}

		switch (curlcode)
		{
		case CURLE_COULDNT_CONNECT:
			return SD_CONNECT_SERVER_ERROR;
		case CURLE_COULDNT_RESOLVE_HOST:
			return SD_RESLOVED_HOST;
		case CURLE_OPERATION_TIMEDOUT:
			return SD_TIMEOUT;
		case CURLE_SEND_ERROR:
			return SD_SEND_ERROR;
		default:
			return SD_ERROR;
		}
	}

parse_http_code_and_response:
	if (httpcode == 200)
		return SD_SUCCESSED;

	if (httpcode == 204 && response.empty())
		return SD_OPENAPI_NO_CONTENT;

	if (httpcode == 308 && response.empty())
		return SD_OPENAPI_308;

#if ENT_KEY
	if (httpcode == 401) {
		printf("invalid_token, The access token provided is expired");
		//TokenManager::get().ForceRefreshToken(access_token);
		return SD_OPENAPI_INVALID_TOKEN;
	}

	if (httpcode == 507) {
		return SD_BACKUP_TOTAL_SPACE_LESS;
	}
#else
	if (!response.empty()) {
		
		Json::Value root;
		Json::Reader jsReader;
		bool is_json = jsReader.parse(response, root, false);
		if (!is_json)
			goto parse_error_by_httpcode;

		if (httpcode == 401) {
			printf( "oauth error: %s", response.c_str());
			int errorCode = root["errorCode"].asInt();
			if (errorCode == OAUTH_ERROR) {
				const std::string msg = root["message"].asString();
				if (stricmp(msg.c_str(), "invalid_token, The access token provided is expired.") == 0) {
					TokenManager::get().ForceRefreshToken(access_token);
					return SD_OPENAPI_INVALID_TOKEN;
				}
			}

			std::string error = root["error"].asString();
			if (error == "invalid_grant" || error == "invalid_token")
				return SD_OPENAPI_INVALID_TOKEN;
			return SD_ERROR_SERVER;
		}

		if (httpcode == 507) {
			int code = root["errorCode"].asInt();
			switch (code) {
			case OVER_STORAGE_QUOTA_TOTAL:
				return SD_BACKUP_TOTAL_SPACE_LESS;
			case OVER_STORAGE_QUOTA_NON_DOC:
				return SD_BACKUP_NON_DOC_SAPCE_LESS;
			case OVER_STORAGE_QUOTA_SINGLE_NON_DOC:
				return SD_BACKUP_FILE_10M_LIMIT;
			default:
				return SD_BACKUP_TOTAL_SPACE_LESS;
			}
		}
	}
#endif

parse_error_by_httpcode:
	printf("[%s] server returns http status code: %ld\n details:\n %s", 
		__FUNCTION__, httpcode, response.c_str());
	switch (httpcode)
	{
	case 304:
		return SD_OPENAPI_NOT_MODIFIED;
	case 400:
		return SD_OPENAPI_BAD_REQUESET;
	case 401:
		return SD_OPENAPI_INVALID_TOKEN;
	case 403:
		return SD_OPENAPI_INVALID_OAUTH_REQUEST;
	case 405:
		return SD_OPENAPI_METHOD_UNEXPECTED;
	case 409:
		return SD_OPENAPI_FILE_OR_DIR_EXISTS;
	case 404:
		return SD_OPENAPI_FILE_NOT_FOUND;
	case 507:
		return SD_BACKUP_FILE_LIMIT;
	default:
		return SD_SERVER_ERROR;
	}
}

void Request::AppendShareParamsToUrl(ShareType type, const std::string& root_id, std::string* url)
{
	std::string params;
	switch (type) {
		case SHARE_NONE:
			break;
		case SHARE_PERSONAL:
			assert (!root_id.empty());
			APPEND_X3W_FIELD(params, "shareFolders", "true");
			APPEND_X3W_FIELD(params, "rootShareFolderId", root_id);
			break;
		case SHARE_ENTERPRISE_DEPAREMENT:
			assert (!root_id.empty());
			APPEND_X3W_FIELD(params, "shareFolderId", root_id);
			break;
	}

	if (params.empty())
		return;

	if (url->find('?') == std::string::npos) {
		url->append("?");
	} else {
		url->append("&");
	}

	url->append(params);
}

SD_CODE Request::OpenAPI_GetUserSpaceInfo(const AccessToken &token, std::string &response)
{
	this->ReInit();
	assert (easy_handle != NULL);
	std::string url = AppendUrl(token.resource_server ,dmvservers::openapi::v1::strUserSpaceInfo);
	SetUrl(url);
	curl_easy_setopt(easy_handle, CURLOPT_HTTPGET, 1L);
	CurlHeaders headers;
	headers.AddHeader("Authorization", "Bearer " + token.access_token);
	SetHttpHeaders(&headers);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, g_WriteToString);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &response);
	CURLcode curlcode = curl_easy_perform(easy_handle);
	long httpcode = GetHttpCode(easy_handle);
	printf("server response: \nHTTP %ld \n%s", httpcode, response.c_str());
	SD_CODE code = err_handle_openapi(curlcode, httpcode, token.access_token, response);
	if (code == SD_SUCCESSED)
		return SD_SUCCESSED;
	return code;
}

SD_CODE Request::Enc_CheckIfSameHashFileExists(const AccessToken &token, const std::string &hash, std::string &response)
{
	assert (easy_handle != NULL);
	curl_easy_setopt(easy_handle, CURLOPT_ENCODING, "identity");

	// url && query string
	std::string url = AppendUrl(token.resource_server, dmvservers::openapi::v2::security::strIfHashExists);
	std::string query;
	APPEND_X3W_FIELD(query, "hash", hash);
	url += "?" + query;
	SetUrl(url);
	curl_easy_setopt(easy_handle, CURLOPT_HTTPGET, 1L);

	// oauth header
	CurlHeaders headers;
	headers.AddHeader("Authorization", "Bearer " + token.access_token);
	SetHttpHeaders(&headers);

	// write callback
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, g_WriteToString);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &response);

	 //perform
	CURLcode curlcode = curl_easy_perform(easy_handle);

	// return code
	long httpcode = GetHttpCode(easy_handle);
	this->ReInit();
	printf( "server response: \nHTTP %ld \n%s", httpcode, response.c_str());
	SD_CODE code = err_handle_openapi(curlcode, httpcode, token.access_token, response);
	if (code == SD_SUCCESSED)
		return SD_SUCCESSED;

	return code;
}

SD_CODE Request::Enc_RapidUpload(const AccessToken &token,BackupFile &file, std::string &response)
{

	//ScopedCurlDebugger debugger(this);	
	assert (easy_handle != NULL);
	assert (!file.parentid.empty());
	assert (!file.digest.empty());
	assert (!file.name.empty());
	assert (!file.encFilePath.empty());
	assert (!file.personalkey.empty());
	assert (!file.storagekey.empty());
	
	// url && query string
	std::string url; 
	if (file.sharetype == SHARE_NONE || file.sharetype == SHARE_PERSONAL) {
		url = AppendUrl(token.resource_server, dmvservers::openapi::v1::security::strRapidUpload);
	} else {
		url = AppendUrl(token.resource_server, department_share::strRapidUpload);
	}
	AppendShareParamsToUrl(file.sharetype, file.sharerootid, &url);
	SetUrl(url);

	// http x3w form	
	std::string form;
	APPEND_X3W_FIELD(form, "filename", file.name);
	APPEND_X3W_FIELD(form, "digest", file.digest);
	APPEND_X3W_FIELD(form, "encdigest", file.encSha1);
	APPEND_X3W_FIELD(form, "enckey", file.personalkey);
	APPEND_X3W_FIELD(form, "seckey", file.storagekey);
	APPEND_X3W_FIELD(form, "sharekey", file.sharekey);
	APPEND_X3W_FIELD(form, "filesize", file.size);
	APPEND_X3W_FIELD(form, "encfilesize", file.encFileSize);
	APPEND_X3W_FIELD(form, "convert", ConvertTypeStr(file.convert));
	APPEND_X3W_FIELD(form, "parentId", file.parentid);
	APPEND_X3W_FIELD(form, "ondup", OnDupOptionStr(file.ondup));
#ifdef STANDARD_LIVE
	if (IfPicType(file.fileName))
	{
		APPEND_X3W_FIELD(form, "dimension", MAX_THUMBNAIL_SIZE);			//略缩图大小
	}
#endif
	//APPEND_X3W_FIELD(form, "createTime", file.modifyTime.toISO8601());
	curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, form.c_str());

	// oauth header
	CurlHeaders headers;
	headers.AddHeader("Authorization", "Bearer " + token.access_token);
	SetHttpHeaders(&headers);

	 //write callback
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION,g_WriteToString);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &response);

	 //perform
	CURLcode curlcode = curl_easy_perform(easy_handle);

	 //return code
	long httpcode = GetHttpCode(easy_handle);
	printf("server response: \nHTTP %ld \n%s", httpcode, response.c_str());
	SD_CODE code = err_handle_openapi(curlcode, httpcode, token.access_token, response);
	if (code == SD_SUCCESSED)
		return SD_SUCCESSED;

	return code;
}


size_t Request::g_UpLoadFileV2( void *ptr, size_t size, size_t nmemb, void *stream)
{
//	DWORD dwRet = WaitForSingleObject ( m_hUpLoadStopEvent, 0 );
//	if ( dwRet == WAIT_OBJECT_0 )
//		return CURL_READFUNC_ABORT;

  FileUploadCtx *ctx = static_cast<FileUploadCtx*>(stream);
	unsigned long read_bytes = 0;
	bool s = ctx->file->Read(ptr, size * nmemb, &read_bytes);
	if (s) {
    ctx->info->upload_content_bytes += read_bytes;
		return read_bytes;
	} else {
		return -1;
	}
}
SD_CODE Request::Enc_UploadSmallFile(const AccessToken &token, BackupFile &file, std::string &response)
{

	assert (easy_handle != NULL);
	assert (!file.parentid.empty());
	assert (!file.digest.empty());
	assert (!file.name.empty());
	assert (!file.encFilePath.empty());
	assert (!file.personalkey.empty());
	assert (!file.storagekey.empty());

	File fp;
	bool blret = fp.OpenForRead(file.encFilePath);
	if (!blret) 
		return SD_LOCAL_FILE_ERROR;	

	 //url && query string
	std::string url;
	if (file.sharetype == SHARE_NONE || file.sharetype == SHARE_PERSONAL) {
		//ent //url = AppendUrl(token.resource_server, dmvservers::openapi::v1::security::strUploadSmall);
		url = AppendUrl(token.resource_server, dmvservers::openapi::v2::security::strUploadSmall);
	} else {
		url = AppendUrl(token.resource_server, department_share::strUploadSmall);
	}
	AppendShareParamsToUrl(file.sharetype, file.sharerootid, &url);
	SetUrl(url);

	// oauth header
	CurlHeaders headers;
	headers.AddHeader("Authorization", "Bearer " + token.access_token);
	headers.AddHeader("content-filesize", NumFmt("%" PRIi64, file.size).str());
	SetHttpHeaders(&headers);

	 //write callback
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, g_WriteToString);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &response);

	// http multipart form	
	CurlHttpPost post;
	post.AddTextField("filename", file.name);
	post.AddTextField("digest", file.digest);
	post.AddTextField("encdigest", file.encSha1);
	post.AddTextField("enckey", file.personalkey);
	post.AddTextField("sharekey", file.sharekey);
 	 post.AddTextField("seckey", file.storagekey);
	post.AddTextField("filesize", file.size);
	post.AddTextField("encfilesize", file.encFileSize);
	post.AddTextField("convert", ConvertTypeStr(file.convert));
	post.AddTextField("parentId", file.parentid);
	post.AddTextField("ondup", OnDupOptionStr(file.ondup));
#ifdef STANDARD_LIVE
	if (IfPicType(file.fileName))
	{
		post.AddTextField("dimension", MAX_THUMBNAIL_SIZE);	//略缩图大小
	}
#endif
	post.AddTextField("createTime", file.modifytime.ToISO8601());

  FileUploadCtx ctx(&fp, &progInfo);
	post.AddFileField("file", file.name, file.encFileSize, &ctx);
	curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, g_UpLoadFileV2);
	curl_easy_setopt(easy_handle, CURLOPT_HTTPPOST, post.data());

	curl_easy_setopt(easy_handle, CURLOPT_LOW_SPEED_TIME, 20*60); 
	curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, 30L);
	curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 7*24*3600L); 
//	 perform

//	curl_easy_setopt(easy_handle, CURLOPT_PROXY, "10.0.1.114:8888");
  // curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 1UL);
	CURLcode curlcode = curl_easy_perform(easy_handle);

	fp.Close();

//	 return code
	long httpcode = GetHttpCode(easy_handle);
printf( "server response: \nHTTP %ld \n%s", httpcode, response.c_str());
	SD_CODE code = err_handle_openapi(curlcode, httpcode, token.access_token, response);
	if (code == SD_SUCCESSED)
		return SD_SUCCESSED;

	return code;
}

size_t Request::g_WriteFileV2( void *ptr, size_t size, size_t nmemb, void *stream) {
	FileDownloadCtx *ctx = static_cast<FileDownloadCtx*>(stream);
	DWORD written_bytes = 0;
	bool s = ctx->file->Write(ptr, size * nmemb, &written_bytes);
	if (s) {
		ctx->info->download_content_bytes += written_bytes;
		return written_bytes;
	} else {
		return -1;
	}
}

SD_CODE Request::err_handle(CURLcode& code, long sd)
{
	if ( code != CURLE_OK )
	{
		printf( "err_handle %s", curl_easy_strerror ( code ) );

		if ( code == CURLE_COULDNT_CONNECT )
		{
			return SD_FAILED_CONNECT_SERVER;
		}
		if ( code == CURLE_URL_MALFORMAT 
			|| code == CURLE_COULDNT_RESOLVE_HOST )
		{
			return SD_RESOLVE_HOST;
		}
		if ( code == CURLE_OPERATION_TIMEDOUT )
			return SD_BACKUP_TIMEOUT;
		return SD_ERROR;
	}

	if  ( sd != 200 )
	{
		printf( "err_handle server returns non-200 http code: %ld", sd);
		return SD_SERVER_ERROR;
	}
	return SD_SUCCESSED;
}
SD_CODE Request::Enc_DownloadFile(const AccessToken &token, const std::string &id, std::string &etag, int64_t offset, File *fp, std::string &personalKey)
{
	
	assert (easy_handle != NULL);
	assert (!id.empty());
	assert (fp !=  NULL && fp->IsOpened());

	// url && query string
	//std::string url = AppendUrl(token.resource_server, dmvservers::openapi::v1::security::strDownload);
	
	std::string url = AppendUrl(token.resource_server, dmvservers::openapi::v2::security::strDownload);
	url += "/" + id + "/content";
	SetUrl(url);
	curl_easy_setopt(easy_handle, CURLOPT_HTTPGET, 1L);

	// oauth header
	CurlHeaders headers;
	headers.AddHeader("Authorization", "Bearer " + token.access_token);
	if (offset > 0) {
		headers.AddHeader("Range", NumFmt("bytes=%" PRId64 "-", offset).str());
	}
	if (!etag.empty())
		headers.AddHeader("If-Match", etag);
	SetHttpHeaders(&headers);

//	 body write back
	FileDownloadCtx ctx(fp, &this->progInfo);
	curl_easy_setopt(easy_handle, CURLOPT_FAILONERROR, 1L);//don't write to file if non-200
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &ctx);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, g_WriteFileV2);

	//header write callback
	std::string response_headers_str;
	curl_easy_setopt(easy_handle, CURLOPT_WRITEHEADER, &response_headers_str);
	curl_easy_setopt(easy_handle, CURLOPT_HEADERFUNCTION, g_WriteToString);

//	 perform
	CURLcode curlcode = curl_easy_perform(easy_handle);

//	 return code
	long httpcode = GetHttpCode(easy_handle);
	if (httpcode == 206)
		httpcode = 200;
	SD_CODE retcode = err_handle(curlcode, httpcode);

	HttpHeaderMap response_headers = ParseHttpHeaders(response_headers_str);
	if (response_headers.find("ETag") != response_headers.end())
	{
		std::string oldetag = etag;
		etag = response_headers["ETag"];
		if (!oldetag.empty() && etag != oldetag)
			return SD_OPENAPI_SERVER_FILE_CHANGED;
	}

	if (response_headers.find("ENC-KEY") != response_headers.end()) {
		personalKey = response_headers["ENC-KEY"];
	}

	if (retcode == SD_SUCCESSED)
		return SD_SUCCESSED;

//	 file changed after last downloading
	if (httpcode == 412)
		return SD_OPENAPI_SERVER_FILE_CHANGED;

	if (httpcode == 416)
		return SD_OPENAPI_SERVER_FILE_CHANGED;

	return retcode;
}

	
SD_CODE Request::Enc_GetResumeAddress(const AccessToken &token,ShareType share_type,const std::string& share_root_id,std::string &location)
{
	this->ReInit();
	assert (easy_handle != NULL);

	std::string url;
	if (share_type == SHARE_NONE || share_type == SHARE_PERSONAL) {
		url = AppendUrl(token.resource_server, dmvservers::openapi::v1::security::strResumeAddr);
	} else {
		url = AppendUrl(token.resource_server, department_share::strResumeAddr);
	}
	AppendShareParamsToUrl(share_type, share_root_id, &url);
	SetUrl(url);

	 //force POST method
	curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, ""); 

	 //oauth header
	CurlHeaders headers;
	headers.AddHeader("Authorization", "Bearer " + token.access_token);
	SetHttpHeaders(&headers);

	 //write callback
	std::string response;
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, g_WriteToString);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &response);

	// header write callback
	std::string response_headers_str;
	curl_easy_setopt(easy_handle, CURLOPT_WRITEHEADER, &response_headers_str);
	curl_easy_setopt(easy_handle, CURLOPT_HEADERFUNCTION, g_WriteToString);

	// perform
	CURLcode curlcode = curl_easy_perform(easy_handle);

	// return code
	long httpcode = GetHttpCode(easy_handle);
	printf( "server response: \nHTTP %ld \n%s", httpcode, response.c_str());
	SD_CODE code = err_handle_openapi(curlcode, httpcode, token.access_token, response);
	if (code != SD_SUCCESSED)
		return code;

	HttpHeaderMap response_headers = ParseHttpHeaders(response_headers_str);
	if (response_headers.find("Location") == response_headers.end())
		return SD_FAILED;

	location = response_headers["Location"];
	if (location.empty())
		return SD_FAILED;
	return SD_SUCCESSED;
}


SD_CODE Request::Enc_GetResumeOffset(const AccessToken &token,  const std::string &location,
		 int64_t file_size,  ShareType share_type, const std::string& share_root_id, int64_t &offset)
{
	this->ReInit();
//	ScopedCurlDebugger debugger(this);
	assert (easy_handle != NULL);
	assert (!location.empty());
	assert (file_size >= 0);

//	 url
	std::string url = location;
	AppendShareParamsToUrl(share_type, share_root_id, &url);
	SetUrl(url);

//	 force PUT method
	curl_easy_setopt(easy_handle, CURLOPT_CUSTOMREQUEST, "PUT");

//	 oauth header
	CurlHeaders headers;
	headers.AddHeader("Authorization", "Bearer " + token.access_token);
	headers.AddHeader("Content-Length", "0");
	headers.AddHeader("Content-Range", NumFmt("bytes */%ld", file_size).str()); 
	SetHttpHeaders(&headers);

//	 write callback
	std::string response;
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, g_WriteToString);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &response);

//	 header write callback
	std::string response_headers_str;
	curl_easy_setopt(easy_handle, CURLOPT_WRITEHEADER, &response_headers_str);
	curl_easy_setopt(easy_handle, CURLOPT_HEADERFUNCTION, g_WriteToString);

//	 perform
	CURLcode curlcode = curl_easy_perform(easy_handle);

	 //return code
	long httpcode = GetHttpCode(easy_handle);
	if (httpcode == 503) {
		ServerErrorCode code;
		std::string msg;
		if (ParseErrorRespones(response, &code, &msg)) {
			if (code == IO_ERROR/* && msg == "The file is being written."*/) {
				return SD_FILE_BEING_WRITTING;
			}
		}
	}

	printf( "server response: \nHTTP %ld \n%s", httpcode, response.c_str());
	SD_CODE code = err_handle_openapi(curlcode, httpcode, token.access_token, response);
	if (code != SD_OPENAPI_308)
		return SD_FAILED;

	HttpHeaderMap response_headers = ParseHttpHeaders(response_headers_str);
	if (response_headers.find("Offset") == response_headers.end())
		return SD_FAILED;

	std::string offset_str = response_headers["Offset"];
	offset =  atoll(offset_str.c_str());
	return SD_SUCCESSED;
}


SD_CODE Request::Enc_ResumeUploadFile(const AccessToken &token, const std::string &location, const BackupFile *file, std::string &response)
{
	this->ReInit();
	//DEFINE_FUNC_TIMER;
	//SET_INTERFACE_NAME;
	//ScopedCurlDebugger debugger(this);
	assert (easy_handle != NULL);
	assert (!location.empty());
	assert (!file->parentid.empty());
	assert (!file->digest.empty());
	assert (!file->name.empty());
	assert (!file->encFilePath.empty());
	assert (file->offset >= 0);
	assert (!file->personalkey.empty());
	assert (!file->storagekey.empty());

	File fp;
	bool blret = fp.OpenForRead(file->encFilePath);
	if (!blret) 
		return SD_LOCAL_FILE_ERROR;	

	blret = fp.SeekTo(File::FROM_BEGIN, file->offset);
	if (!blret) 
		return SD_LOCAL_FILE_ERROR;

	std::string url = location;
	AppendShareParamsToUrl(file->sharetype, file->sharerootid, &url);

	APPEND_X3W_FIELD(url, "filename", file->name);
	APPEND_X3W_FIELD(url, "parentId", file->parentid);
	APPEND_X3W_FIELD(url, "digest", file->digest);
	APPEND_X3W_FIELD(url, "filesize", file->size);
	APPEND_X3W_FIELD(url, "ondup", OnDupOptionStr(file->ondup));
	APPEND_X3W_FIELD(url, "convert", ConvertTypeStr(file->convert));
	APPEND_X3W_FIELD(url, "encdigest", file->encSha1);
	APPEND_X3W_FIELD(url, "enckey", file->personalkey);
	APPEND_X3W_FIELD(url, "seckey", file->storagekey);
	APPEND_X3W_FIELD(url, "sharekey", file->sharekey);
	APPEND_X3W_FIELD(url, "encfilesize", file->encFileSize);
#ifdef STANDARD_LIVE
	if (IfPicType(file->fileName))
	{
	APPEND_X3W_FIELD(url, "dimension", MAX_THUMBNAIL_SIZE);			//略缩图大小
	}
#endif
	APPEND_X3W_FIELD(url, "createTime", file->modifytime.ToISO8601());

	SetUrl(url);

//	 PUT: file upload
  	FileUploadCtx ctx(&fp, &this->progInfo);
	curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(easy_handle, CURLOPT_READDATA, &ctx);
	curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, g_UpLoadFileV2);
	curl_easy_setopt(easy_handle, CURLOPT_INFILESIZE_LARGE, file->encFileSize - file->offset);

//	 request headers
	CurlHeaders headers;
	headers.AddHeader("Authorization", "Bearer " + token.access_token);
	char content_range[256];
	snprintf(content_range, sizeof(content_range), "bytes %ld-%ld/%ld",
		file->offset, file->encFileSize-1, file->encFileSize);
	headers.AddHeader("Content-Range", content_range);
	headers.AddHeader("Content-Type", "application/octet-stream");
	SetHttpHeaders(&headers);

//	 ensure timeout settings for big file uploading
	curl_easy_setopt(easy_handle, CURLOPT_LOW_SPEED_TIME, 20*60); 
	curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, 30L);
	curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 7*24*3600L); 

//	 write callback
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, g_WriteToString);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &response);

//	 perform
//#if defined(DEBUG) || defined(_DEBUG)
//	curl_easy_setopt(easy_handle, CURLOPT_MAX_SEND_SPEED_LARGE, curl_off_t(100*1024));
//	curl_easy_setopt(easy_handle, CURLOPT_PROXY, "");
//#endif
	CURLcode curlcode = curl_easy_perform(easy_handle);
//#if defined(DEBUG) || defined(_DEBUG)
//	curl_easy_setopt(easy_handle, CURLOPT_PROXY, "127.0.0.1:8888");
//#endif

	fp.Close();

	// return code
	long httpcode = GetHttpCode(easy_handle);
	printf( "server response: \nHTTP %ld \n%s", httpcode, response.c_str());
	SD_CODE code = err_handle_openapi(curlcode, httpcode, token.access_token, response);

	return code;
}

SD_CODE Request::OpenAPI_CreateFolder(const AccessToken &token, 
                                      const std::string &parentid, 
                                      const std::string &name, std::string &response,
                                      OnDupOption ondup)
{
	assert (easy_handle != NULL);
	this->ReInit();

	// url && query string
	std::string url = "";
  url = AppendUrl(token.resource_server ,dmvservers::openapi::v2::strFolders);	

	// query string
	std::string query;
	APPEND_X3W_FIELD(query, "ondup", OnDupOptionStr(ondup));

	// join url and query string
	url += "?";
	url += query;
	AppendShareParamsToUrl(SHARE_NONE, "", &url);
	SetUrl(url);

	// oauth header
	CurlHeaders headers;
	headers.AddHeader("Authorization", "Bearer " + token.access_token);
	headers.AddHeader("Content-Type", "application/json");
	SetHttpHeaders(&headers);

	// write callback
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, g_WriteToString);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &response);

	// json requeset body
	Json::Value body;
	body["name"] = name;
	body["type"] = "folder";
	body["parent"]["type"] = "folder";
	body["parent"]["id"] = parentid;
	std::string data = body.toStyledString();
	curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, data.c_str());
	curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, data.length());

	// perform
	CURLcode curlcode = curl_easy_perform(easy_handle);

	// return code
	long httpcode = GetHttpCode(easy_handle);
	SD_CODE code = err_handle_openapi(curlcode, httpcode, token.access_token, response);
	if (code == SD_SUCCESSED)
		return SD_SUCCESSED;

	return code;
}


SD_CODE Request::OpenAPI_MoveFolder(const AccessToken &token, 
                                    const std::string &id, 
                                    const std::string &newParnetId, 
                                    std::string &response)
{
  assert (easy_handle != NULL);
  this->ReInit();

  // url && query string
  std::string url = "";
  url = AppendUrl(token.resource_server, dmvservers::openapi::v2::strFolders) 
      + "/" + id;
  url += "/move";

  // query string
  std::string query;
  APPEND_X3W_FIELD(query, "ondup", "1");

  // join url and query string
  url += "?";
  url += query;
  AppendShareParamsToUrl(SHARE_NONE, "", &url);
  SetUrl(url);
  //url+="&ondup=rename";

  // oauth header
  CurlHeaders headers;
  headers.AddHeader("Authorization", "Bearer " + token.access_token);
  headers.AddHeader("Content-Type", "application/json");
  SetHttpHeaders(&headers);

  // write callback
  curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, g_WriteToString);
  curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &response);

  // json requeset body
  Json::Value body;
  body["type"] = "folder";
  body["id"] = newParnetId;
  curl_easy_setopt(easy_handle, CURLOPT_CUSTOMREQUEST, "POST");

  std::string data = body.toStyledString();
  curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, data.c_str());
  curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, data.length());

  // perform
  CURLcode curlcode = curl_easy_perform(easy_handle);

  // return code
  long httpcode = GetHttpCode(easy_handle);
  SD_CODE code = err_handle_openapi(curlcode, httpcode, token.access_token, response);
  if (code == SD_SUCCESSED)
    return SD_SUCCESSED;

  return code;
}

SD_CODE Request::OpenAPI_RenameFolder(const AccessToken &token, 
                                      const std::string &id, 
                                      const std::string &newName, 
                                      std::string &response)
{
  assert (easy_handle != NULL);
  this->ReInit();

  // url && query string
  std::string url = "";
  url = AppendUrl(token.resource_server, dmvservers::openapi::v2::strFolders);	
  url = AppendUrl(url, id);

  AppendShareParamsToUrl(SHARE_NONE, "", &url); 
  SetUrl(url);

  // oauth header
  CurlHeaders headers;
  headers.AddHeader("Authorization", "Bearer " + token.access_token);
  headers.AddHeader("Content-Type", "application/json");
  SetHttpHeaders(&headers);

  // write callback
  curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, g_WriteToString);
  curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &response);

  // json requeset body
  Json::Value body;
  body["type"] = "folder";
  body["name"] = newName;
  std::string data = body.toStyledString();
  curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, data.c_str());
  curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, data.length());
  curl_easy_setopt(easy_handle, CURLOPT_CUSTOMREQUEST, "PUT");

  // perform
  CURLcode curlcode = curl_easy_perform(easy_handle);

  // return code
  long httpcode = GetHttpCode(easy_handle);
  SD_CODE code = err_handle_openapi(curlcode, httpcode, token.access_token, response);
  if (code == SD_SUCCESSED)
    return SD_SUCCESSED;

  return code;
}

SD_CODE Request::OpenAPI_DeleteFolder(const AccessToken &token, 
                                      const std::string &id, 
                                      bool recursive, 
                                      std::string &response)
{
  assert (easy_handle != NULL);
  this->ReInit();

  // url && query string
  std::string url = "";
  url = AppendUrl(token.resource_server, dmvservers::openapi::v2::strFolders);	
  url = AppendUrl(url, id);

  std::string query;
  APPEND_X3W_FIELD(query, "recursive", recursive?"true":"false");
  url += "?" + query;
  AppendShareParamsToUrl(SHARE_NONE, "", &url);
  SetUrl(url);

  // oauth header
  CurlHeaders headers;
  headers.AddHeader("Authorization", "Bearer " + token.access_token);
  SetHttpHeaders(&headers);

  // "DELETE" method
  curl_easy_setopt(easy_handle, CURLOPT_CUSTOMREQUEST, "DELETE");

  // write callback
  curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, g_WriteToString);
  curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &response);

  // perform
  CURLcode curlcode = curl_easy_perform(easy_handle);

  // return code
  long httpcode = GetHttpCode(easy_handle);
  SD_CODE code = err_handle_openapi(curlcode, httpcode, token.access_token, response);
  if (code == SD_SUCCESSED)
    return SD_SUCCESSED;

  switch (code) {
    case SD_OPENAPI_NO_CONTENT:
    case SD_OPENAPI_FILE_NOT_FOUND:
      return SD_SUCCESSED;
    case SD_OPENAPI_NOT_MODIFIED:
      return SD_FAILED;
    default:
      break; 
  }

  return code;
}

SD_CODE Request::OpenAPI_RenameFile(const AccessToken &token, 
                                    const std::string &id, 
                                    const std::string &newName, 
                                    std::string &response)
{
	assert (easy_handle != NULL);
	assert (!id.empty());
	assert (!newName.empty());

	// url && query string
	std::string url = "";
		url = AppendUrl(token.resource_server, dmvservers::openapi::v2::strFiles) + "/" + id;
	AppendShareParamsToUrl(SHARE_NONE, "", &url);
	SetUrl(url);

	// json body
	Json::Value body;
	body["name"] = newName;
	body["type"] = "file";
	std::string data = body.toStyledString();
	curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, data.c_str());
	curl_easy_setopt(easy_handle, CURLOPT_CUSTOMREQUEST, "PUT");

	// oauth header
	CurlHeaders headers;
	headers.AddHeader("Authorization", "Bearer " + token.access_token);
	headers.AddHeader("Content-Type", "application/json");
	SetHttpHeaders(&headers);

	// write callback
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, g_WriteToString);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &response);

	// perform
	CURLcode curlcode = curl_easy_perform(easy_handle);

	// return code
	long httpcode = GetHttpCode(easy_handle);
	SD_CODE code = err_handle_openapi(curlcode, httpcode, token.access_token, response);
	if (code == SD_SUCCESSED)
		return SD_SUCCESSED;

	return code;
}


SD_CODE Request::OpenAPI_DeleteFile(const AccessToken &token, 
                                   const std::string &id, 
                                   std::string &response)
{
  assert (easy_handle != NULL);
  assert (!id.empty());

  // url && query string
  std::string url = "";
  url = url = AppendUrl(token.resource_server, dmvservers::openapi::v2::strFiles);
  url = AppendUrl(url, id);

  AppendShareParamsToUrl(SHARE_NONE, "", &url);
  SetUrl(url);

  // oauth header
  CurlHeaders headers;
  headers.AddHeader("Authorization", "Bearer " + token.access_token);
  SetHttpHeaders(&headers);

  // "DELETE" method
  curl_easy_setopt(easy_handle, CURLOPT_CUSTOMREQUEST, "DELETE");

  // write callback
  curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, g_WriteToString);
  curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &response);

  // perform
  CURLcode curlcode = curl_easy_perform(easy_handle);

  // return code
  long httpcode = GetHttpCode(easy_handle);
  SD_CODE code = err_handle_openapi(curlcode, httpcode, token.access_token, response);
  if (code == SD_SUCCESSED)
    return SD_SUCCESSED;

  switch (code) {
    case SD_OPENAPI_NO_CONTENT:
    case SD_OPENAPI_FILE_NOT_FOUND:
      return SD_SUCCESSED;
    case SD_OPENAPI_NOT_MODIFIED:
      return SD_FAILED;
    default:
      break;
  }

  return code;
}

SD_CODE Request::OpenAPI_MoveFile(const AccessToken &token, 
                                  const std::string &id, 
                                  const std::string &newParentId, 
                                  std::string &response,
                                  OnDupOption ondup)
{
	assert (easy_handle != NULL);
	assert (!id.empty());
	assert (!newParentId.empty());

	// url: https://server/api/v1/files/{fileId}/copy?ifSameName=1
	//ifSameName optional 目标目录存在同名文件时的操作方式：
	//   1 重命名
	//   2 替换(默认)
	//   3 保留时间较新的文件

	// url && query string
	std::string url = "";
  url = AppendUrl(token.resource_server, dmvservers::openapi::v2::strFiles) + "/" + id;
  url += "/move";

	std::string query;
	APPEND_X3W_FIELD(query, "ondup", OnDupOptionStr(ondup));

	url += "?";
	url += query;
	AppendShareParamsToUrl(SHARE_NONE, "", &url);
	SetUrl(url);

	// json body
	Json::Value body;
	body["id"] = newParentId;
	body["type"] = "folder";
	std::string data = body.toStyledString();
	curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, data.c_str());

	// oauth header
	CurlHeaders headers;
	headers.AddHeader("Authorization", "Bearer " + token.access_token);
	headers.AddHeader("Content-Type", "application/json");
	SetHttpHeaders(&headers);

	// write callback
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, g_WriteToString);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &response);

	// perform
	CURLcode curlcode = curl_easy_perform(easy_handle);

	// return code
	long httpcode = GetHttpCode(easy_handle);
	SD_CODE code = err_handle_openapi(curlcode, httpcode, token.access_token, response);
	if (code == SD_SUCCESSED)
		return SD_SUCCESSED;

	return code;
}

