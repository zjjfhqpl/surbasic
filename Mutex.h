//Author:: zhao junjiang 2015-3-11
#ifndef MUTEX_H
#define MUTEX_H

#include "surbasic/Type.h"
#include "surbasic/nocopyable.hpp"

#define MCHECK(ret) ({__typeof__ (ret) errnum = (ret) \
					assert(errnum==0); (void) errnum;})
namespace SDBasic{	
	class MutexLock:public SDBasic::nocopyable
	{
	public:
		MutexLock():holder_(0){
		pthread_mutex_init(&mutex_,NULL);}
		~MutexLock(){
			assert(holder_==0);
			pthread_mutex_destroy(&mutex_);}
	/*
	// must be called when locked, i.e. for assertion
	bool isLockedByThisThread() const
	{
	return holder_ == CurrentThread::tid();
	}

	void assertLocked() const
	{
	assert(isLockedByThisThread());
	}
	*/
	void lock()
	{
		printf("mutex lock\r\n");
		pthread_mutex_lock(&mutex_);
	}
	void unlock()
	{
		printf("mutex unlock \r\n");
		pthread_mutex_unlock(&mutex_);
	}
	pthread_mutex_t *getPthreadMutex(){return &mutex_;}
	private:
		pid_t holder_;
		pthread_mutex_t mutex_;
	};
	
	class MutexLockGuard : public SDBasic::nocopyable
	{
	public:
		explicit MutexLockGuard(MutexLock& mutex):mutex_(mutex){
			mutex_.lock();}
		~MutexLockGuard(){
			mutex_.unlock();
		}
	private:
		MutexLock & mutex_;
	};
	#define MutexLock(x) error "Missing guard object name"
}
#endif
