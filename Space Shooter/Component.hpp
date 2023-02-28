#ifndef Component_hpp
#define Component_hpp

class Object;

class Component
{
public:
	Component() = default;
	Component(const Component&) = default;
	Component(Component&&) = default;
	Component& operator=(const Component&) = default;
	Component& operator=(Component&&) = default;
	virtual ~Component() = default;

	explicit Component(Object* owner) : owner(owner)
	{
	}

	virtual void awake()
	{
	}

	virtual void start()
	{
	}

	virtual void update(float)
	{
	}

	Object* owner{};
};

#endif
