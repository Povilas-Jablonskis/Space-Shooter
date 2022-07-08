#include "S_Drawable.hpp"
#include "Object.hpp"
#include "C_Sprite.hpp"

#include <algorithm>

void S_Drawable::add(std::vector<std::shared_ptr<Object>>& objects)
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
            auto& obj = *objIterator;

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
    auto& objectsDrawable = object.getSprite();

    if (objectsDrawable)
    {
        auto layer = objectsDrawable->getDrawLayer();

        auto itr = m_drawables.find(layer);

        if (itr != m_drawables.end())
        {
            m_drawables[layer].push_back(objectsDrawable);
        }
        else
        {
            std::vector<std::shared_ptr<C_Sprite>> objs;
            objs.push_back(objectsDrawable);

            m_drawables.insert(std::make_pair(layer, objs));
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
        if (!std::is_sorted(layer.second.begin(), layer.second.end(), layerSort))
        {
            std::sort(layer.second.begin(), layer.second.end(), layerSort);
        }
    }
}

void S_Drawable::draw(Renderer& renderer)
{
    for (auto& layer : m_drawables)
    {
        for (auto& drawable : layer.second)
        {
            drawable->draw(renderer);
        }
    }
}