//Author:zhaojunjiang 2015-3-17 
#ifndef DEFINE_H
#define DEFINE_H
namespace SDBasic{
namespace code{
#define ENT_KEY 1  

enum ShareType {
	//General folder or file,none share              
	SHARE_NONE = 0,              
	//General share folder interface ,add shareFolder=true&rootShareFolderId=xx	
	SHARE_PERSONAL = 50,               
	//Deparment share folder interface,url address diffrent but add sharedFolderId=xx param 
	SHARE_ENTERPRISE_DEPAREMENT = 100, 
};

enum FileConvertType
{
	DOC_AND_CONVERTABLE,     // doc convert: txt, doc, pdf ...
	DOC_AND_NONCONVERTABLE,  // doc not convert: mp3, avi, ...
	NONDOC,                  // none doc: exe, ...
};

enum BACKUP_TYPE
{
	BACKUP_FOLDER = 0,  //folder
	BACKUP_SRC, 	 	  //src file
	BACKUP_STREAM,    ///stream file
	BACKUP_SVG, 		  ///svg file
	BACKUP_IMAGE      ///image file
};

enum BACKUP_STATUS
{
	NO_BACKUP = 0,		// *
	START_BACKUP, 		//
	PAUSE_BACKUP, 		//
	RESUME_BACKUP, 		//
	FAILED_BACKUP, 	 	//
	SUCCESS_BACKUP, 	//
	PART_BACKUP, 		//
	RETRY_CONVERT_FIRST,	//
	RETRY_CONVERT_SECOND,	//
	STOP_BACKUP,		    //
	EXCEPTION_BACKUP,       //
	UPLOADING,              //

	FAILED_BACK_FILE10MLIMT,    

	FAILED_BACK_NONDOCSPACELESS,
	FAILED_BACK_TOATLSPACELESS     
};
enum CREATE_TYPE 
{
	FILE_CREATE_NEW	= 0,
	FILE_REPLACE,
	FILE_EXIST
};

enum FolderType
{
	FTROOT   = 0,
	FTPublic = 1,
	FTReveive = 2,
	FTRecycle = 3,
	FTSync = 4,
	FTMobile=5,
	FTShare = 6,
	FTShareCreator = 7, 
	FTDepartmentShareVritualRoot = 8,
	FTDepartmentShareRoot = 9,
	FTDepartmentShareChild = 10,
	FTRecycleDir = 14,
	FTGeneralFolder = 15, 

	FTThird = 18,
	FTThirdChild = 19,

	FTFile = 20,
	FTDepartmentShareFile = 21,

	FTRecycleFile = 22, 
	FTDownload   = 30,
	FTDownIng   = 31,
	FTDownSuccess    = 32,
	FTBackup     = 40,
	FTBackupIng  = 41,
	FTBackupSuccess = 42,
	FTOther = 50,
};

enum SD_CODE {
	SD_SUCCESSED         =1,
	SD_FAILED            =0,
	SD_ERROR             =-1,
	SD_NO_VALUE          =-10,

	SD_ERROR_POINTERNULL = 90001,
	
	SD_USER_NOT_EXIST,            //user not exist
	SD_PWD_ERROR,                 //pwd error

	SD_CONNECT_SERVER_ERROR,      // connect to server failure
	SD_RESLOVED_HOST,              //
	SD_TIMEOUT,                 // connect to server timeout 
	SD_SEND_ERROR,              
	SD_SERVER_ERROR,
	SD_FAILED_CONNECT_SERVER,
	SD_RESOLVE_HOST,
	
	SD_BACKUP_TOTAL_SPACE_LESS,
	SD_BACKUP_NON_DOC_SPACE_LESS,
	SD_BACKUP_FILE_10M_LIMIT,
	SD_BACKUP_FILE_LIMIT,
	SD_BACKUP_TIMEOUT,
	//open api returns non-200 http code
	SD_OPENAPI_NO_CONTENT,                  // 204
	SD_OPENAPI_NOT_MODIFIED,               //304
	SD_OPENAPI_308,                        //308
	SD_OPENAPI_BAD_REQUESET,               // 400
	SD_OPENAPI_INVALID_TOKEN,		// 401
	SD_OPENAPI_INVALID_OAUTH_REQUEST,      //403
	SD_OPENAPI_FILE_NOT_FOUND,             //404
	SD_OPENAPI_METHOD_UNEXPECTED,          // 405
	SD_OPENAPI_FILE_OR_DIR_EXISTS,        // 409
	SD_OPENAPI_SERVER_FILE_CHANGED,       //  412

	SD_INVALID_DCM_FILE,
	SD_CANCELLED,
	SD_NOT_FOUND,

	SD_BACKUP_DEST_DIR_NOT_FOUND, // ¿¿¿¿¿¿¿¿¿¿¿¿¿¿¿
	SD_SKIP_BY_FILTER,            // ¿¿¿¿¿¿¿¿¿¿¿¿
	SD_FAILED_ENCKEY,
	SD_STOP_ENCANDUNCOMPRESS,
	SD_FILE_NOENCANDUNCOMPRESS,
	SD_FILE_OPENERROR,

	SD_FILE_EXIST,                    
	SD_FILE_NEED_UPLOAD,             
	SD_FILE_CREATE_AND_NOUPLOAD,       
	SD_LOCAL_FILE_ERROR,               
	SD_FILE_BEING_WRITTING,  
	SD_FILE_DIGEST_EXIST,	
};


}}
#endif
