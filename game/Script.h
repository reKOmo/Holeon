#pragma once
#include "Entity.h"
#include "Scene.h"

namespace Engine {
	class Script {
	protected:
		Engine::Entity m_Owner;
		virtual void onCreate() {}
		virtual void onUpdate(float delta) {}
		virtual void onDestroy() {}
		Engine::Entity getEntityByName(std::string name);
	private:
		Engine::Scene* m_Scene = nullptr;
		friend class Scene;
	};
}