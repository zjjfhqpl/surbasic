//
//  NSData_Aes256.h
//  SurDoc
//
//  Created by zhaojunjiang on 14/12/22.
//  Copyright (c) 2014年 sursen. All rights reserved.
//
#include "surbasic/Digestor.h"
//#include "surbasic/FileInfo.h"
#include "surbasic/File.h"

//#include "surbasic/md5_dgst.h"
//#include "surbasic/sha.h"
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <cassert>
using namespace SDBasic::file;
using namespace SDBasic::digestor;
#define USE_SIMPLE_PWD_HASH 1
namespace SDBasic{
namespace digestor{
static std::string BinaryHashToHexString(const unsigned char *hash, int len)
{
	std::string out;

	// reverse enough space to gain a little speedup
	out.reserve(len * 2 + 1);  
	for (int i = 0; i < len; ++i)
		out += NumFmt("%02x", (int)hash[i]).c_str();
	return out;
}

std::string Digestor::Md5str(const std::string &data)
{
	unsigned char md5hash[16] = {0};
	MD5_CTX md5ctx;
	MD5_Init(&md5ctx);
	MD5_Update(&md5ctx, data.c_str(), data.length());
	MD5_Final(md5hash, &md5ctx);

	std::string digest = BinaryHashToHexString(md5hash, 16);
	return digest;
}

std::string Digestor::Md5AndShaFile(const std::string &path)
{
	assert (!path.empty()); 

	ByteArray res;
	if (HashFile(path, HASH_MD5 | HASH_SHA1, res) == false)
		return "";

	return res.toHexStr();
}

std::string Digestor::MD5File(const std::string &path)
{
	assert (!path.empty()); 

	ByteArray res;
	if (HashFile(path, HASH_MD5, res) == false)
		return "";

	return res.toHexStr();
}

bool Digestor::Sha512File(const std::string &path, ByteArray &hash)
{
	return HashFile(path, HASH_SHA512, hash);
}

bool Digestor::Sha1File(const std::string &path, std::string *hash_hex)
{
  ByteArray hash;
	bool ok = HashFile(path, HASH_SHA1, hash);
  if (ok)
    *hash_hex = hash.toHexStr();
  return ok;
}

bool Digestor::HashFile(const std::string &path, int required, ByteArray &res)
{
	const int kBufferSize = 1024 * 16; 

	//FileInfo::FileStatus st_before = FileInfo::StatFile(path);

	unsigned char md5hash[16] = {0};
	unsigned char sha1hash[20] = {0};
	unsigned char sha512hash[64] = {0};
	MD5_CTX md5ctx;
	SHA_CTX sha1ctx;
	SHA512_CTX sha512ctx;

	MD5_Init(&md5ctx);
	SHA1_Init(&sha1ctx);
	SHA512_Init(&sha512ctx);

	File inf;
	char buffer[kBufferSize];
	if (!inf.OpenForRead(path)) {
		return false;
	}

	unsigned long read_bytes;
	while (1)
	{

		if (!inf.Read(buffer, kBufferSize, &read_bytes)) {
			inf.Close();
			return false;
		}

		if (read_bytes == 0)
			break;
		if (required & HASH_MD5) 
			MD5_Update(&md5ctx, buffer, read_bytes);
		if (required & HASH_SHA1)  
			SHA1_Update(&sha1ctx, buffer, read_bytes);
		if (required & HASH_SHA512)  
			SHA512_Update(&sha512ctx, buffer, read_bytes);
	}
	inf.Close();

	if (required & HASH_MD5) 
	{
		MD5_Final(md5hash, &md5ctx);
		res.insert(res.end(), md5hash, md5hash + 16);
	}

	if (required & HASH_SHA1)  
	{
		SHA1_Final(sha1hash, &sha1ctx);
		res.insert(res.end(), sha1hash, sha1hash + 20);
	}

	if (required & HASH_SHA512)  
	{
		SHA512_Final(sha512hash, &sha512ctx);
		res.insert(res.end(), sha512hash, sha512hash + 64);
	}

//	FileInfo::FileStatus st_after = FileInfo::StatFile(path);
//	if (st_before.size != st_after.size || st_before.modifytime != st_after.modifytime) {
//		res.clear();
//		return false;
//	}

	return true;
}

std::string Digestor::Sha1str(const std::string &data)
{
	unsigned char sha1hash[20] = {0};
	SHA_CTX sha1ctx;
	SHA_Init(&sha1ctx);
	SHA_Update(&sha1ctx, data.c_str(), data.length());
	SHA_Final(sha1hash, &sha1ctx);

	std::string digest = BinaryHashToHexString(sha1hash, 20);
	return digest;
}

std::string Digestor::HashPassword(const std::string& plain_pwd)
{
#if USE_SIMPLE_PWD_HASH
	return Md5str(plain_pwd);
#else
	uint8_t sha512[64] = {0};
	SHA512_CTX sha512_ctx;
	SHA512_Init(&sha512_ctx);
	SHA512_Update(&sha512_ctx, plain_pwd.c_str(), plain_pwd.size());
	SHA512_Final(sha512, &sha512_ctx);

	uint8_t md5[16] = {0};
	MD5_CTX md5_ctx;
	MD5_Init(&md5_ctx);
	MD5_Update(&md5_ctx, sha512, 64);
	MD5_Final(md5, &md5_ctx);

	return BinaryHashToHexString(md5, 16);
#endif
}
}}//namespace
