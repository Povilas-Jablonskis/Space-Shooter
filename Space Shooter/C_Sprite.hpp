#ifndef C_Sprite_hpp
#define C_Sprite_hpp

#include <math.h>
#include <memory>
#include <string>
#include <glm/vec4.hpp>

#include "Component.hpp"
#include "Sprite.hpp"

class Renderer;

enum class DrawLayer
{
    Default,
    Background,
    Entities,
    Foreground,
    UI
};

class C_Sprite : public Component
{
public:
    C_Sprite(Object*);

    void load(int);
    void load(const std::string&);

    void update(float) override;
    void draw(const std::shared_ptr<Renderer>&);
    bool continueToDraw() const;

    void setSortOrder(int);
    int getSortOrder() const;

    void setDrawLayer(DrawLayer);
    DrawLayer getDrawLayer() const;

    void setTextureRect(int, int, int, int);
    void setTextureRect(const glm::ivec4&);
    const glm::ivec4& getTextureRect() const { return m_sprite.getTextureRect(); };

    void setScale(float x, float y) { m_sprite.setScale(x, y); }
    const glm::vec2& getScale() const { return m_sprite.getScale(); }
private:
    int m_sortOrder{};
    DrawLayer m_layer{};
    Sprite m_sprite{};
    int currentTextureID{ -1 };
};

#endif