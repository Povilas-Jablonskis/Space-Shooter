#include "Explosion.h"

namespace Engine
{
	Explosion::Explosion(float _width, float _height, glm::vec2 _position)
		: RenderObject(_width, _height, _position, glm::vec4(255.0f, 255.0f, 255.0f, 1.0f))
	{
		
	}

	bool Explosion::update(float dt)
	{
		updateTexture(dt);

		return (texture != nullptr && !texture->getLoopStatus() && currentFrame >= texture->getEndFrame() && animComplete);
	}

	void Explosion::applyTexture(std::shared_ptr<Texture> _texture)
	{
		RenderObject::applyTexture(_texture);

		if (texture != nullptr)
			texture->setDelay(0.1f);
	}
}