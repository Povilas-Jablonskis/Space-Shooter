#ifndef ComponentPairHash_hpp
#define ComponentPairHash_hpp

#include <cstddef>

struct ComponentPairHash
{
	template <typename T>
	size_t operator()(T t) const
	{
		const size_t x = t.first->owner->instanceID->get();
		const size_t y = t.second->owner->instanceID->get();

		return x >= y ? x * x + x + y : y * y + y + x;
	}
};

#endif
