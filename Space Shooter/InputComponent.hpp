#ifndef inputComponentH
#define inputComponentH

#include <memory>

class Entity;
class InputManager;

class InputComponent
{
public:
	void update(Entity*, const std::shared_ptr<InputManager>&, float) const;
};
#endif