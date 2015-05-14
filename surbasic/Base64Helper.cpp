//
//  NSData_Aes256.h
//  SurDoc
//
//  Created by zhaojunjiang on 14/12/22.
//  Copyright (c) 2014年 sursen. All rights reserved.
//
#include "surbasic/Base64Helper.h"
#include "surbasic/ByteArray.h"
namespace SDBasic{
namespace base64{

static inline char indexToChar(unsigned int index)
{
	assert (index < 64);
	static const char *base64Chars = 
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";
	return base64Chars[index];
}

static inline unsigned char charToIndex(unsigned char ch)
{
	static const unsigned char decoding[] = {
		62,128,128,128,
		63,52,53,54,55,56,57,58,59,60,61,
		128,128,128,128,128,128,128,
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
		128,128,128,128,128,128,
		26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51
	};

	static const int  maxIndex = sizeof(decoding) / sizeof(unsigned char);

	int idx = ch - 43;
	if (idx > maxIndex)
		return 128;

	return (decoding[idx]);
};

std::string Base64Helper::Encode(const ByteArray &data)
{
	ByteArray out;
	bool ok = Encode(data, out);
	assert (ok);

	return out.toRawStr();
}

std::string Base64Helper::Encode(const std::string &hexdata)
{
	return Encode(ByteArray::fromHexStr(hexdata));
}

bool Base64Helper::Encode(const ByteArray &data, ByteArray &out)
{
	size_t numThreeByteGroups = data.size() / 3;
	size_t numRemainBytes = data.size() % 3;

	for (size_t i = 0; i < numThreeByteGroups; ++i)
	{
		unsigned char b1 = data[i*3];
		unsigned char b2 = data[i*3+1];
		unsigned char b3 = data[i*3+2];
		unsigned int d = (b1 << 16) | (b2 << 8) | b3;
		out.push_back(indexToChar((d & 0xfc0000) >> 18));
		out.push_back(indexToChar((d & 0x03f000) >> 12));
		out.push_back(indexToChar((d & 0x000fc0) >> 6));
		out.push_back(indexToChar((d & 0x00003F)));
	}

	if (numRemainBytes == 1)
	{
		unsigned char b1 = data[numThreeByteGroups * 3];
		unsigned int d = (b1 << 16);
		out.push_back(indexToChar((d & 0xfc0000) >> 18));
		out.push_back(indexToChar((d & 0x03f000) >> 12));
		out.push_back('=');
		out.push_back('=');
	} else if (numRemainBytes == 2) {
		unsigned char b1 = data[numThreeByteGroups * 3];
		unsigned char b2 = data[numThreeByteGroups * 3 + 1];
		unsigned int d = (b1 << 16) | (b2 << 8);
		out.push_back(indexToChar((d & 0xfc0000) >> 18));
		out.push_back(indexToChar((d & 0x03f000) >> 12));
		out.push_back(indexToChar((d & 0x000fc0) >> 6));
		out.push_back('=');
	}

	return true;
}

bool Base64Helper::Decode(const std::string &data, ByteArray &out)
{
	return Decode(ByteArray::fromRawStr(data.c_str()), out);
}

bool Base64Helper::Decode(const ByteArray &data, ByteArray &out)
{
	if (data.empty() || (data.size() % 4 != 0))
		return false;

	size_t safe = data.size();
	size_t remain = 0;
	if (data.back() == '=')
	{
		safe -= 4;
		remain = 4;
	}

	for (size_t i = 0; i < safe; i += 4)
	{
		unsigned char b1 = charToIndex(data[i]);
		unsigned char b2 = charToIndex(data[i+1]);
		unsigned char b3 = charToIndex(data[i+2]);
		unsigned char b4 = charToIndex(data[i+3]);

		if (b1 == 128 || b2 == 128 || b3 == 128 || b4 == 128)
			return false;

		unsigned int d = (b1 << 18) | (b2 << 12) | (b3 << 6) | b4;

		out.push_back(static_cast<unsigned char>((d & 0xFF0000) >> 16));
		out.push_back(static_cast<unsigned char>((d & 0xFF00) >> 8));
		out.push_back(static_cast<unsigned char>(d & 0xFF));
	}

	if (remain > 0)
	{
		unsigned char b1 = charToIndex(data[safe]);
		unsigned char b2 = charToIndex(data[safe+1]);

		if (b1 == 128 || b2 == 128)
			return false;

		out.push_back(static_cast<unsigned char>((b1 << 2) | (b2 >> 4)));
		unsigned char b3 = charToIndex(data[safe+2]);
		if (b3 != 128) // !'='
		{
			out.push_back(static_cast<unsigned char>(
              ((b2 & 0xF) << 4) | (b3 >> 2)));
		}
	}

	return true;
}

std::string UrlBase64Helper::Encode(const ByteArray &data)
{
	std::string native_base64 = Base64Helper::Encode(data);
	std::string s2;
	s2.reserve(native_base64.size());
	for (std::string::const_iterator it = native_base64.begin(); 
		it != native_base64.end(); ++it)
	{
		const char c = *it;
		if (c == '+')
			s2 += '-';
		else if (c == '/')
			s2 += '_';
		//else if (c == '=')
		//	s2 += '.';
		else 
			s2 += c;
	}
	return s2;
}

bool UrlBase64Helper::Decode(const std::string &data, ByteArray &out)
{
	std::string s2;
	s2.reserve(data.size());
	for (std::string::const_iterator it = data.begin(); it != data.end(); ++it)
	{
		const char c = *it;
		if (c == '-')
			s2 += '+';
		else if (c == '_')
			s2 += '/';
		else if (c == '.')
			s2 += '=';
		else 
			s2 += c;
	}
	return Base64Helper::Decode(s2, out);
}

std::string UrlBase64Helper::EncodeTrimLast(const ByteArray &data)  {
	std::string out = Encode(data);
	
	while (!out.empty() && out[out.size()-1] == '=') 
		out.erase(out.size()-1);
	return out;
}

std::string UrlBase64Helper::EncodeTrimLast(const std::string &data) {
	return EncodeTrimLast(ByteArray::fromRawStr(data.c_str()));
}
}}//namespac
