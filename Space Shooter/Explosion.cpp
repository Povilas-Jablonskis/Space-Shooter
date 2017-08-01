#include "Explosion.h"

namespace Engine
{
	Explosion::Explosion(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: BaseGameObject(_width, _height, _position, _velocity, _color)
	{
		
	}

	Explosion::~Explosion()
	{

	}

	bool Explosion::update(float dt)
	{
		BaseGameObject::update(dt);

		if (texture != nullptr && texture->getCurrentFrame() >= texture->getEndFrame() && texture->getAnimationStatus())
			return true;
		return false;
	}

	void Explosion::applyTexture(std::shared_ptr<Texture> _texture)
	{
		BaseGameObject::applyTexture(_texture);

		if (texture != nullptr)
			texture->setDelay(0.1f);
	}
}