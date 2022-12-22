#include "Scene.h"
#include "Entity.h"
#include "Components.h"

namespace Engine{
	Scene::Scene(Renderer::TextureManager* mgr, Renderer::FontManager* fontmgr)
		: m_FontManager(fontmgr), m_TextureManager(mgr)
	{
		m_Renderer2D = Renderer::Renderer2D(&m_Registry, mgr);
	}

	Scene::~Scene() {
		m_Registry.view<Components::ScriptComponent>().each([=](auto ent, auto& script) {
			script.instance->onDestroy();
			script.destroyFunction(&script);
		});
	}

	Entity Scene::createEntity(std::string name) {
		Entity e = { m_Registry.create(), &m_Registry };
		auto& info = e.addComponent<Engine::Components::InfoComponent>();
		info.name = name == "" ? "Entity" : name;
		return e;
	}

	void Scene::removeEntity(Entity& e) {
		auto& comp = e.getComponent<Components::ScriptComponent>();
		comp.instance->onDestroy();
		comp.destroyFunction(&comp);

		auto& children = e.getChildren();

		for (auto c : children) {
			removeEntity(c);
		}

		m_Registry.destroy(e.m_EnttID);
	}

	Entity Scene::getEntityByName(std::string name) {
		auto view = m_Registry.view<Engine::Components::InfoComponent>();
		for (auto ent : view) {
			if (view.get<Engine::Components::InfoComponent>(ent).name == name) {
				return { ent, &m_Registry };
			}
		}
		return Entity();
	}

	void Scene::update() {
		/*
			1. Update scripts
			2. Update animations
		*/
		m_Registry.view<Components::ScriptComponent, Engine::Components::InfoComponent>().each([=](auto ent, auto &script, auto &info) {
			if (!script.instance) {
				script.instance = script.createFunction();
				script.instance->m_Owner = { ent, &m_Registry };
				script.instance->m_Scene = this;
				script.instance->onCreate();
			}
			if (!info.disabled) {
				script.instance->onUpdate(GetFrameTime());
			}
		});

		Engine::Systems::Animator::updateAnimations(m_Registry, GetFrameTime());
	}

	void Scene::render() {
		BeginDrawing();
		ClearBackground(WHITE);
		m_Renderer2D.render();
		m_RendererUI.render(&m_Registry);
		EndDrawing();
	}

	void Scene::setCamera(Entity& ent) {
		m_Renderer2D.setCamera(ent);
	}

}


