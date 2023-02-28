#ifndef C_Sprite_hpp
#define C_Sprite_hpp

#include "Component.hpp"
#include "Renderer.hpp"
#include "Sprite.hpp"

enum class DrawLayer
{
	DEFAULT,
	BACKGROUND,
	ENTITIES,
	UI
};

class C_Sprite final : public Component
{
public:
	explicit C_Sprite(Object*);

	void update(float) override;
	void draw(const Renderer&) const;
	[[nodiscard]] bool continueToDraw() const;

	void setSortOrder(int);
	[[nodiscard]] int getSortOrder() const;

	void setDrawLayer(DrawLayer);
	[[nodiscard]] DrawLayer getDrawLayer() const;

	Sprite& getSprite();

private:
	int m_sortOrder{};
	DrawLayer m_layer{DrawLayer::DEFAULT};

	Sprite m_sprite;
};

#endif
