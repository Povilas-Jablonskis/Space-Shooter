#include "Explosion.h"

namespace Engine
{
	Explosion::Explosion(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color, std::shared_ptr<Application> _application)
		: BaseGameObject(_width, _height, _position, _velocity, _color, _application)
	{

	}

	Explosion::~Explosion()
	{

	}

	void Explosion::updateTexture(float dt)
	{
		auto tempTexture = application->getTexture(texture);
		if (tempTexture == nullptr) return;

		BaseGameObject::updateTexture(dt);

		if (currentFrame == tempTexture->getEndFrame() && animComplete)
		{

		}
	}
}