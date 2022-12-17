#pragma once

#include "entt.hpp"
#include "Entity.h"
#include "Renderer2D.h"
#include "UIRenderer.h"

namespace Engine {
	class Scene {
	public:
		Scene(Renderer::TextureManager* mgr);
		~Scene();
		
		Entity createEntity(std::string name = std::string());
		void removeEntity(Entity& e);
		Entity getEntityByName(std::string name);

		void update();
		void render();

		void Scene::setCamera(Entity& ent);
		friend class Renderer2D;


	private:
		entt::registry m_Registry;
		Renderer::Renderer2D m_Renderer2D;
		Renderer::UIRenderer m_RendererUI;
	};
}