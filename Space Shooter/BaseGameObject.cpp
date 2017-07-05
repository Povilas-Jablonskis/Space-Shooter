#include "BaseGameObject.h"

namespace Engine
{
	BaseGameObject::BaseGameObject()
	{

	}

	BaseGameObject::BaseGameObject(int _width, int _height, glm::vec2 _position, glm::vec2 _velocity, glm::vec3 _color)
		: width(_width), height(_height), position(_position), velocity(_velocity), color(_color) {}

	BaseGameObject::~BaseGameObject()
	{

	}

	void BaseGameObject::Draw()
	{
		auto program = Application::GetShaderProgram("shader");
		glBindVertexArray(Application::GetVAO());
			glUseProgram(program);
				float _width = (float)(glutGet(GLUT_WINDOW_WIDTH));
				float _height = (float)(glutGet(GLUT_WINDOW_HEIGHT));

				int offsetLocation = glGetUniformLocation(program, "givenposition");
				int offsetLocation2 = glGetUniformLocation(program, "size");
				int offsetLocation3 = glGetUniformLocation(program, "color");

				glUniform2f(offsetLocation, position[0] / _width, position[1] / _height);
				glUniform2f(offsetLocation2, width / _width, height / _height);
				glUniform4f(offsetLocation3, color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, 1.0f);
				glDrawElements(GL_TRIANGLES, (sizeof(Application::indices) / sizeof(*Application::indices)), GL_UNSIGNED_INT, 0);
			glUseProgram(0);
		glBindVertexArray(0);

		DrawBulletList();
	}

	GLboolean BaseGameObject::CheckCollision(std::shared_ptr<BaseGameObject> _objecttocheck) // AABB - AABB collision
	{
		auto oneposition = _objecttocheck->GetPosition();
		auto onewidth = _objecttocheck->GetSize(0);
		auto oneheight = _objecttocheck->GetSize(1);

		// Collision x-axis?
		bool collisionX = oneposition.x + onewidth >= position.x &&
			position.x + width >= oneposition.x;
		// Collision y-axis?
		bool collisionY = oneposition.y + oneheight >= position.y &&
			position.y + height >= oneposition.y;
		// Collision only if on both axes
		return collisionX && collisionY;
	}

	GLboolean BaseGameObject::CheckCollision(std::vector<std::shared_ptr<BaseGameObject>>* _objectstocheck) // AABB - AABB collision
	{
		for (auto object : *_objectstocheck)
		{
			if (CheckCollision(object))
				return true;
		}
		return false;
	}

	bool BaseGameObject::Update(float _dt)
	{
		position[0] += velocity[0] * _dt;
		position[1] += velocity[1] * _dt;
		return true;
	}

	bool BaseGameObject::Update(float _dt, float _t)
	{
		position[0] += velocity[0] * _dt;
		position[1] += velocity[1] * _dt;
		return true;
	}

	void BaseGameObject::DrawBulletList()
	{
		for (auto bullet : bullets)
		{
			bullet->Draw();
		}
	}

	void BaseGameObject::UpdateBulletList(float _dt)
	{
		auto bulletslist = &bullets;
		for (std::vector<std::shared_ptr<BaseGameObject>> ::iterator it = bulletslist->begin(); it != bulletslist->end();)
		{
			if (it->get()->Update(_dt) == false)
				it = bulletslist->erase(it);
			else
				++it;
		}
	}

	float BaseGameObject::GetPosition(int index) const
	{
		return position[index];
	}

	glm::vec2 BaseGameObject::GetPosition() const
	{
		return position;
	}

	int BaseGameObject::GetSize(int index) const
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

	float BaseGameObject::GetColor(int index) const
	{
		return color[index];
	}

	void BaseGameObject::OnCollision(std::shared_ptr<BaseGameObject> collider)
	{
		std::cout << "some gameobject hit" << std::endl;
	}
}