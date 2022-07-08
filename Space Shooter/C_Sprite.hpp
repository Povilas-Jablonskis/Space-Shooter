#ifndef C_Sprite_hpp
#define C_Sprite_hpp

#include "Component.hpp"
#include "Renderer.hpp"
#include "Sprite.hpp"

enum class DrawLayer
{
    Default,
    Background,
    Entities,
    UI
};

class C_Sprite : public Component
{
public:
    C_Sprite(Object*);

    void update(float) override;
    void draw(const Renderer&);
    bool continueToDraw() const;

    void setSortOrder(int);
    int getSortOrder() const;

    void setDrawLayer(DrawLayer);
    DrawLayer getDrawLayer() const;

    Sprite& getSprite();
private:
    int m_sortOrder{};
    DrawLayer m_layer{ DrawLayer::Default };

    Sprite m_sprite;
};

#endif