//
//  NSData_Aes256.h
//  SurDoc
//
//  Created by zhaojunjiang on 14/12/22.
//  Copyright (c) 2014年 sursen. All rights reserved.
//
#ifndef BASE64HELPER_H
#define BASE64HELPER_H
#include "surbasic/Type.h"
#include "surbasic/ByteArray.h"
using namespace SDBasic::bytearray;
namespace SDBasic{
namespace base64{

class  Base64Helper

{
public:
	static bool Encode(const ByteArray &data, ByteArray &out);
	static std::string Encode(const ByteArray &data);
	static std::string Encode(const std::string &hexdata);

	static bool Decode(const ByteArray &data, ByteArray &out);
	static bool Decode(const std::string &data, ByteArray &out);
};

// see http://en.wikipedia.org/wiki/Base64#URL_applications
// '+' --> '-'
// '/' --> '_'
// '=' --> '.'
class  UrlBase64Helper
{
public:
	static std::string Encode(const ByteArray &data);
	static std::string EncodeTrimLast(const ByteArray &data);
	static std::string EncodeTrimLast(const std::string &data);
	static bool Decode(const std::string &data, ByteArray &out);
};

}}//namespace
#endif
