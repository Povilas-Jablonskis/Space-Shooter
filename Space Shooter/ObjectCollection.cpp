#include "ObjectCollection.hpp"
#include "C_Tag.hpp"

void ObjectCollection::add(const std::shared_ptr<Object>& object)
{
	m_newObjects.push_back(object);
}

void ObjectCollection::add(std::vector<std::shared_ptr<Object>>& otherObjects)
{
	m_newObjects.insert(m_newObjects.end(), otherObjects.begin(), otherObjects.end());
}

void ObjectCollection::update(const float deltaTime)
{
	for (const auto& o : m_objects)
	{
		o->update(deltaTime);
	}

	m_collidables.resolve();
}

void ObjectCollection::draw(const Renderer& renderer) const
{
	m_drawables.draw(renderer);
}

void ObjectCollection::removeObjectsWithTag(const Tag tag)
{
	sFor_each_if(m_objects.begin(), m_objects.end(),
	             [=](const std::shared_ptr<Object>& object) { return object->tag->compare(tag); },
	             [](const std::shared_ptr<Object>& object) { object->queueForRemoval(); });
}

void ObjectCollection::processNewObjects()
{
	if (!m_newObjects.empty())
	{
		for (const auto& o : m_newObjects)
		{
			o->awake();
		}

		for (const auto& o : m_newObjects)
		{
			o->start();
		}

		m_objects.insert(m_objects.end(), m_newObjects.begin(), m_newObjects.end());

		m_drawables.add(m_newObjects);
		m_collidables.add(m_newObjects);

		m_newObjects.clear();
	}
}

void ObjectCollection::processRemovals()
{
	bool isRemoved = false;
	auto objIterator = m_objects.begin();
	while (objIterator != m_objects.end())
	{
		if ((*objIterator)->isQueuedForRemoval())
		{
			objIterator = m_objects.erase(objIterator);
			isRemoved = true;
		}
		else
		{
			++objIterator;
		}
	}

	if (isRemoved)
	{
		m_drawables.processRemovals();
		m_collidables.processRemovals();
	}
}

void ObjectCollection::processCollidingObjects()
{
	m_collidables.processCollidingObjects();
}
