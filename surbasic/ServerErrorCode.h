#ifndef SERVERERRORCODE_H
#define SERVERERRORCODE_H
#include <string>
namespace SDBasic{
enum ServerErrorCode
{
	INVALID = 0,
	/**
	* I/O错误
	*/
	IO_ERROR = 10000,

	/**
	* 网络错误
	*/
	NETWORK_ERROR = 10001,

	/**
	* OAuth 错误
	*/
	OAUTH_ERROR = 10010,

	/**
	* 参数错误
	*/
	PARAM_ERROR = 10011,

	/**
	* 资源没找到，包括目录与文件
	*/
	RESOURCE_NOT_FOUND = 10020,

	/**
	* 目录没找到
	*/
	FOLDER_NOT_FOUND = 10021,

	/**
	* 文件没找到
	*/
	FILE_NOT_FOUND = 10022,

	/**
	* 摘要没找到
	*/
	DIGEST_NOT_FOUND = 10023,

	/**
	* 资源同名冲突，包括目录与文件
	*/
	RESOURCE_CONFLICT = 10030,

	/**
	* 目录同名冲突
	*/
	FOLDER_CONFLICT = 10031,

	/**
	* 文件同名冲突
	*/
	FILE_CONFLICT = 10032,

	/**
	* 资源没有被修改，包括目录与文件
	*/
	RESOURCE_NOT_MODIFIED = 10033,

	/**
	* 目录没有被修改
	*/
	FOLDER_NOT_MODIFIED = 10034,

	/**
	* 文件没有被修改
	*/
	FILE_NOT_MODIFIED = 10035,

	/**
	* 资源禁止访问，包括目录与文件
	*/
	RESOURCE_FORBIDDEN = 10060,

	/**
	* 目录禁止访问
	*/
	FOLDER_FORBIDDEN = 10061,

	/**
	* 文件禁止访问
	*/
	FILE_FORBIDDEN = 10062,

	/**
	* 总空间已满
	*/
	OVER_STORAGE_QUOTA_TOTAL = 10071,

	/**
	* 非文档空间已满
	*/
	OVER_STORAGE_QUOTA_NON_DOC = 10072,

	/**
	* 单个非文档大小超过限制
	*/
	OVER_STORAGE_QUOTA_SINGLE_NON_DOC = 10073,

	/**
	* 文件上传错误
	*/
	FILE_UPLOAD_ERROR = 10080,

	/**
	* 存储错误
	*/
	STORAGE_ERROR = 10090,

	/**
	* 数据库错误
	*/
	DATABASE_ERROR = 10091,
};
}
#endif
