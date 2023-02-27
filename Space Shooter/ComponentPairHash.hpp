#ifndef ComponentPairHash_hpp
#define ComponentPairHash_hpp

#include "Object.hpp"
#include "C_InstanceID.hpp"
#include "C_BoxCollider.hpp"

#include <cstddef>
#include <memory>

struct ComponentPairHash
{
	std::size_t operator()(const std::pair<std::shared_ptr<C_BoxCollider>, std::shared_ptr<C_BoxCollider>>& t) const
	{
		const std::size_t x = t.first->m_owner->m_instanceID->get();
		const std::size_t y = t.second->m_owner->m_instanceID->get();

		return (x >= y) ? (x * x + x + y) : (y * y + y + x);
	}
};

#endif
