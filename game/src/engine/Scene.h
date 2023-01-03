#pragma once

#include "entt.hpp"
#include "Entity.h"
#include "Renderer2D.h"
#include "UIRenderer.h"
#include "FontManager.h"
#include "RigidManager.h"
#include "Collection.h"

namespace Engine {
	class SceneManager;
	class Scene {
	public:
		Scene(Renderer::TextureManager* mgr, Renderer::FontManager* fontmgr, Engine::SceneManager* smgr, Engine::Collection<std::string>* gS)
			: m_FontManager(fontmgr), m_TextureManager(mgr), m_SceneManager(smgr), m_GlobalStorage(gS) {}

		Scene(Scene&& other) noexcept
			: m_FontManager(other.m_FontManager), m_TextureManager(other.m_TextureManager), m_SceneManager(other.m_SceneManager), m_GlobalStorage(other.m_GlobalStorage)
		{
			destroy();

			m_Registry = std::move(other.m_Registry);
			m_Renderer2D = other.m_Renderer2D;
			m_RendererUI = other.m_RendererUI;
			m_RigidMganager = std::move(other.m_RigidMganager);
			m_BackgroundColor = other.m_BackgroundColor;
			m_TimeScale = other.m_TimeScale;
			m_NextEntId = other.m_NextEntId;

			other.m_Renderer2D = nullptr;
			other.m_RendererUI = nullptr;

		}

		Scene& operator=(Scene&& other) noexcept {
			if (this != &other) {
				destroy();

				m_Registry = std::move(other.m_Registry);
				m_Renderer2D = other.m_Renderer2D;
				m_RendererUI = other.m_RendererUI;
				m_RigidMganager = std::move(other.m_RigidMganager);
				m_BackgroundColor = other.m_BackgroundColor;
				m_TimeScale = other.m_TimeScale;
				m_NextEntId = other.m_NextEntId;

				other.m_Renderer2D = nullptr;
				other.m_RendererUI = nullptr;
			}

			return *this;
		}

		~Scene();
		
		Entity createEntity(std::string name = std::string());
		void removeEntity(Entity& e);
		Entity getEntityByName(std::string name);
		Entity getEntityById(uint32_t id);
		std::vector<Engine::Entity> getEntitiesByTag(std::string tag);

		void update();
		void render();

		void Scene::setCamera(Entity& ent);
		friend class Renderer2D;
		friend Engine::SceneManager;

		void setBackgroundColor(raylib::Color c) {
			m_BackgroundColor = c;
		}

		float deltaTime() {
			return m_DeltaTime * m_TimeScale;
		}

		float unscaledDeltaTime() {
			return m_DeltaTime;
		}

		void setTimeScale(float s) {
			m_TimeScale = s;
		}

		Renderer::FontManager* const m_FontManager = nullptr;
		Renderer::TextureManager* const m_TextureManager = nullptr;
		Engine::SceneManager* const m_SceneManager = nullptr;
		Engine::Collection<std::string>* const m_GlobalStorage = nullptr;
	private:
		uint32_t m_NextEntId = 0;
		entt::registry m_Registry;
		Renderer::Renderer2D* m_Renderer2D = nullptr;
		Renderer::UIRenderer* m_RendererUI = nullptr;
		Systems::RigidbodyManager m_RigidMganager = { {0.0, 0.0, 10000.0, 10000.0}, 32 };
		raylib::Color m_BackgroundColor = WHITE;
		float m_DeltaTime = 0.0;
		float m_TimeScale = 1.0;
		void destroy();
	};
}