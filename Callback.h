#ifndef CALLBACK_H
#define CALLBACK_H
namespace SDBasic{

template<typename FunType>
class Callback
{
public:
	Callback(void)
	:func(NULL),userdata(NULL){}
	
	Callback(FunType f,void * d)
	:func(f),userdata(d){}

	Callback(const Callback * other)
	{
	    func = other.func;
	    userdata = other.userdata;
	}
	
	Callback& operator = (const Callback * other)
	{
	   func = other.func;
	   userdata = other.userdata;
	  return *this;
	}					

	

	FunType func;
	void * userdata;
	
};
}
#endif
