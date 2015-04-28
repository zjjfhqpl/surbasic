//  NSData_Aes256.h
//  SurDoc
//
//  Created by zhaojunjiang on 14/12/22.
//  Copyright (c) 2014年 sursen. All rights reserved.
//
#ifndef ECIES_H
#define ECIES_H
#include <borzoi.h>
#include <nist_curves.h>
#include "surbasic/Type.h"
#include "surbasic/ByteArray.h"
#include "surbasic/Mutex.hpp"
using namespace SDBasic::bytearray;
using namespace SDBasic::lock;
// forward declaration
class ECPrivKey;
class ECPubKey;
namespace SDBasic{
namespace ecc{

class  EciesHelper
{
public:

	~EciesHelper();
	EciesHelper(const std::string &privkey_hex);

	bool Encrypt(const ByteArray &plain, ByteArray &chipher);
	bool Decrypt(const ByteArray &chiper, ByteArray &plain);

	bool Valid() const { return valid_; }
	const ECPrivKey& PrivKey() const { return *sk_; }
	const ECPubKey& PubKey() const { return *pk_; }

        int DecryptStorageKeyFromPersonalKey(const std::string &personal_key, ByteArray *storage_key);

	static bool GenerateRandomPrivateKey(ByteArray &privkey, ByteArray &pubkey_x, ByteArray &pubkey_y);

	
	static EciesHelper *CreateFromPrivateKeyReturnedByServer(const std::string &privkey);

	
	static int TryParsePrivKey(const std::string &b64_pk_str, const std::string &plainpwd, ByteArray *result);

	
	static int TryParsePlainPrivKey(const std::string &b64_pk_str, ByteArray *result);
private:
	ECPrivKey *sk_;
	ECPubKey *pk_;
	bool valid_;
};
}}//namespace
#endif
