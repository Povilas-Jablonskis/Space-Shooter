#ifndef C_Collidable_hpp
#define C_Collidable_hpp

#include "C_BoxCollider.hpp"

#include <memory>

class C_Collidable
{
public:
    virtual void onCollisionEnter(C_BoxCollider&) {};
    virtual void onCollisionStay(C_BoxCollider&) {};
    virtual void onCollisionExit(C_BoxCollider&) {};
};

#endif