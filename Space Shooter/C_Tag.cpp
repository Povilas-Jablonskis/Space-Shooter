#include "C_Tag.hpp"

C_Tag::C_Tag(Object* initialOwner) : Component(initialOwner), m_tag(Tag::DEFAULT)
{
}

Tag C_Tag::get() const
{
	return m_tag;
}

void C_Tag::set(const Tag tag)
{
	m_tag = tag;
}

bool C_Tag::compare(const C_Tag& tag) const
{
	return m_tag == tag.m_tag;
}

bool C_Tag::compare(const Tag tag) const
{
	return m_tag == tag;
}
