#ifndef ComponentPairHash_hpp
#define ComponentPairHash_hpp

#include <cstddef>
#include <memory>

struct ComponentPairHash
{
	template <typename T>
	std::size_t operator()(T t) const
	{
		const std::size_t x = t.first->owner->instanceID->get();
		const std::size_t y = t.second->owner->instanceID->get();

		return x >= y ? x * x + x + y : y * y + y + x;
	}
};

#endif
