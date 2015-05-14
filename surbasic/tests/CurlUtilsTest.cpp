#include<string>
#include "surbasic/CurlUtils.h"
#include <stdio.h>
using namespace SDBasic::Utils;
int main()
{
	std::string form;
	std::string account = "mac1cc.cc";
        APPEND_X3W_FIELD(form, "email", account);
	printf(form.c_str());
return 0;
}
