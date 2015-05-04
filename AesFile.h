#ifndef _SURBASIC_AES_FILE_H_
#define _SURBASIC_AES_FILE_H_

#include "surbasic/File.h"
#include "surbasic/AESHelper.h"

namespace SDBasic {

class AesReadFile {
 public:
  AesReadFile();
  ~AesReadFile();

  bool Open(const std::string& path, const bytearray::ByteArray& key);
  bool Read(void* buf, size_t size, size_t* read_bytes);

 private:
  bool ReadNextBlock();
  static const size_t kBlockSize = 4096;
  file::File* file_;
  aes::Aes256Decrypt* dec_;
  unsigned char buf_[kBlockSize];
  size_t avail_;
  const unsigned char* pos_;
  bool eof_;
};

class AesWriteFile {
 public:
  AesWriteFile();
  ~AesWriteFile();

  bool Open(const std::string& path, const bytearray::ByteArray& key);
  bool Write(void* buf, size_t size, size_t* written_bytes);
  bool Finalize();
  void Close();

 private:
  static const size_t kBlockSize = 4096;
  file::File* file_;
  aes::Aes256Encrypt* enc_;
  unsigned char buf_[kBlockSize];
};

};

#endif // _SURBASIC_AES_FILE_H_
