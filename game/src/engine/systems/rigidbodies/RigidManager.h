#pragma once
#include "entt.hpp"
#include "Entity.h"
#include "SpatialHashGrid.h"

namespace Engine {
	namespace Systems {
		class RigidbodyManager {
		public:
			void update(entt::registry* scene, float delta);
		private:
			Engine::SpatialHashGrid<entt::entity> coliderGrid = { {0.0, 0.0, 10000.0, 10000.0} };
		};
	}
}