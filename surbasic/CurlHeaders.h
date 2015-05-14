#ifndef CURLHEADER_H
#define CURLHEADER_H
#include "surbasic/Type.h"
#include <curl/curl.h>
#include "surbasic/StrUtils.hpp"
#include "surbasic/File.h"
typedef void CURL;
struct curl_slist;
struct curl_httppost;
using namespace SDBasic::fmt;
using namespace SDBasic::file;
namespace SDBasic{
    namespace headers{

	struct PrefData
	{
	  int DownloadType;
	  int DownBandWidth;
	  int UploadType;
	  int UpBandWidth;
	};
	struct CurlProgressInfo
	{
		double dltotal;
		double dlnow;
		double dlspeed;
		double ultotal;
		double ulnow;
		double ulspeed;
		double upload_content_bytes;
		double download_content_bytes;
		void Reset() {          
			dltotal = 0.0; dlnow = 0.0; dlspeed = 0.0;
			ultotal = 0.0; ulnow = 0.0; ulspeed = 0.0;
			upload_content_bytes = 0.0;
			download_content_bytes = 0.0;
		}
	};

	struct FileUploadCtx {
	  File *file;
	  CurlProgressInfo *info;
	  FileUploadCtx(File *_file, CurlProgressInfo *_info) : file(_file), info(_info) {}
	};
		
	struct FileDownloadCtx {
		File *file;
		CurlProgressInfo *info;
	  FileDownloadCtx(File *_file, CurlProgressInfo *_info) : file(_file), info(_info) {}
	};

	
	typedef std::map<std::string, std::string> HttpHeaderMap;
	 HttpHeaderMap ParseHttpHeaders(const std::string &data);

	class  CurlHeaders
	{
	public:
		CurlHeaders(): headers(NULL) { }

		~CurlHeaders();

		void AddHeader(const char *name, const std::string &val);
		void AddHeader(const char *name,const long val);
		void AddHeader(const char *name,uint64_t val);

		curl_slist *GetList() const { return headers; }	

	private:
		curl_slist *headers;
	};
  
		

	class CurlHttpPost
	{
	public:
		CurlHttpPost();
		~CurlHttpPost();
		void AddTextField(const char *name, const std::string &val);
		void AddTextField(const char *name, uint64_t val);
		void AddFileField(const char *name, const std::string &filename, const std::string &path);
	  	void AddFileField(const char *name, const std::string &filename, int64_t size, FileUploadCtx *ctx);
		curl_httppost *data() const { return post; }
	private:
		bool fileadded;
		curl_httppost *post;
		curl_httppost *last;
	};

 }}//namespace
#endif
