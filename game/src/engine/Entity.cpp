#include "Entity.h"
#include "Components.h"
#include <vector>

namespace Engine {
	Entity::Entity(entt::entity id, entt::registry* reg) : m_EnttID(id), m_Scene(reg) {}

	Engine::Entity Entity::getParent() {
		return getComponent<Engine::Components::InfoComponent>().m_Parent;
	}

	bool Entity::disabled() {
		return getComponent<Engine::Components::InfoComponent>().disabled;
	}

	void Entity::setParent(Engine::Entity p) {
		auto& info = getComponent<Engine::Components::InfoComponent>();
		if (info.m_Parent) {
			//remove from old parent
			auto& children = info.m_Parent.getComponent<Engine::Components::InfoComponent>().m_Children;
			int index = findIndex(children, *this);

			children.erase(children.begin() + index);
		}
		//set new parent
		info.m_Parent = p;
		info.m_Parent.getComponent<Engine::Components::InfoComponent>().m_Children.push_back(*this);
	}

	void Entity::disable() {
		getComponent<Engine::Components::InfoComponent>().disabled = true;
	}

	void Entity::enable() {
		getComponent<Engine::Components::InfoComponent>().disabled = false;
	}

	std::vector<Engine::Entity> Entity::getChildren() {
		return getComponent<Engine::Components::InfoComponent>().m_Children;
	}
}