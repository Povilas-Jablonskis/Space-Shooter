#include "BaseGameObject.h"
#include "Renderer.h"

namespace Engine
{
	BaseGameObject::BaseGameObject()
	{

	}

	BaseGameObject::BaseGameObject(int _width, int _height, float x, float y, float velx, float vely, float r, float g, float b)
	{
		width = _width;
		height = _height;
		position[0] = x;
		position[1] = y;
		velocity[0] = velx;
		velocity[1] = vely;
		color[0] = r;
		color[1] = g;
		color[2] = b;
	}

	BaseGameObject::~BaseGameObject()
	{

	}

	void BaseGameObject::Draw(GLuint program)
	{
		float _width = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float _height = (float)(glutGet(GLUT_WINDOW_HEIGHT));

		int offsetLocation = glGetUniformLocation(program, "givenposition");
		int offsetLocation2 = glGetUniformLocation(program, "size");
		int offsetLocation3 = glGetUniformLocation(program, "color");

		glUniform2f(offsetLocation, position[0] / _width, position[1] / _height);
		glUniform2f(offsetLocation2, width / _width, height / _height);
		glUniform4f(offsetLocation3, color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, 1.0f);
	}

	void BaseGameObject::DrawOtherObjects()
	{
		for (auto object : objects)
		{
			Renderer::Render(object.get());
		}
	}

	void BaseGameObject::Update()
	{
		for (auto object : objects)
		{
			object->Update();
		}
	}

	std::vector<std::shared_ptr<BaseGameObject>>::iterator BaseGameObject::Update(std::vector<std::shared_ptr<BaseGameObject>>& vector, std::vector<std::shared_ptr<BaseGameObject>>::iterator _it)
	{
		float windowwidth = (float)(glutGet(GLUT_WINDOW_WIDTH));
		float windowheigth = (float)(glutGet(GLUT_WINDOW_HEIGHT));

		if (position[1] + height >= windowheigth || position[1] <= 0.0f || position[0] + width >= windowwidth || position[0] <= 0.0f)
		{
			for (std::vector<std::shared_ptr<BaseGameObject>>::iterator it = vector.begin(); it != vector.end(); ++it)
			{
				if (&(*it->get()) == this)
					return vector.erase(it);
			}
		}
		return ++_it;
	}

	float BaseGameObject::GetPosition(int index)
	{
		return position[index];
	}

	int BaseGameObject::GetSize(int index)
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

	float BaseGameObject::GetColor(int index)
	{
		return color[index];
	}
}