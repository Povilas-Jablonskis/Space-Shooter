#ifndef C_Collidable_hpp
#define C_Collidable_hpp

#include "C_BoxCollider.hpp"

class C_Collidable
{
public:
	C_Collidable() = default;
	C_Collidable(const C_Collidable&) = default;
	C_Collidable(C_Collidable&&) = default;
	C_Collidable& operator=(const C_Collidable&) = default;
	C_Collidable& operator=(C_Collidable&&) = default;
	virtual ~C_Collidable() = default;

	virtual void onCollisionEnter(C_BoxCollider& other)
	{
	}

	virtual void onCollisionStay(C_BoxCollider& other)
	{
	}

	virtual void onCollisionExit(C_BoxCollider& other)
	{
	}
};
#endif
