#include "RenderObject.hpp"
#include "Animation.hpp"

namespace Engine
{
	RenderObject::RenderObject(float width, float height, const glm::vec2& position, const glm::vec4& color) : m_width(width), m_height(height), m_position(position), m_color(color)
	{
		
	}

	void RenderObject::updateAnimation(float dt)
	{
		if (getAnimation() == nullptr) return;

		auto sprites = getAnimation()->getAnimation();

		if (!sprites.empty())
		{
			setAnimTimer(getAnimTimer() + dt);

			if (getAnimTimer() > (1.0f / 60.f))
			{
				setAnimTimer(0);
				setCurrentFrame(getCurrentFrame() + 1);
				if (getCurrentFrame() >= sprites.size())
				{
					setCurrentFrame(sprites.size() - 1);
					setAnimationStatus(true);
				}
			}

			setOriginalWidth(getAnimation()->getAnimation().at(getCurrentFrame()).z);
			setOriginalHeight(getAnimation()->getAnimation().at(getCurrentFrame()).w);

			setWidth(getAnimation()->getAnimation().at(getCurrentFrame()).z);
			setHeight(getAnimation()->getAnimation().at(getCurrentFrame()).w);
		}
	}

	void RenderObject::applyAnimation(const std::shared_ptr<Animation>& t_animation)
	{
		if (t_animation == nullptr || t_animation == getAnimation()) return;

		setAnimation(t_animation);
		setAnimTimer(0);
		setAnimationStatus(false);
		setCurrentFrame(0);

		setOriginalWidth(t_animation->getAnimation().at(getCurrentFrame()).z);
		setOriginalHeight(t_animation->getAnimation().at(getCurrentFrame()).w);

		setWidth(t_animation->getAnimation().at(getCurrentFrame()).z);
		setHeight(t_animation->getAnimation().at(getCurrentFrame()).w);
	}
}