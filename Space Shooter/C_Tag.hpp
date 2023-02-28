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
	explicit C_Tag(Object*);

	[[nodiscard]] Tag get() const;
	void set(Tag);

	[[nodiscard]] bool compare(const C_Tag&) const;
	[[nodiscard]] bool compare(Tag) const;

private:
	Tag m_tag;
};


#endif
