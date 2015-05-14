//Author: zhaojunjiang 2015-3-11 
#ifndef CONDITION_H
#define CONDITION_H
#include "surbasic/nocopyable.hpp"
#include "surbasic/Mutex.h"
#include "surbasic/Type.h"

namespace SDBasic{
namespace contidion{
class Contidion : public SDBasic::nocopyable{
public:
	explicit Contidion(MutexLock & mutex):mutex_(mutex){
	pthread_cond_init(&pcond_,NULL);}

	~Contidion(){
	pthread_cond_destroy(&pcond_);}

	void wait(){
	pthread_cond_wait(&pcond_,mutex_.getPthreadMutex());}

	bool waitForSecond(int seconds){
	struct timespec abstime;
	clock_gettime(CLOCK_REALTIME,&abstime);
	abstime.tv_sec +=seconds;
	return ETIMEDOUT == pthread_cond_timedwait(&pcond_,mutex_.getPthreadMutex(),&abstime);}
	
	void notify(){
	pthread_cond_signal(&pcond_);}
	
	void notifyAll(){
	pthread_cond_broadcast(&pcond_);}	

private:
	MutexLock & mutex_;
	pthread_cond_t pcond_;
};
}}//namespace
#endif
