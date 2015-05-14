//Author:zhaojunjiang 2015-4-7
#ifndef COMPRESSHELPER_H
#define COMPRESSHELPER_H
#include "surbasic/Type.h"
#include "surbasic/StoppingFlag.hpp"
namespace SDBasic{
namespace zip{
enum  CompressMethod {
	DEFLATE = 0,
	GZIP,
	DEFLAT_RAW,
	ZIP,
};

class  CompressHelper
{
public:
	static bool Compress(const std::string &inpath, const std::string &outpath, 
		IStoppingFlag *stop, CompressMethod method = DEFLATE);

	static bool Uncompress(const std::string &inpath, const std::string &outpath, 
		IStoppingFlag *stop, CompressMethod method = DEFLATE);
};


}}//namespace

#endif

