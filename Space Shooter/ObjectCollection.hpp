#ifndef ObjectCollection_hpp
#define ObjectCollection_hpp

#include <memory>
#include <vector>

#include "Object.hpp"
#include "S_Drawable.hpp"
#include "S_Collidable.hpp"

class ObjectCollection
{
public:
	void add(const std::shared_ptr<Object>& object);
	void add(std::vector<std::shared_ptr<Object>>& otherObjects);

	void update(const float deltaTime);
	void draw(const Renderer& renderer) const;

	void removeObjectsWithTag(const Tag tag);

	void processNewObjects();
	void processRemovals();
	void processCollidingObjects();

private:
	template <typename Iterator, typename Predicate, typename Op>
	void for_each_if(Iterator first, Iterator last, Predicate p, Op op)
	{
		while (first < last)
		{
			if (p(*first)) op(*first);
			++first;
		}
	}

	std::vector<std::shared_ptr<Object>> m_objects;
	std::vector<std::shared_ptr<Object>> m_newObjects;

	S_Drawable m_drawables;
	S_Collidable m_collidables;
};

#endif
