#include "RenderObject.h"


namespace Engine
{
	RenderObject::RenderObject(float _width, float _height, glm::vec2 _position, glm::vec4 _color)
		: width(_width), height(_height), position(_position), color(_color), theAnimation(nullptr), animComplete(false), animTimer(0.0f), currentFrame(0), rotationAngle(0.0f), rotationAxis(glm::vec3(0.0, 0.0, 1.0))
	{
		
	}

	void RenderObject::updateAnimation(float dt)
	{
		if (theAnimation == nullptr) return;

		auto sprites = theAnimation->getAnimation();

		if (sprites->size() > 1)
		{
			animTimer += dt;
			if (animTimer > theAnimation->getDelay())
			{
				animTimer = 0;
				currentFrame++;
				if (currentFrame >= sprites->size())
				{
					if (theAnimation->getLoopStatus())
						currentFrame = 0;
					else
					{
						currentFrame = (sprites->size() - 1);
						animComplete = true;
					}
				}
			}
		}
	}

	void RenderObject::applyAnimation(std::shared_ptr<Animation> _animation)
	{
		if (_animation == nullptr || _animation == theAnimation) return;

		theAnimation = _animation;
		animTimer = 0.0;
		animComplete = false;
		currentFrame = 0;
	}		

	float RenderObject::getSize(int index) const
	{
		switch (index)
		{
			case 0:
				return width;
			case 1:
				return height;
			default:
				return NULL;
		}
	}

	void RenderObject::draw(std::shared_ptr<Renderer> renderer)
	{
		auto program = renderer->getShaderProgram("shader");
		float windowWidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowHeigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

		int offsetLocation = glGetUniformLocation(program, "color");
		int offsetLocation2 = glGetUniformLocation(program, "renderMode");
		int offsetLocation3 = glGetUniformLocation(program, "spriteCoordinates");
		int offsetLocation6 = glGetUniformLocation(program, "projection");
		int offsetLocation7 = glGetUniformLocation(program, "model");
		glm::mat4 projection = glm::ortho(0.0f, windowWidth, 0.0f, windowHeigth, 0.0f, 1.0f);
		glBindVertexArray(renderer->getVAO());
		glUseProgram(program);
		if (getColor(3) == 0.0f) return;

		auto animation = getAnimation();
		auto sprites = animation->getAnimation();

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(getPosition(), 0.0f));

		model = glm::translate(model, glm::vec3(0.5f * getSize(0), 0.5f * getSize(1), 0.0f));
		model = glm::rotate(model, getRotationAngle(), getRotationAxis());
		model = glm::translate(model, glm::vec3(-0.5f * getSize(0), -0.5f * getSize(1), 0.0f));

		model = glm::scale(model, glm::vec3(getSize(0), getSize(1), 1.0f));

		glUniform4f(offsetLocation, getColor(0) / 255.0f, getColor(1) / 255.0f, getColor(2) / 255.0f, getColor(3));

		if (animation != nullptr)
		{
			auto currentSprite = sprites->at(getCurrentFrame());
			glBindTexture(GL_TEXTURE_2D, animation->getSpriteSheetTexture());

			glUniform1f(offsetLocation2, 1.0f);
			auto spriteSheetSize = glm::vec2(animation->getSpriteSheetSize(0), animation->getSpriteSheetSize(1));
			glUniform4f(offsetLocation3, currentSprite.x / spriteSheetSize.x, currentSprite.y / spriteSheetSize.y, currentSprite.z / spriteSheetSize.x, currentSprite.w / spriteSheetSize.y);
		}
		else
			glUniform1f(offsetLocation2, 0.0f);

		glUniformMatrix4fv(offsetLocation6, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(offsetLocation7, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glUseProgram(0);
		glBindVertexArray(0);
	}
}