#include<stdio.h>
#include"Timestamp.h"
#include <string>
using namespace SDBasic::time;
int main()
{
std::string time = Timestamp::toFormattedString();
printf("time:%s\r\n",time.c_str());

return 1;
}
