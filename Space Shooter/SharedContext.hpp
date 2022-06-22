#ifndef sharedContext_hpp
#define sharedContext_hpp

#include <memory>
#include <vector>

#include "ResourceAllocator.hpp"

class BaseGameObject;
class Entity;
class Player;
class UIElementBase;
class Renderer;
class InputManager;
class Texture;
class ObjectCollection;

struct SharedContext
{
    ObjectCollection* m_objects{};
    std::shared_ptr<InputManager> m_inputManager{};
    std::shared_ptr<Player> m_player{};
    std::vector<std::shared_ptr<BaseGameObject>>* m_meteors;
    std::vector<std::shared_ptr<BaseGameObject>>* m_pickups;
    std::vector<std::shared_ptr<Entity>>* m_enemies;
    std::vector<std::shared_ptr<BaseGameObject>>* m_explosions;
    std::shared_ptr<UIElementBase> m_background{};
    ResourceAllocator<Texture>* m_textureAllocator{};
};

#endif