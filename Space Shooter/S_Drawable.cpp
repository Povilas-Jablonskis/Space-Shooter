#include "S_Drawable.hpp"
#include "Object.hpp"
#include "C_Sprite.hpp"

#include <algorithm>

void S_Drawable::add(const std::vector<std::shared_ptr<Object>>& objects)
{
	for (auto& o : objects)
	{
		add(*o);
	}

	sort();
}

void S_Drawable::processRemovals()
{
	for (auto& layer : m_drawables)
	{
		auto objIterator = layer.second.begin();
		while (objIterator != layer.second.end())
		{
			const auto& obj = *objIterator;

			if (!obj->continueToDraw())
			{
				objIterator = layer.second.erase(objIterator);
			}
			else
			{
				++objIterator;
			}
		}
	}
}

void S_Drawable::add(Object& object)
{
	if (auto& objectsDrawable = object.getSprite())
	{
		auto layer = objectsDrawable->getDrawLayer();

		const auto itr = m_drawables.find(layer);

		if (itr != m_drawables.end())
		{
			m_drawables[layer].push_back(objectsDrawable);
		}
		else
		{
			std::vector<std::shared_ptr<C_Sprite>> objects;
			objects.push_back(objectsDrawable);

			m_drawables.insert(std::make_pair(layer, objects));
		}
	}
}

bool S_Drawable::layerSort(const std::shared_ptr<C_Sprite>& a, const std::shared_ptr<C_Sprite>& b)
{
	return a->getSortOrder() < b->getSortOrder();
}

void S_Drawable::sort()
{
	for (auto& layer : m_drawables)
	{
		if (!std::ranges::is_sorted(layer.second, layerSort))
		{
			std::ranges::sort(layer.second, layerSort);
		}
	}
}

void S_Drawable::draw(const Renderer& renderer) const
{
	for (auto& layer : m_drawables)
	{
		for (const auto& drawable : layer.second)
		{
			drawable->draw(renderer);
		}
	}
}
