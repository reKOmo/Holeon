#pragma once

#include "entt.hpp"
#include "Engine.h"

namespace Engine {
	class Scene {
	public:
		Entity createEntity();

		void removeEntity(Entity& e) {
			m_Registry.destroy(e.m_EnttID);
		}
	private:
		entt::registry m_Registry;
	};
}