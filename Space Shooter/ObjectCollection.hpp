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
    ObjectCollection(S_Drawable&, S_Collidable&);
    void add(std::shared_ptr<Object>);
    void add(std::vector<std::shared_ptr<Object>>&);

    void update(float);
    void draw(const std::shared_ptr<Renderer>&);

    void processNewObjects();
    void processRemovals();

private:
    std::vector<std::shared_ptr<Object>> m_objects;
    std::vector<std::shared_ptr<Object>> m_newObjects;

    S_Drawable& m_drawables;
    S_Collidable& m_collidables;
};

#endif