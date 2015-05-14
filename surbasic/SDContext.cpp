//Author:zhaojunjiang
#include "surbasic/SDContext.h"
using namespace SDBasic::context;
SDContext::SDContext()
{
}
SDContext::~SDContext()
{

}

SDContext* SDContext::g_ctx = new SDContext();

void SDContext::Clear(){
	printf("SDcontext clear\r\n");
	api_type = USER_API_OPENAPI;
	user_info = UserSpaceInfo();
	recycle_bin_id.clear();
	sync_id.clear();
}

