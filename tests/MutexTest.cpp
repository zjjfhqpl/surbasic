#include<stdio.h>
#include "surbasic/Mutex.h"
using namespace SDBasic;
int main(){
	MutexLock _lock;
	MutexLockGuard lock(_lock);
	
	return 1;}
