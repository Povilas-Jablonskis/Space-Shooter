#ifndef C_Tag_hpp
#define C_Tag_hpp

#include "Component.hpp"

#include <memory>

enum class Tag
{
    Default,
    Player,
    NPC,
    PlayerUserInterface
};

class C_Tag : public Component
{
public:
    C_Tag(Object*);

    Tag get() const;
    void set(Tag);

    bool compare(C_Tag&) const;
    bool compare(Tag) const;

private:
    Tag m_tag;
};


#endif