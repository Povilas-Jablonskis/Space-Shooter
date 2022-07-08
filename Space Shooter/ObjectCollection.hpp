#ifndef ObjectCollection_hpp
#define ObjectCollection_hpp

#include <memory>
#include <vector>

#include "Object.hpp"
#include "Renderer.hpp"
#include "S_Drawable.hpp"
#include "S_Collidable.hpp"

enum class Tag;

class ObjectCollection
{
public:
    void add(const std::shared_ptr<Object>&);
    void add(std::vector<std::shared_ptr<Object>>&);

    void update(float);
    void draw(Renderer&);

    void removeObjectsWithTag(Tag);

    void processNewObjects();
    void processRemovals();
    void processCollidingObjects();
private:
    template<typename Iter, typename Pred, typename Op>
    void for_each_if(Iter first, Iter last, Pred p, Op op) {
        while (first != last) {
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