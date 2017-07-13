#include "BaseGameObject.h"

namespace Engine
{
	BaseGameObject::BaseGameObject(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec4 _color)
		: width(_width), height(_height), position(_position), velocity(_velocity), color(_color), texture(nullptr)
	{

	}

	BaseGameObject::~BaseGameObject()
	{
		if (texture != nullptr)
		{
			auto _texture = texture->getTexture();
			glDeleteTextures(0, &_texture);
		}
	}

	void BaseGameObject::draw()
	{
		auto program = Application::getShaderProgram("shader");

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(Application::getVAO());
			glUseProgram(program);
				float windowwidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
				float windowheigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

				int offsetLocation = glGetUniformLocation(program, "givenposition");
				int offsetLocation2 = glGetUniformLocation(program, "size");
				int offsetLocation3 = glGetUniformLocation(program, "color");
				int offsetLocation4 = glGetUniformLocation(program, "renderMode");
				int offsetLocation5 = glGetUniformLocation(program, "animscX");
				int offsetLocation6 = glGetUniformLocation(program, "animscY");
				int offsetLocation7 = glGetUniformLocation(program, "curranim");

				if (texture != nullptr)
				{
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, texture->getTexture());

					glUniform1f(offsetLocation4, 1.0f);
					glUniform1f(offsetLocation5, texture->getCount().x);
					glUniform1f(offsetLocation6, texture->getCount().y);
					glUniform1f(offsetLocation7, (float)texture->getCurrentFrame());
				}
				else
					glUniform1f(offsetLocation4, 0.0f);

				glUniform2f(offsetLocation, position.x / windowwidth, position.y / windowheigth);
				glUniform2f(offsetLocation2, width / windowwidth, height / windowheigth);
				glUniform4f(offsetLocation3, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a);
				glDrawElements(GL_TRIANGLES, (sizeof(Application::indices) / sizeof(*Application::indices)), GL_UNSIGNED_INT, 0);
			glUseProgram(0);
		glBindVertexArray(0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}

	GLboolean BaseGameObject::checkCollision(std::shared_ptr<BaseGameObject> _objecttocheck) // AABB - AABB collision
	{
		auto onePosition = _objecttocheck->getPosition();
		auto oneWidth = _objecttocheck->getSize(0);
		auto oneHeight = _objecttocheck->getSize(1);

		// Collision x-axis?
		bool collisionX = onePosition.x + oneWidth >= position.x &&
			position.x + width >= onePosition.x;
		// Collision y-axis?
		bool collisionY = onePosition.y + oneHeight >= position.y &&
			position.y + height >= onePosition.y;
		// Collision only if on both axes
		if (collisionX && collisionY)
		{
			onCollision(_objecttocheck);
			return true;
		}
		return false;
	}

	GLboolean BaseGameObject::checkCollision(std::vector<std::shared_ptr<BaseGameObject>>* _objectstocheck) // AABB - AABB collision
	{
		for (auto object : *_objectstocheck)
		{
			if (checkCollision(object))
				return true;
		}
		return false;
	}

	bool BaseGameObject::update(float _dt)
	{
		position.x += velocity.x * _dt;
		position.y += velocity.y * _dt;

		if (texture != nullptr)
			texture->update(_dt);
		return true;
	}

	bool BaseGameObject::update(float _dt, float _t)
	{
		position.x += velocity.x * _dt;
		position.y += velocity.y * _dt;

		if (texture != nullptr)
			texture->update(_dt);
		return true;
	}

	float BaseGameObject::getPosition(int index) const
	{
		return position[index];
	}

	void BaseGameObject::setPosition(glm::vec2 _position)
	{
		position = _position;
	}

	glm::vec2 BaseGameObject::getPosition() const
	{
		return position;
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

	float BaseGameObject::getColor(int index) const
	{
		return color[index];
	}

	void BaseGameObject::onCollision(std::shared_ptr<BaseGameObject> collider)
	{
		std::cout << "some gameobject hit" << std::endl;
	}

	void BaseGameObject::applyTexture(Texture* _texture)
	{
		auto tempTexture = new Texture();
		*tempTexture = *_texture;
		texture = tempTexture;
	}

	void BaseGameObject::setCurrentTextureCurrentFrame(int frame)
	{
		texture->setCurrentFrame(frame);
	}
}