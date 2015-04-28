#include "surbasic/TokenManager.h"

namespace SDBasic{
namespace tokenmg{

TokenManager * TokenManager::ins_ = NULL;

TokenManager::TokenManager(void){}

TokenManager::~TokenManager(void){}

TokenManager& TokenManager::get(){
	static MutexLock lockins_;
	SD_Lock lock(lockins_);
	if(ins_==NULL){
	  ins_=new TokenManager();}
	return *ins_;	
}

uint64_t TokenManager::SetToken(const AccessToken & _token){
	SD_Lock lock(lock_);
    AccessToken token = _token;
	uint64_t key = abs((int64_t)&token);
	tokenset_.insert(make_pair(key,token));	
	return key;
}
bool TokenManager::GetToken(const uint64_t &key,AccessToken & token){
	SD_Lock lock(lock_);
	TokenSet::iterator found = tokenset_.find(key);
	if(found!=tokenset_.end()){
		token =  found->second;
		return true;}
	return false;
}

}}
