#include "C_Sprite.hpp"
#include "Object.hpp"
#include "SharedContext.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"
#include "C_Transform.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

C_Sprite::C_Sprite(Object* owner) : Component(owner) {}

void C_Sprite::load(int id)
{
    if (id >= 0 && id != currentTextureID)
    {
        currentTextureID = id;
        std::shared_ptr<Texture> texture = m_owner->m_context->m_textureAllocator->get(id);
        m_sprite.setTexture(*texture);
    }
}

void C_Sprite::load(const std::string& filePath)
{
    int textureID = m_owner->m_context->m_textureAllocator->add(filePath);

    if (textureID >= 0 && textureID != currentTextureID)
    {
        currentTextureID = textureID;
        std::shared_ptr<Texture> texture = m_owner->m_context->m_textureAllocator->get(textureID);
        m_sprite.setTexture(*texture);
    }
}

void C_Sprite::setSortOrder(int order)
{
    m_sortOrder = order;
}

int C_Sprite::getSortOrder() const
{
    return m_sortOrder;
}

void C_Sprite::setDrawLayer(DrawLayer drawLayer)
{
    m_layer = drawLayer;
}

DrawLayer C_Sprite::getDrawLayer() const
{
    return m_layer;
}

//TODO: implement static sprites for level tiles etc.
void C_Sprite::update(float deltaTime)
{
    auto transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(m_owner->m_transform->getPosition(), 0.0f));

    auto width = getTextureRect().z * getScale().x;
    auto height = getTextureRect().w * getScale().y;

    transform = glm::translate(transform, glm::vec3(0.5f * width, 0.5f * height, 0.0f));
    transform = glm::rotate(transform, m_sprite.getRotationAngle(), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::translate(transform, glm::vec3(-0.5f * width, -0.5f * height, 0.0f));

    transform = glm::scale(transform, glm::vec3(width, height, 1.0f));
    m_sprite.setTransform(transform);
}

void C_Sprite::draw(const std::shared_ptr<Renderer>& renderer)
{
    renderer->draw(m_sprite);
}

bool C_Sprite::continueToDraw() const
{
    return !m_owner->isQueuedForRemoval();
}


void C_Sprite::setTextureRect(int x, int y, int width, int height)
{
    m_sprite.setTextureRect(glm::ivec4(x, y, width, height));
}

void C_Sprite::setTextureRect(const glm::ivec4& rect)
{
    m_sprite.setTextureRect(rect);
}