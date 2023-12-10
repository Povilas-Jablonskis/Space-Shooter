#ifndef objectH
#define objectH

#include <vector>
#include <memory>

#include "C_Collidable.hpp"
#include "C_InstanceID.hpp"
#include "C_Sprite.hpp"
#include "C_Tag.hpp"
#include "C_Transform.hpp"
#include "Renderer.hpp"
#include "SharedContext.hpp"

class Object
{
public:
	explicit Object(SharedContext*);

	void awake() const; // Called when object created. Use to ensure required components are present.
	void start() const; // Called after Awake method. Use to initialise variables.

	void update(float dt) const;
	void draw(const Renderer& renderer) const;

	void onCollisionEnter(C_BoxCollider& other) const;
	void onCollisionStay(C_BoxCollider& other) const;
	void onCollisionExit(C_BoxCollider& other) const;

	template <typename T>
	std::shared_ptr<T> addComponent()
	{
		static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

		//TODO: allow us to add more than one component, implement getcomponents
		// Check that we don't already have a component of this type.
		for (auto& existingComponent : m_components)
		{
			if (dynamic_pointer_cast<T>(existingComponent))
			{
				return dynamic_pointer_cast<T>(existingComponent);
			}
		}

		std::shared_ptr<T> newComponent = std::make_shared<T>(this);

		m_components.push_back(newComponent);

		if (dynamic_pointer_cast<C_Sprite>(newComponent))
		{
			m_sprite = dynamic_pointer_cast<C_Sprite>(newComponent);
		}

		if (dynamic_pointer_cast<C_Collidable>(newComponent))
		{
			m_collidables.push_back(dynamic_pointer_cast<C_Collidable>(newComponent));
		}

		return newComponent;
	}

	template <typename T>
	std::shared_ptr<T> getComponent()
	{
		for (auto& existingComponent : m_components)
		{
			if (dynamic_pointer_cast<T>(existingComponent))
			{
				return dynamic_pointer_cast<T>(existingComponent);
			}
		}

		return nullptr;
	}

	template <typename T>
	std::vector<std::shared_ptr<T>> getComponents()
	{
		std::vector<std::shared_ptr<T>> matchingComponents;
		for (auto& existingComponent : m_components)
		{
			if (dynamic_pointer_cast<T>(existingComponent))
			{
				matchingComponents.emplace_back(dynamic_pointer_cast<T>(existingComponent));
			}
		}

		return matchingComponents;
	}

	const std::shared_ptr<C_Sprite>& getSprite();

	[[nodiscard]] bool isQueuedForRemoval() const;
	void queueForRemoval();

	std::shared_ptr<C_Transform> transform;
	std::shared_ptr<C_InstanceID> instanceID;
	std::shared_ptr<C_Tag> tag;

	SharedContext* context;

private:
	std::vector<std::shared_ptr<Component>> m_components;
	std::shared_ptr<C_Sprite> m_sprite{};
	std::vector<std::shared_ptr<C_Collidable>> m_collidables;
	bool m_queuedForRemoval{};
};
#endif
