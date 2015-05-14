#include "surbasic/CompressHelper.h"
#include "surbasic/File.h"
#include <zlib.h>
#include "surbasic/StrUtils.hpp"
//#include "surbasic/minizip/zip.h"
//#include "surbasic/minizip/unzip.h"
//#include "surbasic/minizip/iowin32.h"
using namespace SDBasic::file;

namespace SDBasic{
namespace zip{
#define CHUNK (64*1024)
enum ZlibCompressHeaderType {
	NO_HEADER = 0,
	ZLIB_HEADER = 1,
	GZIP_HEADER = 2,
};


static bool ZlibCompress(const std::string &inpath, const std::string &outpath, int level, 
		 ZlibCompressHeaderType header_type, IStoppingFlag *stop)
{
	bool  blret;

	File infile;
	blret = infile.OpenForRead(inpath);
	if (!blret)
		return false;

	File outfile;
	blret = outfile.OpenForWrite(outpath);
	if (!blret) {
		infile.Close();
		return false;
	}

	int ret, flush;
	unsigned have;
	z_stream strm;
	unsigned char in[CHUNK];
	unsigned char out[CHUNK];

	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	switch (header_type) {
	case NO_HEADER:
		ret = deflateInit2(&strm, level, Z_DEFLATED, -MAX_WBITS, 8, Z_DEFAULT_STRATEGY);
		break;
	case ZLIB_HEADER:
		ret = ::deflateInit(&strm, level); // will add zlib header
		break;
	case GZIP_HEADER:
		ret = deflateInit2(&strm, level, Z_DEFLATED, MAX_WBITS+16, 8, Z_DEFAULT_STRATEGY);
		break;
	}

	DWORD read_bytes, written_bytes;
	do {
		blret = infile.Read(in, CHUNK, &read_bytes);
		if (!blret) {
			infile.Close();
			outfile.Close();
			(void)deflateEnd(&strm);
			return false;
		}

		strm.avail_in = read_bytes;
		flush = (read_bytes == 0) ? Z_FINISH : Z_NO_FLUSH;
		strm.next_in = in;
		do {
			strm.avail_out = CHUNK;
			strm.next_out = out;
			ret = deflate(&strm, flush);
			assert (ret !=  Z_STREAM_ERROR);
			have = CHUNK - strm.avail_out;
			blret = outfile.Write(out, have, &written_bytes);
			if (!blret) {
				infile.Close();
				outfile.Close();
				(void)deflateEnd(&strm);
				return false;
			}
		} while (strm.avail_out == 0);

		assert (strm.avail_in == 0);
	} while (flush != Z_FINISH && !stop->IsStopping());

	if (stop->IsStopping()) {
		infile.Close();
		outfile.Close();
		return false;
	} else {
		assert (ret == Z_STREAM_END);
		(void)deflateEnd(&strm);
		infile.Close();
		outfile.Close();
		return true;
	}
}


static bool ZlibUncompress(const std::string &inpath, const std::string &outpath, 
			 ZlibCompressHeaderType header_type, IStoppingFlag *stop)
{
	bool blret;

	File infile;
	blret = infile.OpenForRead(inpath);
	if (!blret)
		return false;

	File outfile;
	blret = outfile.OpenForWrite(outpath);
	if (!blret) {
		infile.Close();
		return false;
	}

	int ret, flush;
	unsigned have;
	z_stream strm;
	unsigned char in[CHUNK];
	unsigned char out[CHUNK];

	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;

	switch (header_type) {
	case NO_HEADER:
		ret = inflateInit2(&strm, -MAX_WBITS);
		break;
	case ZLIB_HEADER:
		ret = inflateInit(&strm);
		break;
	case GZIP_HEADER:
		ret = inflateInit2(&strm, MAX_WBITS+32);
		break;
	}

	DWORD read_bytes, written_bytes;
	do {
		blret = infile.Read(in, CHUNK, &read_bytes);
		if (!blret) {
			infile.Close();
			outfile.Close();
			(void)inflateEnd(&strm);
			return false;
		}

		strm.avail_in = read_bytes;
		flush = (read_bytes == 0) ? Z_FINISH : Z_NO_FLUSH;
		strm.next_in = in;
		do {
			strm.avail_out = CHUNK;
			strm.next_out = out;
			ret = inflate(&strm, flush);
			assert (ret !=  Z_STREAM_ERROR);
			have = CHUNK - strm.avail_out;
			switch (ret) {
				case Z_NEED_DICT:
					ret = Z_DATA_ERROR;     /* and fall through */
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					infile.Close();
					outfile.Close();
					(void)inflateEnd(&strm);
					return false;
			}
			blret = outfile.Write(out, have, &written_bytes);
			if (!blret) {
				infile.Close();
				outfile.Close();
				(void)inflateEnd(&strm);
				return false;
			}
		} while (strm.avail_out == 0);

		assert (strm.avail_in == 0);
	} while (ret != Z_STREAM_END && !stop->IsStopping());

	if (stop->IsStopping()) {
		infile.Close();
		outfile.Close();
		return false;
	} else {
		assert (ret == Z_STREAM_END);
		(void)inflateEnd(&strm);
		infile.Close();
		outfile.Close();
		return (ret == Z_STREAM_END);
	}
}

/*
static bool ZipFile(const std::string &srcpath, const std::string &outpath, IStoppingFlag *stop)
{
	char buf[CHUNK];

	zlib_filefunc64_def ffunc;
	fill_win32_filefunc64W(&ffunc);

	zipFile zf = zipOpen2_64(outpath.c_str(), 0, NULL, &ffunc);
	if (zf == NULL)
		return false;

	zip_fileinfo zi;
	memset(&zi, 0, sizeof(zi));

	int err = zipOpenNewFileInZip3_64(zf, "", &zi,
		NULL, 0, NULL, 0, NULL,
		Z_DEFLATED, 1, 0, -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
		NULL, 0, 1);

	if (err != ZIP_OK)
	{
		zipCloseFileInZip(zf);
		return false;
	}

	bool blret;
	File inf;
	blret = inf.OpenForRead(srcpath);
	if (!blret) {
		zipCloseFileInZip(zf);
		return false;
	}

	DWORD read_bytes = 0;
	do {
		err = ZIP_OK;
		blret = inf.Read(buf, CHUNK, &read_bytes);
		if (!blret) {		
			zipCloseFileInZip(zf);
			inf.Close();
			return false;
		}

		if (read_bytes > 0)
		{
			err = zipWriteInFileInZip(zf, buf, read_bytes);
			if (err < 0) {
				zipCloseFileInZip(zf);
				inf.Close();
				return false;
			}
		}
	} while ((err == ZIP_OK) && (read_bytes>0));

	inf.Close();
	if (zipClose(zf, NULL) != ZIP_OK)
		return false;

	return true;
}


static bool UnzipFile(const std::string &srcpath, const std::string &outpath, IStoppingFlag *stop)
{
	char buf[CHUNK];

	zlib_filefunc64_def ffunc;
	fill_win32_filefunc64W(&ffunc);

	unzFile uf = unzOpen2_64(srcpath.c_str(), &ffunc);
	if (uf == NULL)
		return false;

	unz_global_info64 gi;
	int err = unzGetGlobalInfo64(uf, &gi);
	if (err != UNZ_OK)
	{
		unzClose(uf);
		return false;
	}

	if (gi.number_entry != 1)
	{
		unzClose(uf);
		return false;
	}

	unz_file_info64 file_info;
	char filename[512];
	err = unzGetCurrentFileInfo64(uf, &file_info, filename, sizeof(filename), NULL, 0, NULL, 0);
	if (err != UNZ_OK)
	{
		unzClose(uf);
		return false;
	}

	bool blret;
	File outf;
	blret = outf.OpenForWrite(outpath);
	if (!blret) {
		unzClose(uf);
		return false;
	}

	err = unzOpenCurrentFilePassword(uf, NULL);
	if (err!=UNZ_OK) {
		unzClose(uf);
		return false;
	}

	do {
		err = unzReadCurrentFile(uf, buf, CHUNK);
		if (err < 0) {
			unzClose(uf);
			outf.Close();
			return false;
		}

		if (err == 0)
			break;

		DWORD written_bytes;
		blret = outf.Write(buf, err, &written_bytes);
		if (!blret || written_bytes != err) {
			unzClose(uf);
			outf.Close();
			return false;
		}
	} while (true);

	outf.Close();
	if (unzCloseCurrentFile(uf) != UNZ_OK) {
		unzClose(uf);
		return false;
	}

	if (unzClose(uf) != UNZ_OK)
		return false;

	return true;
}*/

bool CompressHelper::Compress(const std::string &inpath, const std::string &outpath, 
			IStoppingFlag *stop, CompressMethod method) 
{
	switch (method) {
	case DEFLATE:
		return ZlibCompress(inpath, outpath, Z_DEFAULT_COMPRESSION, ZLIB_HEADER, stop);
	case GZIP:
		return ZlibCompress(inpath, outpath, Z_DEFAULT_COMPRESSION, GZIP_HEADER, stop);
	case DEFLAT_RAW:
		return ZlibCompress(inpath, outpath, Z_DEFAULT_COMPRESSION, NO_HEADER, stop);
	case ZIP:
//		return ZipFile(inpath, outpath, stop);	
	default:
		return false;
	}
}

bool CompressHelper::Uncompress(const std::string &inpath, const std::string &outpath, 
				IStoppingFlag *stop, CompressMethod method) 
{
	switch (method) {
	case DEFLATE:
		return ZlibUncompress(inpath, outpath, ZLIB_HEADER, stop);
	case GZIP:
		return ZlibUncompress(inpath, outpath, GZIP_HEADER, stop);
	case DEFLAT_RAW:
		return ZlibUncompress(inpath, outpath, NO_HEADER, stop);
	case ZIP:
//		return UnzipFile(inpath, outpath, stop);	
	default:
		return false;
	}
}

}}//namespace

