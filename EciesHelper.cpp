//  NSData_Aes256.h
//  SurDoc
//
//  Created by zhaojunjiang on 14/12/22.
//  Copyright (c) 2014年 sursen. All rights reserved.
//
#include "surbasic/Type.h"
#include "surbasic/EciesHelper.h"
#include "surbasic/AESHelper.h"
#include "surbasic/Base64Helper.h"

using namespace SDBasic::base64;
using namespace SDBasic::aes;

static const EC_Domain_Parameters &GetDomainParameters()
{
	static MutexLock _lock;
	static bool inited = false;
	static EC_Domain_Parameters s_nist_b_233;

	SD_Lock lock(_lock);
	if (inited == false)
	{
		use_NIST_B_233();
		s_nist_b_233 = NIST_B_233;
		inited = true;
	}
	return s_nist_b_233;
}

template <typename T>
std::string to_str(T &t)
{
	std::stringstream oss;
	oss << t;
	return oss.str();
}

template <typename T>
ByteArray to_bytearray(T &t)
{
	std::string str = to_str(t);
	return ByteArray::fromHexStr(str);
}

static ByteArray toByteArray(const OCTETSTR &v)
{
	ByteArray out;
	out.insert(out.begin(), v.begin(), v.end());
	return out;
}
namespace SDBasic{
namespace ecc{
EciesHelper::EciesHelper(const std::string &privkey_hex)
:sk_(NULL), pk_(NULL),  valid_(false)
{
	try {
		BigInt b = hexto_BigInt(privkey_hex);
		sk_ = new ECPrivKey(GetDomainParameters(), b);
		pk_ = new ECPubKey(*sk_);
		valid_ = true;
	} catch (borzoiException e) { 
		if (sk_ != NULL)
			delete sk_;
		if (pk_ != NULL)
			delete pk_;

		sk_ = NULL;
		pk_ = NULL;
		valid_ = false;
		return;
	}
}

EciesHelper::~EciesHelper()
{
	if (sk_ != NULL)
		delete sk_;
	if (pk_ != NULL)
		delete pk_;
}

bool EciesHelper::Encrypt(const ByteArray &plain, ByteArray &chipher)
{
	assert (valid_);
	try {
		ECIES enc(const_cast<ByteArray&>(plain), *pk_);
		DER der(enc);
		HexEncoder hex(der);
		const std::string s = to_str(hex);
		chipher = ByteArray::fromHexStr(s);
		return true;
	} catch (borzoiException e) { 
		return false;
	}
}

bool EciesHelper::Decrypt(const ByteArray &chiper, ByteArray &plain)
{
	assert (valid_);
	try { 
		DER der(chiper);
		ECIES decrypt = der.toECIES();
		OCTETSTR temp = decrypt.decrypt(*sk_);	
		plain = toByteArray(temp);
		return true;
	} catch (borzoiException e) { 
		//e.debug_print ();
		return false;
	}
}

bool EciesHelper::GenerateRandomPrivateKey(ByteArray &privkey, ByteArray &pubkey_x, ByteArray &pubkey_y)
{
	try { 
		ECPrivKey ec_priv_key = ECPrivKey(GetDomainParameters());
		privkey = to_bytearray(ec_priv_key.s);

		ECPubKey ec_pub_key = ECPubKey(ec_priv_key);
		pubkey_x = to_bytearray(ec_pub_key.W.x);
		pubkey_y = to_bytearray(ec_pub_key.W.y);
		return true;	
	} catch (borzoiException e) { 
		//e.debug_print ();
		return false;
	}
}

int EciesHelper::DecryptStorageKeyFromPersonalKey(const std::string &personal_key, ByteArray *storage_key)
{
  ByteArray ecies_cipher;
    if (!UrlBase64Helper::Decode(personal_key, ecies_cipher) || ecies_cipher.empty()){
        printf("personal_key is not valid urlbase64-encoded string");
        return 0;}
	
	 ByteArray base64StorageKey;
	if (!Decrypt(ecies_cipher, base64StorageKey)){
        printf("can't ECIES-decrypt personal_key to base64_storage_key");
        return 0;}
    if (base64StorageKey.size() != 44) {
        printf("base64_storage_key is not 44 bytes");
        return 0;}

    if (!Base64Helper::Decode(base64StorageKey, *storage_key)){
        printf("base64_storage_key is not valid base64-encoded string");
        return 0;}
    if (storage_key->size() != 32){
        printf("storage_key is not 32 bytes");
        return 0;}
    return 1;
}

EciesHelper* EciesHelper::CreateFromPrivateKeyReturnedByServer(const std::string &privkey) {
	ByteArray private_key_raw;
	bool ok = UrlBase64Helper::Decode(privkey, private_key_raw);
	if (!ok)
		return NULL;
	std::string s1 = private_key_raw.toHexStr();
	std::string s2 = private_key_raw.toRawStr();
	printf("privatekye hex:%s \n privatekey raw:%s \r\n",s1.c_str(),s2.c_str());
	return new EciesHelper(private_key_raw.toHexStr());
}
                           
int EciesHelper::TryParsePrivKey(const std::string &b64_pk_str, const std::string& plainpwd, ByteArray *result) {
	ByteArray pk_cipher;
	if (UrlBase64Helper::Decode(b64_pk_str, pk_cipher) == false) {
		pk_cipher.clear();
		if (Base64Helper::Decode(b64_pk_str, pk_cipher) == false) {
			printf("failed to decode(urlbase64) private key: %s", b64_pk_str.c_str());
			return 1;
		}
	}
	ByteArray ptmp = ByteArray::fromRawStr(plainpwd);
	std::string ss = ptmp.toRawStr();//test
	ByteArray pk_plain = AESHelper::DecryptPrivateKey(ptmp, pk_cipher);
	if (pk_plain.empty()) {
		printf("failed to decrypt(AES256) private key: %s", b64_pk_str.c_str());
		return 1;
	}

	*result = pk_plain;
	std::cout<<"pk_plain:"<< pk_plain.toHexStr() << endl;
	return 1;
}

int EciesHelper::TryParsePlainPrivKey(const std::string &b64_pk_str, ByteArray *result) {
	ByteArray pk_plain;
	if (UrlBase64Helper::Decode(b64_pk_str, pk_plain) == false) {
		if (Base64Helper::Decode(b64_pk_str, pk_plain) == false) {
			printf("failed to decode(urlbase64) plain private key: %s", b64_pk_str.c_str());
			return 0;
		}
	}

	result->swap(pk_plain);
	return 1;
}

}}//namespace
