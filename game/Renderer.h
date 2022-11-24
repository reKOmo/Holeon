#pragma once
#include "raylib.h"
#include "TextureManager.h"
#include "entt.hpp"
#include "Entity.h"

namespace Engine {
	namespace Renderer {
		class Renderer2D {
		public:
			Renderer2D() = default;
			Renderer2D(entt::registry* r, TextureManager* t) : m_Registry(r), m_TextureMgr(t) {}

			Renderer2D(const Renderer2D& r);
			Renderer2D& operator=(const Renderer2D& r) = default;

			~Renderer2D();
			
			
			void render();
			void setCamera(Entity& c);
		private:
			Entity* m_Camera = nullptr;
			entt::registry* m_Registry;
			TextureManager* m_TextureMgr;
		};
	}
}