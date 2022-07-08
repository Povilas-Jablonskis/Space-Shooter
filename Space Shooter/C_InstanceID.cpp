#include "C_InstanceID.hpp"

unsigned int C_InstanceID::count = 0;

C_InstanceID::C_InstanceID(Object* owner) : Component(owner), id(count++) {}

unsigned int C_InstanceID::get() const
{
    return id;
}