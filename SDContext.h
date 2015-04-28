//Author:zhaojunjiang 2015-3-20
#ifndef CONTEXT_H
#define CONTEXT_H
#include "surbasic/nocopyable.hpp"
#include "surbasic/DateObjects.h"
#include "surbasic/AccessToken.h"
using namespace SDBasic::token;
using namespace SDBasic;
namespace SDBasic{
   namespace context{
	
	class SDContext : public SDBasic::nocopyable
	{
	    public:
               void Clear();
		UserSpaceInfo user_info;
		std::string recycle_bin_id;
		std::string sync_id;
		std::string root_dir_id;
	   	std::string share_root_id;
	        UserAPIType api_type;
		static SDContext* g_ctx;

private:
	SDContext(void);
	~SDContext(void);

	};
#define CTX (*SDContext::g_ctx)
}
}//namespace
#endif
