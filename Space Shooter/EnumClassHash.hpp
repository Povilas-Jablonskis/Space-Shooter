#ifndef EnumClassHash_hpp
#define EnumClassHash_hpp

#include <cstddef>

struct EnumClassHash
{
	template <typename T>
	size_t operator()(T t) const
	{
		return static_cast<size_t>(t);
	}
};

#endif
