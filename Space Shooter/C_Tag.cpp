#include "C_Tag.hpp"

C_Tag::C_Tag(Object* owner) : Component(owner), m_tag(Tag::Default)
{
}

Tag C_Tag::get() const
{
    return m_tag;
}

void C_Tag::set(Tag tag)
{
    m_tag = tag;
}

bool C_Tag::compare(C_Tag& other) const
{
    return m_tag == other.m_tag;
}

bool C_Tag::compare(Tag other) const
{
    return m_tag == other;
}