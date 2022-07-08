#ifndef C_RemoveObjectOnCollisionEnter_hpp
#define C_RemoveObjectOnCollisionEnter_hpp

#include "Component.hpp"
#include "C_Collidable.hpp"

class C_RemoveObjectOnCollisionEnter : public Component, public C_Collidable
{
public:
    C_RemoveObjectOnCollisionEnter(Object*);
    
    void onCollisionEnter(C_BoxCollider&) override;
};

#endif /* C_RemoveObjectOnCollisionEnter_hpp */
