#include "C_RemoveObjectOnCollisionEnter.hpp"
#include "Object.hpp"

C_RemoveObjectOnCollisionEnter::C_RemoveObjectOnCollisionEnter(Object* owner) : Component(owner) {}

void C_RemoveObjectOnCollisionEnter::onCollisionEnter(C_BoxCollider& other)
{
    m_owner->queueForRemoval();
}
