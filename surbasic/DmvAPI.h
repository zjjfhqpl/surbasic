#ifndef DMV_API_H
#define DMV_API_H
namespace dmvservers
{
   
		static const char * strcheckAccountsGetServer = "checkAccountsGetServer.do";
	namespace enterprise {

		 //enterprise login
		static const char* strLogin = "client/login";

		namespace v1 {
			namespace department_share { // deparment share interface
				//get current user all share folder
				static const char* strListShareRoots = "api/v1/sharefolders/list";
				static const char* strCreateFolder = "api/v1/sharefolders/folders";
				static const char* strListItems = "api/v1/sharefolders/folders/%s/items";  
				static const char* strMoveDir = "api/v1/sharefolders/folders/%s/move";
				static const char* strFoldersGeneral = "api/v1/sharefolders/folders/%s";

				static const char* strMoveFile = "api/v1/sharefolders/files/%s/move";
				static const char* strCopyFile = "api/v1/sharefolders/files/%s/copy";
				static const char* strFileGeneral = "api/v1/sharefolders/files/%s";

				static const char *strIfHashExists = "api/v1/sharefolders/files/security/isExist";
				static const char *strRapidUpload = "api/v1/sharefolders/files/security/rapidupload";
				static const char *strUploadSmall = "api/v1/sharefolders/files/security/upload";
				static const char *strDownload = "api/v1/sharefolders/files/security/%s/content";
				static const char *strResumeAddr = "api/v1/sharefolders/files/security/content?uploadType=resumable";
				//recycle				
				static const char* strDeleteFileToRecycleBin = "api/v1/sharefolders/files/%s/recycle";
				static const char* strDeleteFolderToRecycleBin = "api/v1/sharefolders/folders/%s/recycle";

				//share url
				static const char * strShareLink = "api/v1/sharefolders/files/%s/sharelink";
			}
		}
	}
	namespace openapi{
	  namespace v1{
			
			static const char *strUserSpaceInfo = "api/v1/space";
			namespace security
			{
				static const char *strIfHashExists = "api/v1/files/security/isExist";
				static const char *strRapidUpload = "api/v1/files/security/rapidupload";
				static const char *strUploadSmall = "api/v1/files/security/upload";
				static const char *strDownload = "api/v1/files/security";
				static const char *strResumeAddr = "api/v1/files/security/content?uploadType=resumable";
				static const char *strGetStorageKey = "api/dicom/%s/ciphertextKey";
			}
		}//v1
	
      namespace v2{
		static const char *strOAuthToken = "oauth/token";	
		static const char *strUserUpdateTime = "api/v2/space/update";
		static const char *strUserSpaceInfo = "api/v2/space";
		static const char *strFolderChanges = "api/v2/folders/changes";
		static const char *strFileChanges = "api/v2/files/changes";
		static const char *strFolderItems = "api/v2/folders"; //api/v2/folders/{folderId}/items?limit=2&offset=0	
		static const char *strFileVersion = "api/v2/files/version";
		static const char *strFolders = "api/v2/folders";
		static const char *strFiles = "api/v2/files";
		static const char *strFileUpload = "api/v2/files/content";
		static const char *strFileRapidUpload = "api/v2/files/rapidupload";
		
		
		namespace security{
		static const char *strIfHashExists = "api/v2/files/security/isExist";
		static const char *strRapidUpload = "api/v2/files/security/rapidupload";
		static const char *strUploadSmall = "api/v2/files/security/upload";
		static const char *strDownload = "api/v2/files/security";
		static const char *strResumeAddr = "api/v2/files/security/content?uploadType=resumable";
		static const char *strGetStorageKey = "api/dicom/%s/ciphertextKey";	
		}//v2-security

	  }//v2


	}//openapi
}

#endif
