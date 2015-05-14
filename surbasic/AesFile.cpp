#include "surbasic/AesFile.h"

namespace SDBasic {

AesReadFile::AesReadFile()
    : file_(new file::File()), dec_(NULL), avail_(0), eof_(false)
{
}

AesReadFile::~AesReadFile() {
  if (file_) 
    delete file_;
  if (dec_) 
    delete dec_;
}

bool AesReadFile::Open(const std::string& path, const bytearray::ByteArray& key) {
  if (file_->IsOpened())
    return false;
  if (file_->OpenForRead(path) == false)
    return false;
  dec_ = new aes::Aes256Decrypt(key, kBlockSize);
  return true;
}

bool AesReadFile::ReadNextBlock() {
  assert (avail_ == 0);
  if (eof_) {
    avail_ = 0;
    return true;
  }

  while (true) {
    unsigned long cipher_bytes = 0;
    if (file_->Read(buf_, kBlockSize, &cipher_bytes) == false)
      return false;

    if (cipher_bytes == 0) {
      if (!dec_->Finalize()) {
        return false;
      } 
      eof_ = true;
      break;
    } else {
      if (!dec_->Update(buf_, static_cast<int>(cipher_bytes))) {
        return false;
      }
      if (dec_->len() > 0)
        break;
    }
  }

  pos_ = dec_->data();
  avail_ = dec_->len();
  return true;
}

bool AesReadFile::Read(void* buf, size_t size, size_t* read_bytes) {
  (*read_bytes) = 0;

  if (avail_ == 0) {
    if (ReadNextBlock() == false)
      return false;

    if (avail_ == 0)
      return true;
  }

  size_t b = (size > avail_ ? avail_ : size);
  ::memcpy(buf, pos_, b);
  pos_ += b;
  avail_ -= b;
  (*read_bytes) = b;
  return true;
}

AesWriteFile::AesWriteFile()
: file_(new file::File()), enc_(NULL)
{
}

AesWriteFile::~AesWriteFile() {
  if (file_) 
    delete file_;
  if (enc_) 
    delete enc_;
}

bool AesWriteFile::Open(const std::string& path, const bytearray::ByteArray& key) {
  if (file_->IsOpened())
    return false;
  if (file_->OpenForWrite(path) == false)
    return false;
  enc_ = new aes::Aes256Encrypt(key, kBlockSize);
  return true;
}

bool AesWriteFile::Write(void* buf, size_t size, size_t* write_bytes) {
  (*write_bytes) = 0;

  unsigned long cipher_bytes = 0;
  size_t blocks = size / kBlockSize;
  size_t remain = size % kBlockSize;

  const unsigned char* pos = static_cast<const unsigned char*>(buf);
  for (size_t i = 0; i < blocks; ++i) {
    if (!enc_->Update(pos, kBlockSize)) {
      return false;
    }
    if (file_->Write(enc_->data(), enc_->len(), &cipher_bytes) == false)
      return false;
    (*write_bytes) += kBlockSize;
    pos += kBlockSize;
  }

  if (remain != 0) {
    if (!enc_->Update(pos, static_cast<int>(remain))) {
      return false;
    }
    if (file_->Write(enc_->data(), enc_->len(), &cipher_bytes) == false)
      return false;
    (*write_bytes) += remain;
  }

  assert (*write_bytes == size);
  return true;
}

bool AesWriteFile::Finalize() {
  unsigned long cipher_bytes = 0;
  enc_->Finalize();
  if (file_->Write(enc_->data(), enc_->len(), &cipher_bytes) == false)
    return false;
  file_->Flush();
  return true;
}

void AesWriteFile::Close() { file_->Close(); }

}
