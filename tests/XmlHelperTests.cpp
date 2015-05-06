#include <surbasic/XmlHelper.h>

using namespace SDBasic;

int main() {
  const char* xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?> <root><resultCode>1</resultCode><webservername>data1p1e9.surdoc.com</webservername><servername>data1p1e9.surdoc.com:9443</servername><svgservername>data1p1e9.surdoc.com:9443</svgservername><dbservername>data1p1e9.surdoc.com:9443</dbservername><privatekey>kZJzwWOUPu37a02su5PEHzLjX/O10QxR5ZT07X0/ozM=</privatekey><token><state>0</state><access_token></access_token><expires_in></expires_in><refresh_token></refresh_token><resource_server></resource_server></token></root>";

  SDBasic::token::AccessToken t;
  SDBasic::xml::ParseCheckAccountXml(xml, t);
  return 0;
}
