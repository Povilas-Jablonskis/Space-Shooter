#ifndef C_Collidable_hpp
#define C_Collidable_hpp

#include <memory>

#include "C_BoxCollider.hpp"

class C_Collidable
{
public:
    virtual void onCollisionEnter(std::shared_ptr<C_BoxCollider>) {};
    virtual void onCollisionStay(std::shared_ptr<C_BoxCollider>) {};
    virtual void onCollisionExit(std::shared_ptr<C_BoxCollider>) {};
};

#endif