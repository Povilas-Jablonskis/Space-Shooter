#include "S_Drawable.hpp"

#include <algorithm>
#include <ranges>

void S_Drawable::add(const std::vector<std::shared_ptr<Object>>& objects)
{
	for (auto& object : objects)
	{
		add(*object);
	}

	sort();
}

void S_Drawable::processRemovals()
{
	for (auto& layer : m_drawables | std::views::values)
	{
		auto layerIterator = layer.begin();
		while (layerIterator != layer.end())
		{
			if (!(*layerIterator)->continueToDraw())
			{
				layerIterator = layer.erase(layerIterator);
			}
			else
			{
				++layerIterator;
			}
		}
	}
}

void S_Drawable::add(Object& object)
{
	if (auto& objectsDrawable = object.getSprite())
	{
		auto layer = objectsDrawable->getDrawLayer();

		if (m_drawables.contains(layer))
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

bool S_Drawable::sLayerSort(const std::shared_ptr<C_Sprite>& firstComponent,
                            const std::shared_ptr<C_Sprite>& secondComponent)
{
	return firstComponent->getSortOrder() < secondComponent->getSortOrder();
}

void S_Drawable::sort()
{
	for (auto& layer : m_drawables | std::views::values)
	{
		if (!std::ranges::is_sorted(layer, sLayerSort))
		{
			std::ranges::sort(layer, sLayerSort);
		}
	}
}

void S_Drawable::draw(const Renderer& renderer) const
{
	for (const auto& layer : m_drawables | std::views::values)
	{
		for (const auto& drawable : layer)
		{
			drawable->draw(renderer);
		}
	}
}
