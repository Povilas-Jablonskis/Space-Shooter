#include "Explosion.h"

namespace Engine
{
	Explosion::Explosion(float _width, float _height, glm::vec2 _position)
		: RenderObject(_width, _height, _position, glm::vec4(255.0f, 255.0f, 255.0f, 1.0f))
	{
		
	}

	bool Explosion::update(float dt)
	{
		updateAnimation(dt);

		if (theAnimation == nullptr)
			return false;

		auto sprites = theAnimation->getAnimation();
		return (!theAnimation->getLoopStatus() && currentFrame >= (sprites->size() - 1) && animComplete);
	}

	void Explosion::applyAnimation(std::shared_ptr<Animation> _animation)
	{
		RenderObject::applyAnimation(_animation);

		if (theAnimation != nullptr)
			theAnimation->setDelay(0.1f);
	}
}