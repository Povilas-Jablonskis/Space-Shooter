#ifndef sharedContext_hpp
#define sharedContext_hpp

#include <memory>
#include <vector>
#include <irrKlang/irrKlang.h>

class InputManager;
class ObjectCollection;
class SpriteSheet;
class Renderer;
class Font;

struct SharedContext
{
    irrklang::ISoundEngine* m_soundEngine{};
    ObjectCollection* m_objects{};
    InputManager* m_inputManager{};
    SpriteSheet* m_spriteSheet{};
    Renderer* m_renderer{};
    Font* m_font{};
};

#endif