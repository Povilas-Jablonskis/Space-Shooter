#include "C_RemoveObjectOnCollisionEnter.hpp"
#include "Object.hpp"

C_RemoveObjectOnCollisionEnter::C_RemoveObjectOnCollisionEnter(Object* initialOwner) : Component(initialOwner)
{
}

void C_RemoveObjectOnCollisionEnter::onCollisionEnter(C_BoxCollider&)
{
	owner->queueForRemoval();
}
