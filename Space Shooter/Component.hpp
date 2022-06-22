#ifndef Component_hpp
#define Component_hpp

class Object;

class Component
{
public:
    Component(Object* owner) : m_owner(owner) {}

    virtual void awake() {};
    virtual void start() {};

    virtual void update(float) {};

    Object* m_owner{};
};

#endif