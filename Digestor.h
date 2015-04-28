//
//  NSData_Aes256.h
//  SurDoc
//
//  Created by zhaojunjiang on 14/12/22.
//  Copyright (c) 2014年 sursen. All rights reserved.
//
#ifndef DIGESTER_H
#define DIGESTER_H
#include "surbasic/ByteArray.h"
using namespace SDBasic::bytearray;
namespace SDBasic{
namespace digestor{
enum HashRequired
{
    HASH_MD5 = 1,
    HASH_SHA1 = 2,
    HASH_SHA512 = 4,

	HASH_ALL = HASH_MD5 | HASH_SHA1 | HASH_SHA512,
};


class  Digestor
{
public:
	static bool HashFile(const std::string &path, int required, ByteArray &res);
	static std::string Md5AndShaFile(const std::string &path);
	static bool Sha512File(const std::string &path, ByteArray &hash);
    static bool Sha1File(const std::string &path, std::string *hash_hex);
	static std::string MD5File(const std::string &path);
	static std::string Md5str(const std::string &data);
	static std::string Sha1str(const std::string &data);
	static std::string HashPassword(const std::string& plain_pwd);

private:
	Digestor();
};
}}//namespace
#endif
