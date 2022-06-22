#ifndef S_Drawable_hpp
#define S_Drawable_hpp

#include <map>
#include <vector>
#include <memory>

class Renderer;
class Object;
class C_Sprite;
enum class DrawLayer;

class S_Drawable
{
public:
    void add(std::vector<std::shared_ptr<Object>>&);

    void processRemovals();

    void draw(const std::shared_ptr<Renderer>&);

private:
    static bool layerSort(std::shared_ptr<C_Sprite>, std::shared_ptr<C_Sprite>);
    void add(std::shared_ptr<Object>);
    void sort();

    std::map<DrawLayer, std::vector<std::shared_ptr<C_Sprite>>> m_drawables;
};

#endif