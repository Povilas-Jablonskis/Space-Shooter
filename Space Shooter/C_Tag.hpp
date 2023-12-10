#ifndef C_Tag_hpp
#define C_Tag_hpp

#include "Component.hpp"

enum class Tag
{
	DEFAULT,
	PLAYER,
	NPC,
	PLAYER_USER_INTERFACE
};

class C_Tag final : public Component
{
public:
	explicit C_Tag(Object* initialOwner);

	[[nodiscard]] Tag get() const;
	void set(Tag);

	[[nodiscard]] bool compare(const C_Tag& tag) const;
	[[nodiscard]] bool compare(Tag tag) const;

private:
	Tag m_tag;
};


#endif
