#include "surbasic/CurlUtils.h"
#include "surbasic/StrUtils.hpp"
using namespace SDBasic::fmt;
using namespace SDBasic::Utils;
namespace SDBasic{
	namespace Utils{
std::string AppendUrl(const std::string &domain, const std::string &page)
{
	assert (!domain.empty());
	assert (!page.empty());

	if (domain[domain.length()-1] == '/')
		return domain + page;
	else
		return domain + "/" + page;
}

char dec2hexChar(short int n) {
	if ( 0 <= n && n <= 9 ) {
		return char( short('0') + n );
	} else if ( 10 <= n && n <= 15 ) {
		return char( short('A') + n - 10 );
	} else {
		return char(0);
	}
}

short int hexChar2dec(char c) {
	if ( '0'<=c && c<='9' ) {
		return short(c-'0');
	} else if ( 'a'<=c && c<='f' ) {
		return ( short(c-'a') + 10 );
	} else if ( 'A'<=c && c<='F' ) {
		return ( short(c-'A') + 10 );
	} else {
		return -1;
	}
}

std::string escapeURL(const std::string &url)
{
	std::string result = "";
	for ( unsigned int i=0; i<url.size(); i++ ) {
		char c = url[i];
		// check whether is unreserved character
		// see http://tools.ietf.org/html/rfc3986#section-2.3
		if ( 
			( '0'<=c && c<='9' ) ||
			( 'a'<=c && c<='z' ) ||
			( 'A'<=c && c<='Z' ) ||
			c=='-' || c=='.' || c == '_' || c == '~' 
			) {
				result += c;
		} else {
			int j = (short int)c;
			if ( j < 0 ) {
				j += 256;
			}
			int i1, i0;
			i1 = j / 16;
			i0 = j - i1*16;
			result += '%';
			result += dec2hexChar(i1);
			result += dec2hexChar(i0); 
		}
	}
	return result;
}
void APPEND_X3W_FIELD(std::string &form, const std::string &name, const std::string &val, bool needEscape)
{
	if (!form.empty())
		form += "&";

	form += name;
	form += "=";
	form += (needEscape ? ::escapeURL(val.c_str()) : val);
}


void APPEND_X3W_FIELD(std::string &form, const std::string &name, long long  val)
{
	APPEND_X3W_FIELD(form, name, NumFmt("lld", val).c_str(), false);
}
std::string AppendClientTypeToUrl(const std::string& url, const char* name) {
	assert (!url.empty());

	std::string result = url;
	bool has_question_mark = (url.find_first_of('?') != std::string::npos);
	if (has_question_mark) {
		result += "&";
	} else {
		result += "?";
	}

	result += "clienttype=";
	result += escapeURL(name);
	printf("result:%s\r\n",result.c_str());
	return result;
}

}
}

