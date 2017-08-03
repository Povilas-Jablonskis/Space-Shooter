#include "Explosion.h"

namespace Engine
{
	Explosion::Explosion(int _width, int _height, glm::vec2 _position)
		: RenderObject(_width, _height, _position, glm::vec4(255.0f, 255.0f, 255.0f, 1.0f))
	{
		
	}

	Explosion::~Explosion()
	{

	}

	bool Explosion::update(float dt)
	{
		updateTexture(dt);

		return (texture != nullptr && !texture->getLoopStatus() && texture->getCurrentFrame() >= texture->getEndFrame() && texture->getAnimationStatus());
	}

	void Explosion::applyTexture(std::shared_ptr<Texture> _texture)
	{
		RenderObject::applyTexture(_texture);

		if (texture != nullptr)
			texture->setDelay(0.1f);
	}
}