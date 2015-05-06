//Author:zhaojunjiang 2015-4-22
#ifndef	XML_H
#define XML_H
#include "surbasic/tinyxml/tinyxml2.h"
#include "surbasic/AccessToken.h"
#include "surbasic/DateObjects.h"
using namespace tinyxml2;
using namespace SDBasic::token;
namespace SDBasic{
namespace xml{

bool ParseCheckAccountXml(const char * xml,AccessToken & token);

}}//namespace
#endif
