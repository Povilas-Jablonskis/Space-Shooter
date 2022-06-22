#ifndef objectH
#define objectH

#include <vector>
#include <memory>

#include "SharedContext.hpp"
#include "C_Sprite.hpp"
#include "C_Collidable.hpp"

class C_BoxCollider;
class C_Transform;
class Component;
class C_InstanceID;
class Renderer;

class Object
{
public:
    Object(SharedContext*);

    void awake(); // Called when object created. Use to ensure required components are present.
    void start(); // Called after Awake method. Use to initialise variables.

    void update(float);
    void draw(const std::shared_ptr<Renderer>&);

    void onCollisionEnter(std::shared_ptr<C_BoxCollider>);
    void onCollisionStay(std::shared_ptr<C_BoxCollider>);
    void onCollisionExit(std::shared_ptr<C_BoxCollider>);

    template <typename T> std::shared_ptr<T> addComponent()
    {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

        //TODO: allow us to add more than one component, implement getcomponents
        // Check that we don't already have a component of this type.
        for (auto& exisitingComponent : m_components)
        {
            if (std::dynamic_pointer_cast<T>(exisitingComponent))
            {
                return std::dynamic_pointer_cast<T>(exisitingComponent);
            }
        }

        std::shared_ptr<T> newComponent = std::make_shared<T>(this);

        m_components.push_back(newComponent);

        if (std::dynamic_pointer_cast<C_Sprite>(newComponent))
        {
            m_sprite = std::dynamic_pointer_cast<C_Sprite>(newComponent);
        }

        if (std::dynamic_pointer_cast<C_Collidable>(newComponent))
        {
            m_collidables.push_back(std::dynamic_pointer_cast<C_Collidable>(newComponent));
        }

        return newComponent;
    };

    template <typename T> std::shared_ptr<T> getComponent()
    {
        for (auto& exisitingComponent : m_components)
        {
            if (std::dynamic_pointer_cast<T>(exisitingComponent))
            {
                return std::dynamic_pointer_cast<T>(exisitingComponent);
            }
        }

        return nullptr;
    };

    template <typename T> std::vector<std::shared_ptr<T>> getComponents()
    {
        std::vector<std::shared_ptr<T>> matchingComponents;
        for (auto& exisitingComponent : m_components)
        {
            if (std::dynamic_pointer_cast<T>(exisitingComponent))
            {
                matchingComponents.emplace_back(std::dynamic_pointer_cast<T>(exisitingComponent));
            }
        }

        return matchingComponents;
    };

    std::shared_ptr<C_Sprite> getSprite() { return m_sprite; };

    bool isQueuedForRemoval();
    void queueForRemoval();

    std::shared_ptr<C_Transform> m_transform;
    std::shared_ptr<C_InstanceID> m_instanceID;

    SharedContext* m_context;
private:
    std::vector<std::shared_ptr<Component>> m_components;
    std::shared_ptr<C_Sprite> m_sprite{};
    std::vector<std::shared_ptr<C_Collidable>> m_collidables;
    bool m_queuedForRemoval{};
};
#endif