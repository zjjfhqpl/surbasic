#include "surbasic/./ecc/borzoi.h"    

#include "surbasic/./ecc/nist_curves.h" 

#include <fstream>
#include <sstream>
using namespace std;

typedef unsigned char uint8_t;
uint8_t hex_to_int(uint8_t ch)
{
	if (ch >= 'a' && ch <= 'z')
		return ch - 'a' + 10;

	if (ch >= '0' && ch <= '9')
		return ch - '0';

	if (ch >= 'A' && ch <= 'Z')
		return ch - 'A' + 10;

	assert (false);
	return 0;
}

void hexstr_to_octstr(const std::string &hex, OCTETSTR &bytes)
{
	for (unsigned i = 0; i < hex.length(); i+=2)
	{
		uint8_t high = hex_to_int(hex[i]);
		uint8_t low = hex_to_int(hex[i+1]);
		bytes.push_back((high << 4) | low);
	}
}

template <typename T>
std::string to_str(T &t)
{
	std::stringstream oss;
	oss << t;
	return oss.str();
}

OCTETSTR str_to_octstr(const std::string &str)
{
	OCTETSTR out;
	for (unsigned i = 0; i < str.length(); ++i)
		out.push_back((unsigned char)str[i]);
	return out;
}

void dec_data(const std::string &encdata, ECPrivKey &sk)
{
	OCTETSTR bytes;
	hexstr_to_octstr(encdata, bytes);
	try { 
		DER der2(bytes);
		ECIES decrypt = der2.toECIES();
		OCTETSTR plain2 = decrypt.decrypt(sk);	
		std::cout << "decrpyted data: " << std::endl;
		for (int i=0; i<plain2.size(); i++)
			std::cout << plain2[i];
		std::cout << std::endl;
	} catch (borzoiException e) { 
		e.debug_print ();
		return;
	}
}

void ecies_parse_server_data()
{
	use_NIST_B_233();
	EC_Domain_Parameters dp = NIST_B_233;
	BigInt skInt = hexto_BigInt("be80557484496ca677ab1be947848ae8fbb307a2");
	ECPrivKey sk(dp, skInt);

	dec_data("308201383082010c3081c706072a8648ce3d02013081bb0201013012020200e906092a8648ce3d0102030202014a3040041e000000000000000000000000000000000000000000000000000000000001041e0066647ede6c332c7f8c0923bb58213b333b20e9ce4281fe115f7d8f90ad043d0400fac9dfcbac8313bb2139f1bb755fef65bc391f8b36f8f8eb7371fd558b01006a08a41903350678e58528bebf8a0beff867a7ca36716f7e01f81052021e01000000000000000000000000000013e974e72f8a6922031d2603cfe0d7020102034000043d040065ca4048ca0f008356f6b045ca1420f1467accd2979d17e028c17bfa1e01ee46b5ad1172f5d9840f48e1544a9127cad0bc43fbc1ea971805c1205c0410f44b1a2eb6a1afad41b3443cb577fd41041490d3eb6476724a8499dcc74c58062322ac895d1b",
		sk);
}

void ecies_dec_without_user_privkey()
{
	use_NIST_B_233();
	EC_Domain_Parameters dp = NIST_B_233;

	const char *data = "308201383082010c3081c706072a8648ce3d02013081bb0201013012020200e906092a8648ce3d0102030202014a3040041e000000000000000000000000000000000000000000000000000000000001041e0066647ede6c332c7f8c0923bb58213b333b20e9ce4281fe115f7d8f90ad043d0400fac9dfcbac8313bb2139f1bb755fef65bc391f8b36f8f8eb7371fd558b01006a08a41903350678e58528bebf8a0beff867a7ca36716f7e01f81052021e01000000000000000000000000000013e974e72f8a6922031d2603cfe0d7020102034000043d040065ca4048ca0f008356f6b045ca1420f1467accd2979d17e028c17bfa1e01ee46b5ad1172f5d9840f48e1544a9127cad0bc43fbc1ea971805c1205c0410f44b1a2eb6a1afad41b3443cb577fd41041490d3eb6476724a8499dcc74c58062322ac895d1b";

	
	F2M fx = to_F2M("28e05bc600fceda4a2f02f248054214236e7f2045861e45b0dc5103f4a");
	F2M fy = to_F2M("3957865ce3204b82857423de9c2080b5949e099000d0ef06b1baedc4a0");
	ECPubKey K(dp, Point(fx, fy)); 

	
	BigInt skInt = decto_BigInt("123");
	ECPrivKey r(dp, skInt);

	OCTETSTR bytes;
	hexstr_to_octstr(data, bytes);
	try { 
		DER der2(bytes);
		ECIES decrypt = der2.toECIES();

		OCTETSTR P1, P2; 
		F2M z = ECSVDP_DH (K.dp, r.s, K.W);
		OCTETSTR Z = FE2OSP (z);
		OCTETSTR K = KDF2_SURDOC (Z, 32, P1); 

		OCTETSTR K1; 
		OCTETSTR K2; 
		K1.insert(K1.end(), K.begin(), K.begin() + 16);
		K2.insert(K2.end(), K.begin()+16, K.end());

		OCTETSTR M = AES_CBC_IV0_Decrypt (K1, decrypt.C);
		if (decrypt.T != MAC1 (K2, decrypt.C||P2)) {
			throw borzoiException ("ECIES: tag invalid");
		}

		std::cout << "decrpyted data: " << std::endl;
		for (int i=0; i<M.size(); i++)
			std::cout << M[i];
		std::cout << std::endl;
	} catch (borzoiException e) { 
		e.debug_print ();
		return;
	}
}


std::string enc_data(ECPubKey &K, OCTETSTR &plain)
{
	ECIES ec(plain, K);
	DER der1(ec);
	HexEncoder hexstr1(der1);

	return to_str(hexstr1);
}

void ecies_enc_dec_test()
{
	use_NIST_B_233();
	EC_Domain_Parameters dp = NIST_B_233;
	BigInt skInt = hexto_BigInt("be80557484496ca677ab1be947848ae8fbb307a2");
	ECPrivKey sk(dp, skInt);
	ECPubKey pk(sk);

	std::cout << "private_key: " << std::endl;
	std::cout << sk.s << std::endl;

	std::string pkx = to_str(pk.W.x);
	std::string pky = to_str(pk.W.y);

	std::cout << "public_key: " << std::endl;
	std::cout << "X: " << pk.W.x << std::endl;
	std::cout << "Y: " << pk.W.y << std::endl;


	F2M fx = to_F2M(pkx.c_str());
	F2M fy = to_F2M(pky.c_str());
	
	ECPubKey pk2(dp, Point(fx, fy));

	use_NIST_B_233();	
	OCTETSTR plain = str_to_octstr("11111111111111111111111111111111");
	ECIES encrypt(plain, pk2);

	std::cout << "ECIES object: " << std::endl;
	std::cout << encrypt << std::endl;

	DER der1(encrypt);
	HexEncoder hexstr1(der1);

	std::string encdata = to_str(hexstr1);
	std::cout << "encrypted data: " << std::endl;
	std::cout << encdata << std::endl;

	dec_data(encdata, sk);
}

void test_gen_rand()
{
	for (;;)
	{
		BigInt a = GenRandom(163);
		BigInt b = hexto_BigInt(to_str(a));

		std::cout << to_str(a) << std::endl;

		assert (a == b);

		if (a.numBits() != 163)
			break;
	}
}

void test_der_public_key()
{
	use_NIST_B_233();
	EC_Domain_Parameters dp = NIST_B_233;
	BigInt skInt = hexto_BigInt("be80557484496ca677ab1be947848ae8fbb307a2");
	ECPrivKey sk(dp, skInt);
	ECPubKey pk(sk);

	DER der1(pk);
	HexEncoder hexstr1(der1);
	std::string encdata = to_str(hexstr1);
	std::cout << "DER encoded public key: " << std::endl;
	std::cout << encdata << std::endl;
}

extern OCTETSTR DER_Encode (Point P);
extern Point DER2Point (OCTETSTR der_rep);
extern OCTETSTR DER_Encode (OCTETSTR in);
extern OCTETSTR DER_Seq_Encode (std::vector<OCTETSTR> in);
extern OCTETSTR DER_Seq_Encode (OCTETSTR der_rep);
extern std::vector<OCTETSTR> DER_Seq_Decode (OCTETSTR in);
extern OCTETSTR DER2OCTETSTR (OCTETSTR der_rep);

OCTETSTR ecies_trim_encode(const ECIES &ct)
{
	std::vector<OCTETSTR> buf;
	buf.push_back(DER_Encode(ct.V.W));
	buf.push_back(DER_Encode(ct.C));
	buf.push_back(DER_Encode(ct.T));
	return DER_Seq_Encode(buf);
}

ECIES ecies_trim_decode(const EC_Domain_Parameters &dp, const OCTETSTR &data)
{
	std::vector<OCTETSTR> buf = DER_Seq_Decode(data);
	Point p = DER2Point(buf[0]);
	ECPubKey pk(dp, DER2Point(buf[0]));
	return ECIES(pk, DER2OCTETSTR(buf[1]), DER2OCTETSTR(buf[2]));
}

void test_trim_der()
{
	use_NIST_B_233();
	EC_Domain_Parameters dp = NIST_B_233;
	BigInt skInt = hexto_BigInt("be80557484496ca677ab1be947848ae8fbb307a2");
	ECPrivKey sk(dp, skInt);
	ECPubKey pk(sk);

	OCTETSTR plain = str_to_octstr("11111111111111111111111111111111");
	ECIES encrypt(plain, pk);
	std::cout << "Original ECIES object: " << std::endl;
	std::cout << encrypt << std::endl;

	OCTETSTR der = ecies_trim_encode(encrypt);
	HexEncoder hex(der);
	std::cout << "DER(trim): \n" << to_str(hex) << std::endl;
	ECIES decrypt = ecies_trim_decode(dp, der);
	std::cout << "Decoded ECIES object: " << std::endl;
	std::cout << decrypt << std::endl;

	try { 
		OCTETSTR plain2 = decrypt.decrypt(sk);	
		std::cout << "decrpyted data: " << std::endl;
		for (int i=0; i<plain2.size(); i++)
			std::cout << plain2[i];
		std::cout << std::endl;
	} catch (borzoiException e) { 
		e.debug_print ();
		return;
	}
}

int main()
{
	test_trim_der();
	test_der_public_key();
	test_gen_rand();
	std::cout << "--------------------------------------------------" << std::endl;
	ecies_dec_without_user_privkey();
	std::cout << "--------------------------------------------------" << std::endl;
	ecies_enc_dec_test();
	std::cout << "--------------------------------------------------" << std::endl;
	ecies_parse_server_data();
	return 1;
}
