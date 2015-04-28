#include "surbasic/XmlHelper.h"

namespace SDBasic{
namespace xml{

void ParseCheckAccountXml(const char * xml,AccessToken & token)
{
		/*	const char * xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
			<root><resultCode>1</resultCode>\
			<webservername>data1p1e12.surdoc.com</webservername><servername>data1p1e12.surdoc.com:9443\
			</servername><svgservername>data1p1e12.surdoc.com:9443</svgservername><dbservername>\
			data1p1e12.surdoc.com:9443</dbservername><privatekey>M+vCuXV8fd2VAmfioMYoIlAlRS2ORrKikzP2aylnSwQ=\
			</privatekey><token><state>1</state><access_token>a71979bbf70ebe0116ec4a1277b83442</access_token>\
			<expires_in>581092</expires_in><refresh_token>cda937c340385bdcf4f14976195dc812</refresh_token>\
			<resource_server>https://resource.surdoc.com/</resource_server></token></root>";*/
			XMLDocument doc;
			XMLError error = doc.Parse( xml );
			if(error !=0){
			printf("xml parse error\r\n");
			return;}
			XMLElement * rootElement = doc.FirstChildElement("root");
			if(!rootElement){
				printf("find first element is not root.\r\n");
				return;}
			XMLElement* firstElement = rootElement->FirstChildElement( "resultCode" );
			if(!firstElement){
				printf("result code parse error.\r\n");
				return;}
			const char* first = firstElement->GetText();
			printf( "result: %s\n", first );
			
			XMLElement * NextElement = firstElement->NextSiblingElement();
			if(!NextElement){
				printf("webservername parse error.\r\n");
				return;}
			const char * webservername = NextElement->GetText();
			printf( "webservername: %s\n", webservername );
			token.webserver = webservername;
			NextElement = NextElement->NextSiblingElement();
			if(!NextElement){
				printf("servername parse error.\r\n");
				return;}
			const char * servername = NextElement->GetText();
			printf( "servername: %s\n", servername );
			token.server = servername;
			NextElement = NextElement->NextSiblingElement();
			if(!NextElement){
				printf("svgsservername parse error.\r\n");
				return;}
			const char * svgsservername = NextElement->GetText();
			printf( "svgsservername: %s\n", svgsservername );
			token.svgserver = svgsservername;
			NextElement = NextElement->NextSiblingElement();
			if(!NextElement){
				printf("dbservername parse error.\r\n");
				return;}
			const char * dbservername = NextElement->GetText();
			printf( "dbservername: %s\n", dbservername );
			token.dbserver = dbservername;
			NextElement = NextElement->NextSiblingElement();
			if(!NextElement){
				printf("privatekey parse error.\r\n");
				return;}
			const char * pkname = NextElement->GetText();
			printf( "privatekey: %s\n",pkname );
			token.privatekey = pkname;
	       XMLElement* 	NextElementChild =NextElement->NextSiblingElement();
			if(!NextElementChild){
				printf("state parse error.\r\n");
				return;}
			
			NextElement = NextElementChild->FirstChildElement( "state" );
			const char * statename = NextElement->GetText();
			printf( "state key: %s\n", statename );
	
			NextElement = NextElement->NextSiblingElement();
			if(!NextElement){
				printf("access token parse error.\r\n");
				return;}
			const char * accesstokenname = NextElement->GetText();
			printf( "access token: %s\n", accesstokenname );
			token.access_token = accesstokenname;	
			NextElement = NextElement->NextSiblingElement();
			if(!NextElement){
				printf("expries  parse error.\r\n");
				return;}
			const char * expriesname = NextElement->GetText();
			printf( "expries: %s\n", expriesname );
			token.expires_in = atoi(expriesname);
			NextElement = NextElement->NextSiblingElement();
			if(!NextElement){
				printf("refresh token  parse error.\r\n");
				return;}
			const char * refreshname = NextElement->GetText();
			printf( "refresh: %s\n", refreshname );
			token.refresh_token = refreshname;
			NextElement = NextElement->NextSiblingElement();
			if(!NextElement){
				printf("resource server  parse error.\r\n");
				return;}
			const char * resourcename = NextElement->GetText();
			printf( "resource: %s\n", resourcename );
			token.resource_server = resourcename;
}		


}}//namespace
