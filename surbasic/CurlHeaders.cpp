#include "surbasic/CurlHeaders.h"

#include <curl/curl.h>
#include "surbasic/StrUtils.hpp"

namespace SDBasic{
namespace headers{

static std::string TrimSpace(const std::string &str)
{
        std::string out = str;
        std::string::size_type pos = 0;
        while (!out.empty() && out[0] == ' ')
                out.erase(out.begin());

        while (!out.empty() && out[str.size()-1] == ' ')
                out.erase(--out.end());

        return out;
}
CurlHeaders::~CurlHeaders()
{
	if (headers != NULL)
		curl_slist_free_all(headers); 
}

void CurlHeaders::AddHeader(const char *name,uint64_t val)
{
	AddHeader(name, NumStr(val));
}

void CurlHeaders::AddHeader(const char *name,const long val)
{
	AddHeader(name, NumStr(val));
}

void CurlHeaders::AddHeader(const char *name, const std::string &val)
{
	std::string header = name;
	header += ": ";
	header += val;
	headers = curl_slist_append(headers, header.c_str());
}


CurlHttpPost::CurlHttpPost()
:post(NULL), last(NULL), fileadded(false)
{
}

CurlHttpPost::~CurlHttpPost()
{
	if (post != NULL)
		curl_formfree(post);
}

void CurlHttpPost::AddTextField(const char *name, const std::string &val)
{
	CURLFORMcode c = curl_formadd(&post, &last, CURLFORM_COPYNAME, name, 
		CURLFORM_COPYCONTENTS, val.c_str(), CURLFORM_END);

	assert (c == CURL_FORMADD_OK);
}

void CurlHttpPost::AddTextField(const char *name, uint64_t val)
{
	AddTextField(name, NumStr(val));
}

void CurlHttpPost::AddFileField(const char *name, const std::string &filename, const std::string &path)
{
	assert (fileadded == false);
	CURLFORMcode c = curl_formadd(&post, &last, CURLFORM_COPYNAME, name, 
		CURLFORM_FILENAME, filename.c_str(), 
		CURLFORM_FILE, path.c_str(), CURLFORM_END);
	assert (c == CURL_FORMADD_OK);
}

void CurlHttpPost::AddFileField(const char *name, const std::string &filename, 
                                int64_t size, FileUploadCtx *ctx) 
{
  assert (fileadded == false);
  CURLFORMcode c = curl_formadd(&post, &last, CURLFORM_COPYNAME, name, 
    CURLFORM_FILENAME, filename.c_str(), 
    CURLFORM_STREAM, ctx,
    CURLFORM_CONTENTSLENGTH, static_cast<long>(size),
    CURLFORM_END);
  assert (c == CURL_FORMADD_OK);
}


// 注意：当libcurl通过代理连接的时候，curl_easy_perform()之后头信息中实际
// // 包含两个连接的头信息：一个是代理的，一个是实际数据的
// /* 通过Filder代理下载文件得到的头部数据
// HTTP/1.1 200 Connection Established
// FiddlerGateway: Direct
// StartTime: 09:44:59.562
// Connection: close
//
// HTTP/1.1 206 Partial Content
// Server: Tengine/1.5.1
// Date: Thu, 28 Nov 2013 01:45:03 GMT
// Content-Type: application/octet-stream
// Transfer-Encoding: chunked
// Connection: keep-alive
// Content-Disposition: attachment;filename="QQPinyin_Setup_4.6.2029.400.exe.mp3";filename*=utf-8''QQPinyin_Setup_4.6.2029.400.exe.mp3
// Accept-Ranges: bytes
// ETag: e477f344ea7797905de31956db641497
// Last-Modified: Wed, 09 Oct 2013 06:09:03 GMT
// Expires: Thu, 05 Dec 2013 01:45:03 GMT
// Content-Range: bytes 0-28830295/28830296
// Content-Encoding: gzip
// Vary: Accept-Encoding
// */
HttpHeaderMap ParseHttpHeaders(const std::string &data)
{
	HttpHeaderMap out;
	std::string::size_type pos = 0;
	std::string::size_type last = 0;

	while (true)
	{
		pos = data.find("\r\n", last);
		if (pos == std::string::npos)
			return out;

		std::string oneline = data.substr(last, pos-last);
		last = pos + 2;

		std::string::size_type colon = oneline.find_first_of(':');
		if (colon == std::string::npos)
			continue;

		std::string header_name = TrimSpace(oneline.substr(0, colon));
		std::string header_val = TrimSpace(oneline.substr(colon+1));

		out.insert(make_pair(header_name, header_val));
	}
}

}}//namespace
