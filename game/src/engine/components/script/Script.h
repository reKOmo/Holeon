#pragma once
#include "Entity.h"
#include "Scene.h"
#include "Collection.h"

namespace Engine {
	class Script {
	public:
		virtual ~Script() {}
	protected:
		Engine::Entity m_Owner;
		bool paused = false;
		
		virtual void onCreate() {}
		virtual void onUpdate(float delta) {}
		virtual void onDestroy() {}
		
		/*
			Called when colided with colider makred as trigger
		*/
		virtual void onTrigger(Engine::Entity other) {}
		virtual void onTriggerEnter(Engine::Entity other) {}
		virtual void onTriggerExit(Engine::Entity other) {}

		Engine::Entity getEntityByName(std::string name);
		std::vector<Engine::Entity> getEntitiesByTag(std::string tag);
		inline Engine::Entity getEntityById(uint32_t id) {
			return m_Scene->getEntityById(id);
		}

		inline float deltaTime() {
			return m_Scene->deltaTime();
		}

		inline float unscaledDeltaTime() {
			return m_Scene->unscaledDeltaTime();
		}
		
		/*
			Lambda passed to this function should return entity that is highest in hiearchy
		*/
		Engine::Entity Instattiate(std::function<Engine::Entity(Engine::Scene* scene)>);

		Engine::SceneManager& getSceneManager();
		inline Engine::Collection<std::string>& getGlobalStorage() {
			return *m_Scene->m_GlobalStorage;
		}
	private:
		Engine::Scene* m_Scene = nullptr;
		friend class Scene;
		friend Engine::Systems::RigidbodyManager;
	};
}