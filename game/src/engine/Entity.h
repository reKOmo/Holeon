#pragma once

#include "entt.hpp"

namespace Engine {
	class Entity {
	public:
		Entity() {};
		Entity(entt::entity id, entt::registry* reg);
		Entity(const Entity& other) : m_EnttID(other.m_EnttID), m_Scene(other.m_Scene) {};

		template<typename T>
		T& getComponent() {
			return m_Scene->get<T>(m_EnttID);
		}

		template<typename T, typename... Args>
		T& addComponent(Args&&... args) {
			return m_Scene->emplace<T>(m_EnttID, std::forward<Args>(args)...);
		}

		template<typename T>
		bool hasComponent() {
			return m_Scene->all_of<T>(m_EnttID);
		}

		template<typename T>
		void removeComponent() {
			m_Scene->remove<T>(m_EnttID);
		}

		Engine::Entity getParent();
		void setParent(Engine::Entity p);
		std::vector<Engine::Entity> getChildren();
		void removeChild(Engine::Entity child);

		bool disabled();
		void disable();
		void enable();

		operator bool() const {
			return m_EnttID != entt::null;
		}
		bool operator ==(const Entity& other) const {
			return m_EnttID == other.m_EnttID;
		}
		bool operator !=(const Entity& other) const {
			return m_EnttID != other.m_EnttID;
		}
		bool operator <(const Entity& other) const {
			return m_EnttID < other.m_EnttID;
		}
	private:
		entt::entity m_EnttID = entt::null;
		entt::registry* m_Scene = nullptr;
		friend class Scene;
	};
}

