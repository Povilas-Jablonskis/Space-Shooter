#ifndef sharedContext_hpp
#define sharedContext_hpp

#include <irrKlang/irrKlang.h>

#include "Font.hpp"
#include "Renderer.hpp"
#include "InputManager.hpp"

class ObjectCollection;

struct SharedContext
{
	irrklang::ISoundEngine* soundEngine{};
	ObjectCollection* objects{};
	InputManager* inputManager{};
	SpriteSheet* spriteSheet{};
	Renderer* renderer{};
	Font* font{};
};

#endif
