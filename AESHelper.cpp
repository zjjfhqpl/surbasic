
//  NSData_Aes256.h
//  SurDoc
//
//  Created by zhaojunjiang on 14/12/22.
//  Copyright (c) 2014年 sursen. All rights reserved.
//
#include <zlib.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include "surbasic/File.h"
#include "surbasic/ByteArray.h"
#include "surbasic/AESHelper.h"

using namespace SDBasic::file;
using namespace SDBasic::bytearray;
namespace SDBasic{
namespace aes{

static inline int GetAesMaxOutlen(int len)
{
	return (len/AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;;
}

Aes256Encrypt::Aes256Encrypt(const ByteArray &key, int blockSize) 
:ctx_(NULL),
outlen_(0),
blockSize_(blockSize)
{ 
	assert (key.size() == 32);
	ctx_ = new EVP_CIPHER_CTX();
	EVP_CIPHER_CTX_init((EVP_CIPHER_CTX *)ctx_);
	int ret = EVP_EncryptInit((EVP_CIPHER_CTX *)ctx_, EVP_aes_256_ecb(), &key[0], NULL);
	assert (ret = 1);

	out_ = new unsigned char[GetAesMaxOutlen(blockSize)];
}

Aes256Encrypt::~Aes256Encrypt() 
{ 
	if (ctx_ != NULL)
	{
		EVP_CIPHER_CTX_cleanup((EVP_CIPHER_CTX *)ctx_);
		delete (EVP_CIPHER_CTX*)ctx_;
		ctx_ = NULL;

		delete []out_;
		out_ = NULL;
	} 
}

bool Aes256Encrypt::Update(const unsigned char *in, int inlen)
{
	assert (inlen <= blockSize_);
	int ret = EVP_EncryptUpdate((EVP_CIPHER_CTX *)ctx_, out_, &outlen_, in, inlen);
	assert (ret == 1);
	return true;
}

bool Aes256Encrypt::Finalize()
{
	int ret = EVP_EncryptFinal_ex((EVP_CIPHER_CTX *)ctx_, out_, &outlen_);
	assert (ret == 1);
	return true;
}


Aes256Decrypt::Aes256Decrypt(const ByteArray &key, int blockSize) 
:ctx_(NULL),
outlen_(0),
blockSize_(blockSize)
{ 
	assert (key.size() == 32);
	ctx_ = new EVP_CIPHER_CTX();
	EVP_CIPHER_CTX_init((EVP_CIPHER_CTX *)ctx_);
	int ret = EVP_DecryptInit((EVP_CIPHER_CTX *)ctx_, EVP_aes_256_ecb(), &key[0], NULL);
	assert (ret = 1);

	out_ = new unsigned char[GetAesMaxOutlen(blockSize)];
}

Aes256Decrypt::~Aes256Decrypt() 
{ 
	if (ctx_ != NULL)
	{
		EVP_CIPHER_CTX_cleanup((EVP_CIPHER_CTX *)ctx_);
		delete (EVP_CIPHER_CTX *)ctx_;
		ctx_ = NULL;

		delete []out_;
		out_ = NULL;
	}
}

bool Aes256Decrypt::Update(const unsigned char *in, int inlen)
{
	int ret = EVP_DecryptUpdate((EVP_CIPHER_CTX *)ctx_, out_, &outlen_, in, inlen);
	assert (ret == 1);
	return true;
}

bool Aes256Decrypt::Finalize()
{
	int ret = EVP_DecryptFinal_ex((EVP_CIPHER_CTX *)ctx_, out_, &outlen_);
	
	return (ret == 1);
}
Aes128Encrypt::Aes128Encrypt(const ByteArray &key, int blockSize) 
:ctx_(NULL),
outlen_(0),
blockSize_(blockSize)
{ 
	assert (key.size() == 16);
	ctx_ = new EVP_CIPHER_CTX();
	EVP_CIPHER_CTX_init((EVP_CIPHER_CTX *)ctx_);
	int ret = EVP_EncryptInit((EVP_CIPHER_CTX *)ctx_, EVP_aes_128_ecb(), &key[0], NULL);
	assert (ret = 1);

	out_ = new unsigned char[GetAesMaxOutlen(blockSize)];
}

Aes128Encrypt::~Aes128Encrypt() 
{ 
	if (ctx_ != NULL)
	{
		EVP_CIPHER_CTX_cleanup((EVP_CIPHER_CTX *)ctx_);
		delete (EVP_CIPHER_CTX *)ctx_;
		ctx_ = NULL;

		delete []out_;
		out_ = NULL;
	}
}

bool Aes128Encrypt::Update(const unsigned char *in, int inlen)
{
	assert (inlen <= blockSize_);
	int ret = EVP_EncryptUpdate((EVP_CIPHER_CTX *)ctx_, out_, &outlen_, in, inlen);
	assert (ret == 1);
	return true;
}

bool Aes128Encrypt::Finalize()
{
	int ret = EVP_EncryptFinal_ex((EVP_CIPHER_CTX *)ctx_, out_, &outlen_);
	assert (ret == 1);
	return true;
}


Aes128Decrypt::Aes128Decrypt(const ByteArray &key, int blockSize) 
:ctx_(NULL),
outlen_(0),
blockSize_(blockSize)
{ 
	assert (key.size() == 16);
	ctx_ = new EVP_CIPHER_CTX();
	EVP_CIPHER_CTX_init((EVP_CIPHER_CTX *)ctx_);
	int ret = EVP_DecryptInit((EVP_CIPHER_CTX *)ctx_, EVP_aes_128_ecb(), &key[0], NULL);
	assert (ret = 1);

	out_ = new unsigned char[GetAesMaxOutlen(blockSize)];
}

Aes128Decrypt::~Aes128Decrypt() 
{ 
	if (ctx_ != NULL)
	{
		EVP_CIPHER_CTX_cleanup((EVP_CIPHER_CTX *)ctx_);
		delete (EVP_CIPHER_CTX *)ctx_;
		ctx_ = NULL;

		delete []out_;
		out_ = NULL;
	}
}

bool Aes128Decrypt::Update(const unsigned char *in, int inlen)
{
	int ret = EVP_DecryptUpdate((EVP_CIPHER_CTX *)ctx_, out_, &outlen_, in, inlen);
	assert (ret == 1);
	return true;
}

bool Aes128Decrypt::Finalize()
{
	int ret = EVP_DecryptFinal_ex((EVP_CIPHER_CTX *)ctx_, out_, &outlen_);

	return (ret == 1);
}

ByteArray AESHelper::Encrypt128(const ByteArray &key, const ByteArray &plain)
{
	assert (plain.size() > 0);
	ByteArray cipher;

	Aes128Encrypt enc(key, plain.size());
	enc.Update(&plain[0], plain.size());
	cipher.insert(cipher.end(), enc.data(), enc.end());

	enc.Finalize();
	cipher.insert(cipher.end(), enc.data(), enc.end());

	return cipher;
}

ByteArray AESHelper::Decrypt128(const ByteArray &key, const ByteArray &cipher)
{
	assert (cipher.size() > 0);
	ByteArray plain;

	Aes128Decrypt dec(key, cipher.size());
	dec.Update(&cipher[0], cipher.size());
	plain.insert(plain.end(), dec.data(), dec.end());

	if (dec.Finalize() == false)
	{
		plain.clear();
		return plain;
	}

	plain.insert(plain.end(), dec.data(), dec.end());

	return plain;
}

ByteArray AESHelper::Encrypt256(const ByteArray &key, const ByteArray &plain)
{
	assert (plain.size() > 0);
	ByteArray cipher;

	Aes256Encrypt enc(key, plain.size());
	enc.Update(&plain[0], plain.size());
	cipher.insert(cipher.end(), enc.data(), enc.end());

	enc.Finalize();
	cipher.insert(cipher.end(), enc.data(), enc.end());

	return cipher;
}

ByteArray AESHelper::Decrypt256(const ByteArray &key, const ByteArray &cipher)
{
	assert (cipher.size() > 0);
	ByteArray plain;

	Aes256Decrypt dec(key, cipher.size());
	dec.Update(&cipher[0], cipher.size());
	plain.insert(plain.end(), dec.data(), dec.end());

	if (dec.Finalize() == false)
	{
		plain.clear();
		return plain;
	}

	plain.insert(plain.end(), dec.data(), dec.end());

	return plain;
}

SD_CODE AESHelper::EncFile(const ByteArray &key, const std::string &in, const std::string &out/*, IStoppingFlag *stop*/)
{
	const int kBlockSize = 4096;
	File inf, outf;
	if (!inf.OpenForRead(in))
		return SD_FAILED;
	
	if (!outf.OpenForWrite(out)) {
		inf.Close();
		return SD_FAILED;
	}

	unsigned char buf[kBlockSize];
	Aes256Encrypt enc(key, kBlockSize);

	DWORD read_bytes, written_bytes;
	while (1)
	{
		/*if (stop != NULL && stop->IsStopping())
			goto failed;*/

		if (!inf.Read(buf, sizeof(buf), &read_bytes))
			goto failed;

		if (read_bytes == 0)
		{
			enc.Finalize();
			if (!outf.Write(enc.data(), enc.len(), &written_bytes))
				goto failed;

			inf.Close();
			outf.Close();
			return SD_SUCCESSED;
		} 

		enc.Update(buf, read_bytes);
		if (!outf.Write(enc.data(), enc.len(),&written_bytes))
			goto failed;
	}

failed:
	inf.Close();
	outf.Close();
	return SD_FAILED;
}

static std::string BinaryHashToHexString(const unsigned char *hash, int len)
{
	std::string out;

	
	out.reserve(len * 2 + 1);  
	for (int i = 0; i < len; ++i)
		out += NumFmt("%02x", (int)hash[i]).c_str();
	return out;
}

SD_CODE AESHelper::CompressAndEncFile(const ByteArray &key, const std::string &in, const std::string &out, std::string *enc_sha1, std::string*src_md5/*, IStoppingFlag *stop*/) 
{
	std::string errmsg;
	const int kBlockSize = 4096;
	File inf, outf;
	unsigned char buf[kBlockSize];
	Aes256Encrypt cipher(key, kBlockSize);

	int ret, flush;
	unsigned have;
	z_stream strm;
	unsigned char outbuf[kBlockSize];
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;

	SHA_CTX sha1ctx;
	unsigned char sha1hash[20] = {0};
	SHA1_Init(&sha1ctx);
	MD5_CTX md5ctx;
	unsigned char md5hash[16] = {0};
	MD5_Init(&md5ctx);

	::deflateInit(&strm, Z_DEFAULT_COMPRESSION); 

	if (!inf.OpenForRead(in)) {
		printf("failed to open file: %s :%s", in.c_str());
		goto error;
	}

	if (!outf.OpenForWrite(out)) {
		printf("failed to open file: %s :%s", out.c_str());
		inf.Close();
		goto error;
	}

	DWORD read_bytes, written_bytes;
	while (1)
	{
		/*if (stop != NULL && stop->IsStopping()) {
			goto error;
		}*/

		if (!inf.Read(buf, sizeof(buf), &read_bytes)) {
			printf("fread() failed: %s ", out.c_str());
			goto error;
		}

		MD5_Update(&md5ctx, buf, read_bytes);

		int flush = (read_bytes == 0 ? Z_FINISH : Z_NO_FLUSH);
		strm.avail_in = read_bytes;
		strm.next_in = buf;

		do {
			strm.avail_out = kBlockSize;
			strm.next_out = outbuf;
			ret = ::deflate(&strm, flush);
			assert (ret !=  Z_STREAM_ERROR);
			have = kBlockSize - strm.avail_out;
			switch (ret) {
				case Z_NEED_DICT:
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					goto error;
			}

			if (!cipher.Update(outbuf, have)) {
				goto error;
			}
			
			if (cipher.len() > 0) {
				if (!outf.Write(cipher.data(), cipher.len(),& written_bytes)) {
					printf("failed to write output: %s ", out.c_str());
					goto error;
				}

				SHA1_Update(&sha1ctx, cipher.data(), cipher.len());
			}
		} while (strm.avail_out == 0);

		if (ret == Z_STREAM_END) 
			break;
	}

	if (!cipher.Finalize()) {
		printf("AES256.Finalize() failed");
		goto error;
	}

	if (cipher.len() > 0) {
		if (!outf.Write(cipher.data(), cipher.len(),& written_bytes)) {
			printf("failed to write output: %s ", out.c_str());
			goto error;
		}

		SHA1_Update(&sha1ctx, cipher.data(), cipher.len());
	}

	::deflateEnd(&strm);
	inf.Close();
	outf.Close();
	SHA1_Final(sha1hash, &sha1ctx);
	enc_sha1->assign(BinaryHashToHexString(sha1hash, 20));
	MD5_Final(md5hash, &md5ctx);
	src_md5->assign(BinaryHashToHexString(md5hash, 16));
	return SD_SUCCESSED;

error:
	::deflateEnd(&strm);
	if (inf.IsOpened())
	{
	inf.Close();
	}
	if (outf.IsOpened())
	{
	outf.Close();
	}
	return SD_FAILED;
}


SD_CODE AESHelper::DecAndUncompressFile(const ByteArray &key, const std::string &in, const std::string &out/*, IStoppingFlag *stop*/) {
	std::string errmsg;
	const int kBlockSize = 4096;
	File inf, outf; 
	unsigned char buf[kBlockSize];
	Aes256Decrypt dec(key, kBlockSize);

	int ret, flush;
	unsigned have;
	z_stream strm;
	unsigned char outbuf[kBlockSize];
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;

	::inflateInit(&strm); 

	if (!inf.OpenForRead(in)) {
		printf("failed to open file: %s ", in.c_str());
		goto error;
	}

	if (!outf.OpenForWrite(out)) {
		printf("failed to open file: %s ", out.c_str());
		inf.Close();
		goto error;
	}

	DWORD read_bytes, written_bytes;
	while (1)
	{
		/*if (stop != NULL && stop->IsStopping()) {
			goto error;
		}*/

		if (!inf.Read(buf, sizeof(buf),&read_bytes)) {
			printf("fread() failed: %s ", out.c_str());
			goto error;
		}

		int flush = 0;
		if (read_bytes == 0) {
			if (!dec.Finalize()) {
				printf("AES256.Finalize() failed");
				goto error;
			}
			flush = Z_FINISH;
		}  else {
			if (!dec.Update(buf, read_bytes)) {
				printf("AES256.Update() failed");
				goto error;
			}
			flush = Z_NO_FLUSH;
		}

		strm.avail_in = dec.len();
		strm.next_in = const_cast<Bytef*>(dec.data());

		do {
			strm.avail_out = kBlockSize;
			strm.next_out = outbuf;
			ret = inflate(&strm, flush);
			assert (ret !=  Z_STREAM_ERROR);
			have = kBlockSize - strm.avail_out;
			switch (ret) {
				case Z_NEED_DICT:
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					printf("zlib::inflate() failed");
					goto error;
			}

			if (!outf.Write(outbuf, have,&written_bytes)) {
				printf("failed to write output: %s : %s", out.c_str());
				goto error;
			}
		} while (strm.avail_out == 0);

		if (ret == Z_STREAM_END) 
			break;
	}

	::inflateEnd(&strm);
	inf.Close();
	outf.Close();
	return SD_SUCCESSED;

error:
	::inflateEnd(&strm);
	if(inf.IsOpened())
	inf.Close();
	if(outf.IsOpened())
	outf.Close();
	return SD_FAILED;
}

SD_CODE AESHelper::DecFile(const ByteArray &key, const std::string &in, const std::string &out/*, IStoppingFlag *stop*/)
{
	const int kBlockSize = 4096;
	File inf, outf;
	if (!inf.OpenForRead(in))
		return SD_FAILED;

	if (!outf.OpenForWrite(out))
	{
		inf.Close();
		return SD_FAILED;
	}

	unsigned char buf[kBlockSize];
	Aes256Decrypt dec(key, kBlockSize);

	DWORD read_bytes, written_bytes;
	while (1)
	{
		/*if (stop != NULL && stop->IsStopping())
			goto failed;*/

		if (!inf.Read(buf, sizeof(buf),&read_bytes))
			goto failed;

		if (read_bytes == 0)
		{
			dec.Finalize();
			if (!outf.Write(dec.data(), dec.len(),&written_bytes)) 
				goto failed;

			inf.Close();
			outf.Close();
			return SD_SUCCESSED;
		} 

		
		dec.Update(buf, read_bytes);
		if (!outf.Write(dec.data(), dec.len(),&written_bytes)) 
			goto failed;
	}

failed:
	inf.Close();
	outf.Close();
	return SD_FAILED;
}

static ByteArray GenAesKeyFromPlainPassword(ByteArray &plain_pwd)
{
//	ByteArray pk_enc_key = ByteArray::fromRawStr(plain_pwd.c_str());
	int to_pad = (plain_pwd.size() >= 32 ? 0 : 32 - plain_pwd.size());
	plain_pwd.resize(32);
	for (int i = 0; i < to_pad; ++i)
		plain_pwd[32-i-1] = 0;

	return plain_pwd;
}

ByteArray AESHelper::DecryptPrivateKey( ByteArray &plain_pwd, const ByteArray &pk_cipher)
{
	ByteArray pk_enc_key = GenAesKeyFromPlainPassword(plain_pwd);
	return AESHelper::Decrypt256(pk_enc_key, pk_cipher);
}

ByteArray AESHelper::EncryptPrivateKey( ByteArray &plain_pwd, const ByteArray &pk_plain)
{
	ByteArray pk_enc_key = GenAesKeyFromPlainPassword(plain_pwd);
	return AESHelper::Encrypt256(pk_enc_key, pk_plain);
}
}}//namespace
