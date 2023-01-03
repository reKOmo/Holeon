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
		for (auto& ent : getChildren()) {
			ent.disable();
		}
	}

	void Entity::enable() {
		getComponent<Engine::Components::InfoComponent>().disabled = false;
		for (auto& ent : getChildren()) {
			ent.enable();
		}
	}

	std::vector<Engine::Entity> Entity::getChildren() {
		return getComponent<Engine::Components::InfoComponent>().m_Children;
	}

	void Entity::removeChild(Engine::Entity child) {
		auto& info = getComponent<Engine::Components::InfoComponent>();
		int index = Engine::findIndex(info.m_Children, child);
		if (index != -1) {
			info.m_Children.erase(info.m_Children.begin() + index);
			if (child.hasComponent<Engine::Components::InfoComponent>())
				child.getComponent<Engine::Components::InfoComponent>().m_Parent = {};
		}
	}
}