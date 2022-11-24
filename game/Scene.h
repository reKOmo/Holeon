#pragma once

#include "entt.hpp"
#include "Engine.h"

namespace Engine {
	class Scene {
	public:
		Scene(Renderer::TextureManager* mgr);
		~Scene();
		Entity createEntity();

		void removeEntity(Entity& e);
		void update();
		void render();
		void Scene::setCamera(Entity& ent);
		friend class Renderer2D;
	private:
		entt::registry m_Registry;
		Renderer::Renderer2D m_Renderer;
	};
}