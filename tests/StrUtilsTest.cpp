#include<stdio.h>
#include "surbasic/StrUtils.hpp"
#include<string>
using namespace SDBasic;
int main(){
	printf("%s\n",NumFmt("%d",12).c_str());

	return 1;
}

