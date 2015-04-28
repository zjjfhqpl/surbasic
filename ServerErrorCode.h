#ifndef SERVERERRORCODE_H
#define SERVERERRORCODE_H
#include <string>
namespace SDBasic{
enum ServerErrorCode
{
	INVALID = 0,
	/**
	* I/O����
	*/
	IO_ERROR = 10000,

	/**
	* �������
	*/
	NETWORK_ERROR = 10001,

	/**
	* OAuth ����
	*/
	OAUTH_ERROR = 10010,

	/**
	* ��������
	*/
	PARAM_ERROR = 10011,

	/**
	* ��Դû�ҵ�������Ŀ¼���ļ�
	*/
	RESOURCE_NOT_FOUND = 10020,

	/**
	* Ŀ¼û�ҵ�
	*/
	FOLDER_NOT_FOUND = 10021,

	/**
	* �ļ�û�ҵ�
	*/
	FILE_NOT_FOUND = 10022,

	/**
	* ժҪû�ҵ�
	*/
	DIGEST_NOT_FOUND = 10023,

	/**
	* ��Դͬ����ͻ������Ŀ¼���ļ�
	*/
	RESOURCE_CONFLICT = 10030,

	/**
	* Ŀ¼ͬ����ͻ
	*/
	FOLDER_CONFLICT = 10031,

	/**
	* �ļ�ͬ����ͻ
	*/
	FILE_CONFLICT = 10032,

	/**
	* ��Դû�б��޸ģ�����Ŀ¼���ļ�
	*/
	RESOURCE_NOT_MODIFIED = 10033,

	/**
	* Ŀ¼û�б��޸�
	*/
	FOLDER_NOT_MODIFIED = 10034,

	/**
	* �ļ�û�б��޸�
	*/
	FILE_NOT_MODIFIED = 10035,

	/**
	* ��Դ��ֹ���ʣ�����Ŀ¼���ļ�
	*/
	RESOURCE_FORBIDDEN = 10060,

	/**
	* Ŀ¼��ֹ����
	*/
	FOLDER_FORBIDDEN = 10061,

	/**
	* �ļ���ֹ����
	*/
	FILE_FORBIDDEN = 10062,

	/**
	* �ܿռ�����
	*/
	OVER_STORAGE_QUOTA_TOTAL = 10071,

	/**
	* ���ĵ��ռ�����
	*/
	OVER_STORAGE_QUOTA_NON_DOC = 10072,

	/**
	* �������ĵ���С��������
	*/
	OVER_STORAGE_QUOTA_SINGLE_NON_DOC = 10073,

	/**
	* �ļ��ϴ�����
	*/
	FILE_UPLOAD_ERROR = 10080,

	/**
	* �洢����
	*/
	STORAGE_ERROR = 10090,

	/**
	* ���ݿ����
	*/
	DATABASE_ERROR = 10091,
};
}
#endif
