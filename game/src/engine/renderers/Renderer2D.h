#pragma once
#include "raylib-cpp.hpp"
#include "TextureManager.h"
#include "entt.hpp"
#include "Entity.h"

namespace Engine {
	namespace Renderer {
		class Renderer2D {
		public:
			Renderer2D() = default;
			Renderer2D(entt::registry* r, TextureManager* t) : m_Registry(r), m_TextureMgr(t) {}
			Renderer2D& operator=(const Renderer2D& r) = default;
			
			
			void render();
			void setCamera(Engine::Entity c);
		private:
			Engine::Entity m_Camera;
			entt::registry* m_Registry;
			TextureManager* m_TextureMgr;
		};
	}
}