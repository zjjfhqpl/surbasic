
#ifndef SDCurl_H
#define SDCurl_H
#include<curl/curl.h>
#include "surbasic/Type.h"
#include "surbasic/nocopyable.hpp"
#include "surbasic/Public_Define.h"
#include "surbasic/StrUtils.hpp"
#include "surbasic/CurlHeaders.h"
#include "surbasic/Callback.h"
#include "surbasic/CJsonParse.h"
#include "surbasic/File.h"
#include "surbasic/BackupFile.h"
using namespace SDBasic::fmt;
using namespace SDBasic::headers;
using namespace SDBasic::code;
using namespace SDBasic::json;
using namespace SDBasic::file;
namespace SDBasic{
	namespace curl{
	typedef void CURLM;
	typedef void CURL;
		typedef int (*CURLWRITEFUNCTION)(void *ptr, size_t size, size_t nmemb, void* stream);
		typedef int (*CURLHEADERFUNCTION)(void *ptr, size_t size, size_t nmemb, void* stream);
		typedef int (*CURLPROGRESS)(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);

		class Request : public SDBasic::nocopyable
		{
		public:
		enum CurlType
		{
		  Upload = 0,
		  Download =1,
		  Timeout = 2,
		  Other = 3,
		};
		public:
			Request();
			~Request();
		public:
			void Init();
			void ReInit();
			void CleanUp();
		public:
			static void CURL_Init();
			static void CURL_UnInit();
			static void SetCurlPref(CURL * curl_handle,CurlType type = Other);
			void SetWriteFunction (CURLWRITEFUNCTION* pWriterFunction, void* data);
			void SetHeaderFunction( CURLHEADERFUNCTION* pHeaderFunction, void* data );
			bool Curl_connect(const std::string &url);
			void SetCurlBufferSize (long nSize);
			void SetLowSpeedTimeOut ( long nBytes, long nTime );
			void SetConnectTimeOut ( long nMSTime );
			void SetCurlNoProgress( bool bProgress );
			bool curl_connect(const std::string &url);
			void SetHttpHeaders(CurlHeaders* headers) ;
			typedef int (*ProgressFunc)(const CurlProgressInfo& info, void *userdata);
			typedef Callback<ProgressFunc> CurlProgressCallback;
			void SetProgressCallback(ProgressFunc func, void *data) { progress.func = func; progress.userdata = data; } 
			void ClearProgressCallback() { progress.func = NULL; progress.userdata = NULL; }
			//Enterprise	
			SD_CODE Ent_Login(const std::string& server,
							const std::string& account, const std::string& md5pwd, unsigned long timeout, std::string& response);
			SD_CODE Enc_CheckIfSameHashFileExists(const AccessToken &token, const std::string &hash, std::string &response);	
			//Personal
			SD_CODE Personal_Login(const std::string& server,
									const std::string& account, const std::string& md5pwd, unsigned long timeout, std::string& response);
			//
			SD_CODE OpenAPI_GetUserSpaceInfo(const AccessToken &token, std::string &response);
			SD_CODE Enc_RapidUpload(const AccessToken &token, BackupFile &file, std::string &response);
			SD_CODE Enc_UploadSmallFile(const AccessToken &token, BackupFile &file, std::string &response); 
			SD_CODE Enc_DownloadFile(const AccessToken &token,
				 const std::string &id, std::string &etag, int64_t offset, File *fp, std::string &personalKey);
			SD_CODE Enc_GetResumeAddress(const AccessToken &token, 
				 ShareType share_type,
				 const std::string& share_root_id,
				 std::string &location);
			SD_CODE Enc_GetResumeOffset(const AccessToken &token, 
				 const std::string &location, 
				 int64_t file_size, 
				 ShareType share_type,
				 const std::string& share_root_id,
				 int64_t &offset);
			SD_CODE Enc_ResumeUploadFile(const AccessToken &token, 
				const std::string &location, const BackupFile *file, std::string &response);
			SD_CODE Enc_GetDomainName(const string& server, 
				 const string& account, 
				 const string& plainpwd, 
				 const string& md5pwd, 
			 	unsigned long timeout, 
		 		string& response);

	 SD_CODE OpenAPI_CreateFolder(const AccessToken &token, 
                                const std::string &parentid, 
                                const std::string &name, std::string &response, 
                                OnDupOption ondup = RENAME);

	 SD_CODE OpenAPI_MoveFolder(const AccessToken &token, 
                              const std::string &id, 
                              const std::string &newParnetId, 
                              std::string &response);

	 SD_CODE OpenAPI_RenameFolder(const AccessToken &token, 
                                const std::string &id, 
                                const std::string &newName, 
                                std::string &response);

	 SD_CODE OpenAPI_DeleteFolder(const AccessToken &token, 
                                const std::string &id, 
                                bool recursive, 
                                std::string &response);

	 SD_CODE OpenAPI_RenameFile(const AccessToken &token, 
                              const std::string &id, 
                              const std::string &newName, 
                              std::string &response);

	 SD_CODE OpenAPI_DeleteFile(const AccessToken &token, 
                              const std::string &id, 
                              std::string &response);

   SD_CODE OpenAPI_MoveFile(const AccessToken &token, 
                            const std::string &id, 
                            const std::string &newParentId, 
                            std::string &response,
                            OnDupOption ondup);

		private:
			static	size_t g_WriteToString(void *ptr, size_t size, size_t nmemb, void *stream);
			static size_t g_UpLoadFileV2( void *ptr, size_t size, size_t nmemb, void *stream);
			static size_t g_WriteFileV2( void *ptr, size_t size, size_t nmemb, void *stream);
			static int g_CurlProgress(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
			void SetUrl(const std::string& url);
			void AppendShareParamsToUrl(ShareType type, const std::string& root_id, std::string* url);
			long GetHttpCode(CURL *handle);
			int DoCurlProgress(double dltotal, double dlnow, double ultotal, double ulnow);
			SD_CODE err_handle_openapi(CURLcode curlcode, long httpcode, const std::string &access_token, const std::string &response);
			SD_CODE err_handle(CURLcode& code, long sd);
		private:
			CURL * easy_handle;
			CURLcode resultcode; 		 
			CurlProgressCallback progress;
			CurlProgressInfo progInfo;
			bool enableProgLog;
		};

	}
}
#endif
