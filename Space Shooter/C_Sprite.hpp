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
	explicit C_Sprite(Object* initialOwner);

	void update(float dt) override;
	void draw(const Renderer& renderer) const;
	[[nodiscard]] bool continueToDraw() const;

	void setSortOrder(const int order);
	[[nodiscard]] int getSortOrder() const;

	void setDrawLayer(const DrawLayer drawLayer);
	[[nodiscard]] DrawLayer getDrawLayer() const;

	Sprite& getSprite();

private:
	int m_sortOrder{};
	DrawLayer m_layer{DrawLayer::DEFAULT};

	Sprite m_sprite;
};

#endif
