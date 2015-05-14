#include "surbasic/XmlHelper.h"

namespace SDBasic{
namespace xml{

static const char* GetText(XMLElement* elem) {
  const char* v = elem->GetText();
  return (v == NULL ? "" : v);
}

bool ParseCheckAccountXml(const char * xml,AccessToken & token)
{
  XMLDocument doc;
  XMLError error = doc.Parse( xml );
  if(error !=0){
    printf("xml parse error\r\n");
    return false;}
  XMLElement * rootElement = doc.FirstChildElement("root");
  if(!rootElement){
    printf("find first element is not root.\r\n");
    return false;}
  XMLElement* firstElement = rootElement->FirstChildElement( "resultCode" );
  if(!firstElement){
    printf("result code parse error.\r\n");
    return false;}
  const char* first = GetText(firstElement);
  printf( "result: %s\n", first );

  XMLElement * NextElement = firstElement->NextSiblingElement();
  if(!NextElement){
    printf("webservername parse error.\r\n");
    return false;}
  const char * webservername = GetText(NextElement);
  printf( "webservername: %s\n", webservername );
  token.webserver = webservername;

  NextElement = NextElement->NextSiblingElement();
  if(!NextElement){
    printf("servername parse error.\r\n");
    return false;}
  const char * servername = GetText(NextElement);
  printf( "servername: %s\n", servername );
  token.server = servername;

  NextElement = NextElement->NextSiblingElement();
  if(!NextElement){
    printf("svgsservername parse error.\r\n");
    return false;}
  const char * svgsservername = GetText(NextElement);
  printf( "svgsservername: %s\n", svgsservername );
  token.svgserver = svgsservername;
  NextElement = NextElement->NextSiblingElement();
  if(!NextElement){
    printf("dbservername parse error.\r\n");
    return false;}
  const char * dbservername = GetText(NextElement);
  printf( "dbservername: %s\n", dbservername );
  token.dbserver = dbservername;
  NextElement = NextElement->NextSiblingElement();
  if(!NextElement){
    printf("privatekey parse error.\r\n");
    return false;}
  const char * pkname =  GetText(NextElement);
  printf( "privatekey: %s\n",pkname );
  token.privatekey = pkname;
  XMLElement* 	NextElementChild =NextElement->NextSiblingElement();
  if(!NextElementChild){
    printf("state parse error.\r\n");
    return false;}

  NextElement = NextElementChild->FirstChildElement( "state" );
  const char * statename = GetText(NextElement);
  printf( "state key: %s\n", statename );

  NextElement = NextElement->NextSiblingElement();
  if(!NextElement){
    printf("access token parse error.\r\n");
    return false;}
  const char * accesstokenname = GetText(NextElement);
  printf( "access token: %s\n", accesstokenname );
  token.access_token = accesstokenname;	
  if (token.access_token.empty())
    return false;

  NextElement = NextElement->NextSiblingElement();
  if(!NextElement){
    printf("expries  parse error.\r\n");
    return false;}
  const char * expriesname = GetText(NextElement);
  printf( "expries: %s\n", expriesname );
  token.expires_in = atoi(expriesname);

  NextElement = NextElement->NextSiblingElement();
  if(!NextElement){
    printf("refresh token  parse error.\r\n");
    return false;}
  const char * refreshname = GetText(NextElement);
  printf( "refresh: %s\n", refreshname );
  token.refresh_token = refreshname;
  if (token.refresh_token.empty())
    return false;

  NextElement = NextElement->NextSiblingElement();
  if(!NextElement){
    printf("resource server  parse error.\r\n");
    return false;}
  const char * resourcename = GetText(NextElement);
  printf( "resource: %s\n", resourcename );
  token.resource_server = resourcename;

  return true;
}		


}}//namespace
