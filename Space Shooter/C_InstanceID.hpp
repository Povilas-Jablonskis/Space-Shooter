#ifndef C_InstanceID_hpp
#define C_InstanceID_hpp

#include "Component.hpp"

class C_InstanceID final : public Component
{
public:
	explicit C_InstanceID(Object*);

	[[nodiscard]] unsigned int get() const;

private:
	static unsigned int count;
	unsigned int id;
};


#endif
