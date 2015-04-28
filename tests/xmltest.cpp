#include "surbasic/XmlHelper.h"
#include "surbasic/AccessToken.h"

using namespace SDBasic::token;

int main(){
	AccessToken token;
	const char * xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
	<root><resultCode>1</resultCode>\
	<webservername>data1p1e12.surdoc.com</webservername><servername>data1p1e12.surdoc.com:9443\
	</servername><svgservername>data1p1e12.surdoc.com:9443</svgservername><dbservername>\
	data1p1e12.surdoc.com:9443</dbservername><privatekey>M+vCuXV8fd2VAmfioMYoIlAlRS2ORrKikzP2aylnSwQ=\
	</privatekey><token><state>1</state><access_token>a71979bbf70ebe0116ec4a1277b83442</access_token>\
	<expires_in>581092</expires_in><refresh_token>cda937c340385bdcf4f14976195dc812</refresh_token>\
	<resource_server>https://resource.surdoc.com/</resource_server></token></root>";
	SDBasic::xml::ParseCheckAccountXml(xml,token);
	printf("token resource:%s\r\n",token.resource_server.c_str());
return 1;
}
