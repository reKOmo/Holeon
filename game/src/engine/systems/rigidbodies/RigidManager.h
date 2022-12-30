#pragma once
#include "entt.hpp"
#include "Entity.h"
#include "SpatialHashGrid.h"

namespace Engine {
	namespace Systems {
		class RigidbodyManager {
		public:
			RigidbodyManager() {}

			RigidbodyManager(raylib::Rectangle bounds, int chunkSize = 32)
				: coliderGrid(bounds, chunkSize) {}

			RigidbodyManager(RigidbodyManager&& other) noexcept {
				coliderGrid = std::move(other.coliderGrid);
			}

			RigidbodyManager& operator=(RigidbodyManager&& other) noexcept {
				if (this != &other) {
					coliderGrid = std::move(other.coliderGrid);
				}

				return *this;
			}

			void update(entt::registry* scene, float delta);
		private:
			Engine::SpatialHashGrid<entt::entity> coliderGrid;
		};
	}
}