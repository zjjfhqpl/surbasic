//Author:zhaojunjiang
#ifndef TOKENMANAGER_H
#define TOKENMANAGER_H
#include "surbasic/Type.h"
#include "surbasic/AccessToken.h"
#include "surbasic/Mutex.hpp"
using namespace SDBasic::token;
using namespace SDBasic::lock;
namespace SDBasic{
namespace tokenmg{

class TokenManager{
public:
	TokenManager(void);
	~TokenManager(void);
	static TokenManager& get();
	uint64_t SetToken(const AccessToken & _token);
	bool GetToken(const uint64_t & key,AccessToken & token);
private:
	MutexLock lock_;
	static TokenManager * ins_;
	typedef std::map<uint64_t ,AccessToken> TokenSet;
	TokenSet tokenset_;
};
}}//namespace
#endif
