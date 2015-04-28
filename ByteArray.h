//
//  NSData_Aes256.h
//  SurDoc
//
//  Created by zhaojunjiang on 14/12/22.
//  Copyright (c) 2014年 sursen. All rights reserved.
//
#ifndef BYTEARRAY_H
#define BYTEARRAY_H
#include "surbasic/Type.h"
#include "surbasic/StrUtils.hpp"
using namespace SDBasic::fmt;
namespace SDBasic{
namespace bytearray{
class ByteArray :public std::vector<unsigned char>
{
	typedef std::vector<unsigned char> base;
public:
	ByteArray():base(){}
	ByteArray(size_t s):base(s){}
	~ByteArray() {}
	std::string toBase64Str() const;
	std::string toUrlBase64Str() const;
	std::string toRawStr() const;
	std::string toHexStr() const;
	std::string toDecimalStr() const;

        static ByteArray fromKey(const  char *raw ,unsigned int length);
//	static ByteArray fromRawStr(const  char *raw);
	static ByteArray fromRawStr(const std::string &str);
	static ByteArray fromHexStr(const std::string &str);
	static ByteArray genRandom(size_t size);
    
};

}}//namespace 
#endif
