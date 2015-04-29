#include <gwbase/base/test_harness.h>
#include <gwbase/base/util.h>
#include <gwbase/base/random.h>
#include <surbasic/surbasic.h>

const	std::string server = "https://open.surdoc.com";
const	std::string user = "surdoctestooJUvF20@cc.cc";
const	std::string password ="aA111111";

using namespace dicom;

class SurbasicOpTests 
{ 
 public:

 protected:
  void DoLogin() {
    SDBasic::AccessToken token;
    uint64_t result = SDBasic::Login(server, user, password, token);
    if (result == 0) {
      LOG_ERROR << "failed to login";
      exit(-1);
    }
    token_key_ = result;

    bool ret = SDBasic::GetUserSpaceInfo(token_key_, uinfo_);
    if (!ret) {
      LOG_ERROR << "failed to get user space info";
      exit(-1);
    }
  }

  void ExitIfFalse(bool result, const char* msg) {
    if (!result) {
      LOG_ERROR << msg;
      exit(-1);
    }
  }

  void CreateTempFile(const std::string& path, size_t size) {
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

  std::string GenRandomName(size_t n) {
    std::string name;
    for (size_t i = 0; i < n; ++i) {
      name.push_back(static_cast<char>(rand_.Uniform(26) + 'a'));
    }
    return name;
  }
  
  uint64_t token_key_;
  UserSpaceInfo uinfo_;
  dicom::Random rand_;
};

TEST(SurbasicOpTests, FolderOperations) {
  DoLogin();
  SDBasic::TreeEntity node, node1;
  std::string test_folder_name = GenRandomName(16);
  bool result = SDBasic::CreateFolder(token_key_, 
                                      uinfo_.rootDirId,
                                      test_folder_name,
                                      node);
  ExitIfFalse(result, "failed to create folder");

  std::string folder_name_2 = GenRandomName(16);
  result = SDBasic::RenameFolder(token_key_,
                                 node.id,
                                 folder_name_2,
                                 node1);
  EXPECT_TRUE(result);
  EXPECT_Eq(node.id, node1.id);

  result = SDBasic::DeleteFolder(token_key_, 
                                 node.id,
                                 true);
  EXPECT_TRUE(result);
}

TEST(SurbasicOpTests, FileOperations) {
  DoLogin();
  SDBasic::TreeEntity node, node1;

  // create two folders
  std::string test_folder_name = GenRandomName(16);
  bool result = SDBasic::CreateFolder(token_key_, 
                                      uinfo_.rootDirId,
                                      test_folder_name,
                                      node);
  EXPECT_TRUE(result);
  std::string folder_name_2 = GenRandomName(16);
  result = SDBasic::CreateFolder(token_key_, 
                                 uinfo_.rootDirId,
                                 folder_name_2,
                                 node1);
  EXPECT_TRUE(result);


  // upload a file
  SDBasic::TreeEntity fnode, fnode1, fnode2;
  std::string test_file_path = GenRandomName(16);
  std::string test_file_path2 = GenRandomName(16);
  CreateTempFile(test_file_path, 1024);
  result = SDBasic::Upload(token_key_,
                           node.id,
                           test_file_path,
                           test_file_path,
                           SDBasic::time::Timestamp::Now(),
                           fnode);
  DeleteFile(test_file_path);
  if (!result) {
    LOG_ERROR << "failed to upload file";
    goto cleanup;
  }

  result = SDBasic::RenameFile(token_key_,
                               fnode.id,
                               test_file_path2,
                               fnode1);
  if (!result) {
    LOG_ERROR << "failed to rename file";
    goto cleanup;
  }

  EXPECT_Eq(fnode1.name, test_file_path2);
  EXPECT_Eq(fnode1.id, fnode.id);

  result = SDBasic::MoveFile(token_key_,
                             fnode1.id,
                             node1.id,
                             fnode2);
  if (!result) {
    LOG_ERROR << "failed to move file";
    goto cleanup;
  }

  EXPECT_Eq(fnode2.parentid, node1.id);
  result = true;

cleanup:
  if (!node.id.empty()) {
    SDBasic::DeleteFolder(token_key_, node.id, true);
  }
  if (!node1.id.empty()) {
    SDBasic::DeleteFolder(token_key_, node1.id, true);
  }
  EXPECT_TRUE(result);
}

int main()
{
  dicom::RunAllTests();
}
