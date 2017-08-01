#include "BaseGameObject.h"

namespace Engine
{
	BaseGameObject::BaseGameObject(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: width(_width), height(_height), position(_position), velocity(_velocity), color(_color), texture(nullptr)
	{

	}

	BaseGameObject::~BaseGameObject()
	{

	}

	void BaseGameObject::draw(GLuint program)
	{
		float windowwidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowheigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

		int offsetLocation = glGetUniformLocation(program, "color");
		int offsetLocation2 = glGetUniformLocation(program, "renderMode");
		int offsetLocation3 = glGetUniformLocation(program, "animscX");
		int offsetLocation4 = glGetUniformLocation(program, "animscY");
		int offsetLocation5 = glGetUniformLocation(program, "curranim");
		int offsetLocation6 = glGetUniformLocation(program, "projection");
		int offsetLocation7 = glGetUniformLocation(program, "model");

		glm::mat4 projection = glm::ortho(0.0f, (float)glutGet(GLUT_WINDOW_WIDTH), 0.0f, (float)glutGet(GLUT_WINDOW_HEIGHT), 0.0f, 1.0f);

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(position, 0.0f));

		/*model = glm::translate(model, glm::vec3(0.5f * width, 0.5f * height, 0.0f));
		model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * width, -0.5f * height, 0.0f));*/

		model = glm::scale(model, glm::vec3(width, height, 1.0f));

		glUniform4f(offsetLocation, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a);

		if (texture != nullptr)
		{
			glBindTexture(GL_TEXTURE_2D, texture->getTexture());

			glUniform1f(offsetLocation2, 1.0f);
			glUniform1f(offsetLocation3, texture->getCount().x);
			glUniform1f(offsetLocation4, texture->getCount().y);
			glUniform1f(offsetLocation5, (float)texture->getCurrentFrame());
		}
		else
			glUniform1f(offsetLocation2, 0.0f);

		glUniformMatrix4fv(offsetLocation6, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(offsetLocation7, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	bool BaseGameObject::update(float _dt)
	{
		position.x += velocity.x * _dt;
		position.y += velocity.y * _dt;
		updateTexture(_dt);
		return true;
	}

	int BaseGameObject::getSize(int index) const
	{
		switch (index)
		{
			case 0:
			{
				return width;
			}
			case 1:
			{
				return height;
			}
			default:
			{
				return NULL;
			}
		}
	}

	void BaseGameObject::onCollision(BaseGameObject* collider)
	{
		std::cout << "some gameobject hit" << std::endl;
	}

	void BaseGameObject::applyTexture(std::shared_ptr<Texture> _texture)
	{
		if (_texture == nullptr) return;
		_texture->setAnimTimer(0.0f);
		texture = _texture;
	}

	void BaseGameObject::updateTexture(float dt)
	{
		if (texture == nullptr) return;
		if (texture->getEndFrame() - texture->getStartFrame() > 0)
		{
			texture->setAnimTimer(texture->getAnimTimer() + dt);
			if (texture->getAnimTimer() > texture->getDelay())
			{
				texture->setAnimTimer(texture->getAnimTimer() - texture->getDelay());
				texture->setCurrentFrame(texture->getCurrentFrame() + 1);
				if (texture->getCurrentFrame() < texture->getStartFrame() || texture->getCurrentFrame() > texture->getEndFrame())
				{
					if (texture->getLoopStatus())
						texture->setCurrentFrame(texture->getStartFrame());
					else
					{
						texture->setCurrentFrame(texture->getEndFrame());
						texture->setAnimationStatus(true);
					}
				}
			}
		}
	}

	void BaseGameObject::addBullet(std::shared_ptr<BaseGameObject> bullet, std::shared_ptr<Texture> texture)
	{
		bullet->applyTexture(texture);
		bullets.push_back(bullet);
	}
}