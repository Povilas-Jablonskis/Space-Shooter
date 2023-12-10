#ifndef S_Drawable_hpp
#define S_Drawable_hpp

#include <map>
#include <vector>
#include <memory>

#include "Object.hpp"

class S_Drawable
{
public:
	void add(const std::vector<std::shared_ptr<Object>>& objects);

	void processRemovals();

	void draw(const Renderer& renderer) const;

private:
	static bool sLayerSort(const std::shared_ptr<C_Sprite>& firstComponent, const std::shared_ptr<C_Sprite>& secondComponent);
	void add(Object& object);
	void sort();

	std::map<DrawLayer, std::vector<std::shared_ptr<C_Sprite>>> m_drawables;
};

#endif
