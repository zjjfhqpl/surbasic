
#ifndef CURL_UTILS_H
#define CURL_UTILS_H
#include "surbasic/Type.h"
namespace SDBasic{
	namespace Utils{
 	
 		char dec2hexChar(short int n);
 		short int hexChar2dec(char c);
 		std::string escapeURL(const std::string &url);
 		std::string AppendClientTypeToUrl(const std::string& url, const char* name="linux");
 		std::string AppendUrl(const std::string &domain, const std::string &page);
 		void APPEND_X3W_FIELD(std::string &form, const std::string &name, const std::string &val, bool needEscape = true);
		void APPEND_X3W_FIELD(std::string &form, const std::string &name,long long val);

	}
}
#endif
