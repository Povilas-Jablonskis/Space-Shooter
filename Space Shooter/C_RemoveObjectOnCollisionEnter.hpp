#ifndef C_RemoveObjectOnCollisionEnter_hpp
#define C_RemoveObjectOnCollisionEnter_hpp

#include "Component.hpp"
#include "C_Collidable.hpp"

class C_RemoveObjectOnCollisionEnter final : public Component, public C_Collidable
{
public:
	explicit C_RemoveObjectOnCollisionEnter(Object* initialOwner);

	void onCollisionEnter(C_BoxCollider& other) override;
};

#endif /* C_RemoveObjectOnCollisionEnter_hpp */
