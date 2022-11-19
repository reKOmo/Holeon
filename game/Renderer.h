#pragma once
#include "raylib.h"
#include "TextureManager.h"
#include "entt.hpp"

namespace Engine {
	namespace Renderer {
		class Renderer2D {
		public:
			Renderer2D(entt::registry* r, TextureManager& t) : m_Registry(r), m_TextureMgr(t) {}
			~Renderer2D();
			void render();
			void setScene(entt::registry* r);
		private:
			entt::registry* m_Registry;
			TextureManager& m_TextureMgr;
		};
	}
}