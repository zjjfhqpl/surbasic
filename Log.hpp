
#ifndef LOG_H
#define LOG_H
#include <string>
#define DEBUG 1
#ifdef DEBUG
#define NSLog(fmt,...) printf(("%s(%d)-%s:"fmt+"\r\n"),__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)
#else
#define NSLog(fmt,...)
#endif

namespace SDBasic{
	namespace Log{
	
		class LogScope
		{
		public:
			typedef enum _LogLevel
			{
				LOG_DEBUG = 0,	//调试
				LOG_NORMAL,		//一般
				LOG_MEDIUM,		//中
				LOG_HIGH		//高
			} ELogLevel;

			LogScope(ELogLevel level,const std::string & file,const std::string & line,const std::string & fun);
			~LogScope();
		private:
			std::string fun_;
			std::string line_;
			std::string file_;
		};
		inline LogScope::LogScope(ELogLevel level,const std::string & file,const std::string & line,const std::string & fun)
		{
			
		}
		inline LogScope::~LogScope()
		{

		}
	}
}
#endif
