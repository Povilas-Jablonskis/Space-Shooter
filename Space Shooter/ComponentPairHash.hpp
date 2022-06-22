#ifndef ComponentPairHash_hpp
#define ComponentPairHash_hpp

#include <cstddef>

struct ComponentPairHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        std::size_t x = t.first->m_owner->m_instanceID->get();
        std::size_t y = t.second->m_owner->m_instanceID->get();

        return (x >= y) ? (x * x + x + y) : (y * y + y + x);
    }
};

#endif