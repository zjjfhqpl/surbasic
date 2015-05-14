//
//  NSData_Aes256.h
//  SurDoc
//
//  Created by zhaojunjiang on 14/12/22.
//  Copyright (c) 2014年 sursen. All rights reserved.
//
#ifndef AESHELPER_H
#define AESHELPER_H
#include "surbasic/ByteArray.h"
#include "surbasic/Public_Define.h"
#include "surbasic/Type.h"
using namespace SDBasic::code;
using namespace SDBasic::bytearray;

namespace SDBasic{
namespace aes{
class  Aes256Encrypt
{
public:
	Aes256Encrypt(const ByteArray &key, int blockSize);
	virtual ~Aes256Encrypt();

	bool Update(const unsigned char *data, int datalen);
	bool Finalize();

	const unsigned char *data() { return out_; }
	const unsigned char *end() { return out_ + outlen_; }
	int len() { return outlen_; }

private:
	void *ctx_;
	int  blockSize_;
	unsigned char *out_;
	int outlen_;
};

class  Aes256Decrypt
{
public:
	Aes256Decrypt(const ByteArray &key, int blockSize);
	virtual ~Aes256Decrypt();

	virtual bool Update(const unsigned char *data, int datalen);
	virtual bool Finalize();

	const unsigned char *data() { return out_; }
	const unsigned char *end() { return out_ + outlen_; }
	int len() { return outlen_; }

private:
	void *ctx_;
	int  blockSize_;
	unsigned char *out_;
	int outlen_;
};
class  Aes128Encrypt
{
public:
	Aes128Encrypt(const ByteArray &key, int blockSize);
	virtual ~Aes128Encrypt();

	bool Update(const unsigned char *data, int datalen);
	bool Finalize();

	const unsigned char *data() { return out_; }
	const unsigned char *end() { return out_ + outlen_; }
	int len() { return outlen_; }

private:
	void *ctx_;
	int  blockSize_;
	unsigned char *out_;
	int outlen_;
};

class  Aes128Decrypt
{
public:
	Aes128Decrypt(const ByteArray &key, int blockSize);
	virtual ~Aes128Decrypt();

	virtual bool Update(const unsigned char *data, int datalen);
	virtual bool Finalize();

	const unsigned char *data() { return out_; }
	const unsigned char *end() { return out_ + outlen_; }
	int len() { return outlen_; }

private:
	void *ctx_;
	int  blockSize_;
	unsigned char *out_;
	int outlen_;
};

enum AesMode {
	kAes128 = 128,
	kAes256 = 256,
};

class  AESHelper
{
public:
	static ByteArray Encrypt256(const ByteArray &key, const ByteArray &plain);
	static ByteArray Encrypt128(const ByteArray &key, const ByteArray &plain);

	static ByteArray Decrypt256(const ByteArray &key, const ByteArray &cipher);
	static ByteArray Decrypt128(const ByteArray &key, const ByteArray &cipher);

	static SD_CODE EncFile(const ByteArray &key, const std::string &in, const std::string &out/*, IStoppingFlag *stop = NULL*/);
	static SD_CODE DecFile(const ByteArray &key, const std::string &in, const std::string &out/*, IStoppingFlag *stop = NULL*/);

	
	static SD_CODE DecAndUncompressFile(const ByteArray &key, const std::string &in, const std::string &out/*, IStoppingFlag *stop = NULL*/);
	static SD_CODE CompressAndEncFile(const ByteArray &key, const std::string &in, const std::string &out, 
		std::string *enc_sha1, std::string* src_md5/*, IStoppingFlag *stop = NULL*/);

	static ByteArray DecryptPrivateKey( ByteArray &plain_pwd, const ByteArray &pk_cipher);
	static ByteArray EncryptPrivateKey( ByteArray &plain_pwd, const ByteArray &pk_plain);
};
}}//namespace
#endif
