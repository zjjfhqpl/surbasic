//
//  NSData_Aes256.h
//  SurDoc
//
//  Created by zhaojunjiang on 14/12/22.
//  Copyright (c) 2014年 sursen. All rights reserved.
//
#include "surbasic/ByteArray.h"
#include "surbasic/Base64Helper.h"
using namespace SDBasic::base64;
namespace SDBasic{
namespace bytearray{
static unsigned char HexToInt(unsigned char ch)
{
	if (ch >= 'a' && ch <= 'z')
		return ch - 'a' + 10;

	if (ch >= '0' && ch <= '9')
		return ch - '0';

	if (ch >= 'A' && ch <= 'Z')
		return ch - 'A' + 10;
	return 0;
}

ByteArray ByteArray::fromHexStr(const std::string &hex)
{
	assert (hex.size() % 2 == 0);
	ByteArray out;
	for (unsigned i = 0; i < hex.length(); i+=2)
	{
		unsigned char high = HexToInt(hex[i]);
		unsigned char low = HexToInt(hex[i+1]);
		out.push_back((high << 4) | low);
	}
	return out;
}


ByteArray ByteArray::genRandom(size_t size)
{
	ByteArray bytes(size);
	srand((unsigned int)time(NULL));
	for (unsigned i = 0; i<size;++i)
		bytes[i] = rand() % 256;
	return bytes;
}
std::string ByteArray::toHexStr() const
{
	std::string out;
	out.reserve(this->size() * 2 + 1);  
	for (int i = 0; i < this->size(); ++i)
		out += NumFmt("%02x", (int)this->at(i)).c_str(); 
	return out;
}
std::string ByteArray::toDecimalStr() const
{
	std::string out;
    out.reserve(this->size()*2+1);
	for (int i =0;i<this->size();++i)
		out+=NumFmt("%d",(int)this->at(i)).c_str();
	return out;
}

ByteArray ByteArray::fromKey(const  char *raw ,unsigned int length)
{
    ByteArray out;
    
    out.insert(out.end(), raw, raw + length);
    return out;
}

/*ByteArray ByteArray::fromRawStr(const  char *raw)
{
	ByteArray out;
	out.insert(out.end(), raw, raw + strlen(raw));
	return out;
}*/

ByteArray ByteArray::fromRawStr(const std::string &raw)
{
	ByteArray out;
	const uint8_t *start = (const uint8_t *)raw.c_str();
	out.insert(out.end(), start, start + raw.length());
	return out;
}
std::string ByteArray::toRawStr() const
{
	return std::string(this->begin(), this->end());
}

std::string ByteArray::toBase64Str() const
{
    return Base64Helper::Encode(*this);
}

std::string ByteArray::toUrlBase64Str() const
{
    return UrlBase64Helper::Encode(*this);
}

}}//namespace
