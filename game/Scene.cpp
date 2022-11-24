#include "Scene.h"
#include "Entity.h"
#include "Components.h"

namespace Engine{
	Scene::Scene(Renderer::TextureManager* mgr) {
		m_Renderer = Renderer::Renderer2D(&m_Registry, mgr );
	}
	Scene::~Scene() {
		m_Registry.view<Components::ScriptComponent>().each([=](auto ent, auto& script) {
			script.instance->onDestroy();
			script.destroyFunction(&script);
		});
	}
	Entity Scene::createEntity() {
		Entity e = { m_Registry.create(), &m_Registry };
		return e;
	}
	void Scene::removeEntity(Entity& e) {
		auto& comp = e.getComponent<Components::ScriptComponent>();
		comp.instance->onDestroy();
		comp.destroyFunction(&comp);
		m_Registry.destroy(e.m_EnttID);
	}
	void Scene::update() {
		m_Registry.view<Components::ScriptComponent>().each([=](auto ent, auto &script) {
			if (!script.instance) {
				script.instance = script.createFunction();
				script.instance->m_Owner = { ent, &m_Registry };
				script.instance->onCreate();
			}
			script.instance->onUpdate(GetFrameTime());
		});
	}
	void Scene::render() {
		m_Renderer.render();
	}
	void Scene::setCamera(Entity& ent) {
		m_Renderer.setCamera(ent);
	}
}


