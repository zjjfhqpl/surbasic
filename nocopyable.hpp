//add by zhaojunjiang 2015-2-5
#ifndef NOCOPYABLE_H
#define NOCOPYABLE_H

namespace SDBasic{
	namespace nocopyable_{

		class nocopyable
		{
		protected:
			nocopyable(){}
			~nocopyable(){}
		private:
			nocopyable(const nocopyable &);
			const nocopyable& operator=(const nocopyable&);
		};
	}

typedef nocopyable_::nocopyable nocopyable;
}
#endif
