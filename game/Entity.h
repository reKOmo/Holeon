#pragma once

#include "entt.hpp"


namespace Engine {
	class Entity {
	public:
		Entity(entt::entity id, entt::registry* reg);
		Entity(const Entity& other) = default;

		template<typename T>
		T getComponent() {
			return m_Scene->get<T>(m_EnttID);
		}

		template<typename T, typename... Args>
		T& addComponent() {
			return m_Scene->emplace<T>(m_EnttID, std::forward(Args));
		}

		template<typename T>
		bool hasComponent() {
			return m_Scene->has<T>(m_EnttID);
		}

		template<typename T>
		void removeComponent() {
			m_Scene->remove<T>(m_EnttID);
		}


	private:
		entt::entity m_EnttID;
		entt::registry* m_Scene;
		friend class Scene;
	};
}