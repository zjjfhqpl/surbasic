#ifndef STR_UNTILS_H
#define STR_UNTILS_H
#include "Type.h"
#include "nocopyable.hpp"
#include <sstream>
namespace SDBasic{
	namespace fmt{
    template <typename T>
    std::string NumStr(const T& v) {
      std::ostringstream os;
      os << v;
      return os.str();
    }

		class  NumFmt:public SDBasic::nocopyable
		{
		public:
			template<typename T>
			NumFmt(const char * fmt,T num);
			const char * c_str()const{return data_;}
			const std::string str() const {return data_;}
		private:
			char data_[64];
		};
		template<typename	T>
		inline NumFmt::NumFmt(const char * fmt,T num)
		{
			sprintf(data_,fmt,num);
		}
		inline std::ostream&operator <<(std::ostream &stream,const NumFmt&fmt)
		{
		stream<<fmt.c_str();
			return stream;
		}
		inline std::string & operator +=(std::string &str,const NumFmt&fmt)
		{
			str+=fmt.str();
			return str;
		}
		inline std::string operator +(std::string &str,const NumFmt & fmt)
		{
			return str+fmt.str();
		}
}}//namespace
#endif
