//Author:zhaojunjiang 2015-4-13
#include "surbasic/TreeEntity.h"
using namespace std;
using namespace SDBasic;
using namespace SDBasic::code;
using namespace SDBasic::time;
namespace SDBasic{
namespace tree{

    TreeEntity::TreeEntity()
    :id(""),name(""),extension(""),parentid(""),filesize(0),\
    gifnum(0),createtime(Timestamp::Now()),modifytime(Timestamp::Now()),lastupdatetime(Timestamp::Now()),\
    isdoc(false),nodetype(FTGeneralFolder),pagecount(0),creator(""),del(0),metaV(tree::CAN_NOT_CONVERT),\
    md5digest(""),version(0),stroagekey(""),sharetype(SHARE_NONE),sharerootid("")	
    {}
    TreeEntity::~TreeEntity(){}
	
}}//namespace
