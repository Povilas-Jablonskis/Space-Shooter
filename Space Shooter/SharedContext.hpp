#ifndef sharedContext_hpp
#define sharedContext_hpp

#include <irrKlang/irrKlang.h>

class InputManager;
class ObjectCollection;
class SpriteSheet;
class Renderer;
class Font;

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
