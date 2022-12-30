#pragma once
#include "raylib-cpp.hpp"
#include "TextureManager.h"
#include "entt.hpp"
#include "Entity.h"

namespace Engine {
	namespace Renderer {
		class Renderer2D {
		public:
			void render(entt::registry* scene);
			void setCamera(Engine::Entity c);
		private:
			Engine::Entity m_Camera;
		};
	}
}