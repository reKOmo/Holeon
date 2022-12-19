#pragma once
#include "Entity.h"
#include "Scene.h"

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
		Engine::Entity getEntityByName(std::string name);
		/*
			Lambda passed to this function should return entity that is highest in hiearchy
		*/
		Engine::Entity Instattiate(std::function<Engine::Entity(Engine::Scene* scene)>);
	private:
		Engine::Scene* m_Scene = nullptr;
		friend class Scene;
	};
}