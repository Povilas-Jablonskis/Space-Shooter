#include "C_InstanceID.hpp"

unsigned int C_InstanceID::m_s_Count = 0;

C_InstanceID::C_InstanceID(Object* initialOwner) : Component(initialOwner), m_id(m_s_Count++)
{
}

unsigned int C_InstanceID::get() const
{
	return m_id;
}
