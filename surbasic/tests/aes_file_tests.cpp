#include <gwbase/base/util.h>
#include <gwbase/base/random.h>
#include <surbasic/Digestor.h>
#include <surbasic/AesFile.h>

using namespace dicom;
using namespace SDBasic;

static void CreateTempFile(const std::string& path, size_t size) {
  WritableFilePtr f;
  Status s = WritableFile::Open(path, true, &f);
  if (!s.ok()) {
    LOG_ERROR << "failed to create file: " << path;
    exit(-1);
  }

  const size_t kBlockSize = 1024;
  char buf[kBlockSize];
  size_t blocks = size / kBlockSize;
  size_t remain = size % kBlockSize;

  for (size_t i = 0; i < blocks; ++i)
    f->Append(Slice(buf, kBlockSize));
  if (remain > 0)
    f->Append(Slice(buf, remain));
  f->Sync();
}

class AesFileTests { 
 public:
  AesFileTests() { 
    key_ = bytearray::ByteArray::genRandom(32);
  }
  virtual ~AesFileTests() { }

 protected:
  bool TowFileAreSame(const std::string& f1,
                      const std::string& f2) 
  {
    std::string m1 = digestor::Digestor::MD5File(f1);
    std::string m2 = digestor::Digestor::MD5File(f2);
    return (!m1.empty() && m1 == m2);
  }

  void ReadTest(size_t file_size, size_t buf_size) {
    const std::string plain_file = "aes_test_plain.dat";
    const std::string plain_file_out = "aes_test_plain.out";
    const std::string aes_file = "aes_test_cipher.dat";
    dicom::DeleteFile(plain_file);
    dicom::DeleteFile(plain_file_out);
    dicom::DeleteFile(aes_file);

    CreateTempFile(plain_file, file_size);

    aes::AESHelper::EncFile(key_, plain_file, aes_file);

    AesReadFile rf;
    EXPECT_TRUE(rf.Open(aes_file, key_));

    file::File out;
    EXPECT_TRUE(out.OpenForWrite(plain_file_out));

    char* buf = new char[buf_size];
    size_t bytes = 0;
    while (true) {
      EXPECT_TRUE(rf.Read(buf, buf_size, &bytes));
      if (bytes == 0)
        break;
      size_t written_bytes = 0;
      EXPECT_TRUE(out.Write(buf, bytes, &written_bytes));
    }
    out.Close();

    EXPECT_TRUE(TowFileAreSame(plain_file, plain_file_out));
    dicom::DeleteFile(plain_file);
    dicom::DeleteFile(plain_file_out);
    dicom::DeleteFile(aes_file);
  }

  void WriteTest(size_t file_size, size_t buf_size) {
    const std::string plain_file = "aes_test_plain.dat";
    const std::string aes_file_out = "aes_test_ciper.out";
    const std::string aes_file = "aes_test_cipher.dat";
    dicom::DeleteFile(plain_file);
    dicom::DeleteFile(aes_file_out);
    dicom::DeleteFile(aes_file);

    CreateTempFile(plain_file, file_size);

    aes::AESHelper::EncFile(key_, plain_file, aes_file_out);

    AesWriteFile wf;
    EXPECT_TRUE(wf.Open(aes_file, key_));

    file::File in;
    EXPECT_TRUE(in.OpenForRead(plain_file));

    char* buf = new char[buf_size];
    size_t bytes = 0;
    while (true) {
      EXPECT_TRUE(in.Read(buf, buf_size, &bytes));
      if (bytes == 0)
        break;
      size_t written_bytes = 0;
      EXPECT_TRUE(wf.Write(buf, bytes, &written_bytes));
      EXPECT_Eq(written_bytes, bytes);
    }
    wf.Finalize();
    in.Close();

    EXPECT_TRUE(TowFileAreSame(aes_file_out, aes_file));
    dicom::DeleteFile(plain_file);
    dicom::DeleteFile(aes_file_out);
    dicom::DeleteFile(aes_file);
  }

  bytearray::ByteArray key_;
};

class ReadStreamTests: public AesFileTests {
 public:
  void RunTests() {
    ReadTest(0, 1); 
    ReadTest(1, 1); 
    ReadTest(128, 128); 
    ReadTest(4096, 1); 
    ReadTest(4096, 4096); 
    ReadTest(4097, 4096); 
    ReadTest(10000, 4096); 
  }
}

class WriteStreamTests: public AesFileTests {
 public:
  void RunTests() {
    WriteTest(0, 1); 
    WriteTest(1, 1); 
    WriteTest(128, 128); 
    WriteTest(4096, 1); 
    WriteTest(4096, 4096); 
    WriteTest(4097, 4096); 
    WriteTest(10000, 4096); 
  }
}

int main()
{
  ReadStreamTests t1;
  t1.RunTests();
}
