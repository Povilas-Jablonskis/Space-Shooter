#include "Object.hpp"
#include "Renderer.hpp"
#include "C_BoxCollider.hpp"
#include "C_InstanceID.hpp"
#include "C_Tag.hpp"

Object::Object(SharedContext* context) : m_context(context)
{
    m_transform = addComponent<C_Transform>();
    m_instanceID = addComponent<C_InstanceID>();
    m_tag = addComponent<C_Tag>();
}

void Object::awake()
{
    for (const auto& component : m_components)
    {
        component->awake();
    }
}

void Object::start()
{
    for (const auto& component : m_components)
    {
        component->start();
    }
}

void Object::update(float timeDelta)
{
    for (const auto& component : m_components)
    {
        component->update(timeDelta);
    }
}

void Object::draw(const Renderer& renderer)
{
    m_sprite->draw(renderer);
}

void Object::onCollisionEnter(C_BoxCollider& other)
{
    for (const auto& component : m_collidables)
    {
        component->onCollisionEnter(other);
    }
}

void Object::onCollisionStay(C_BoxCollider& other)
{
    for (const auto& component : m_collidables)
    {
        component->onCollisionStay(other);
    }
}

void Object::onCollisionExit(C_BoxCollider& other)
{
    for (const auto& component : m_collidables)
    {
        component->onCollisionExit(other);
    }
}

void Object::queueForRemoval()
{
    m_queuedForRemoval = true;
}

const std::shared_ptr<C_Sprite>& Object::getSprite()
{
    return m_sprite;
}

bool Object::isQueuedForRemoval()
{
    return m_queuedForRemoval;
}