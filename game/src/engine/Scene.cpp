#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "SceneManager.h"

namespace Engine{
	Scene::~Scene() {
		destroy();
	}

	Entity Scene::createEntity(std::string name) {
		Entity e = { m_Registry.create(), &m_Registry };
		auto& info = e.addComponent<Engine::Components::InfoComponent>(m_NextEntId);
		m_NextEntId++;
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

	std::vector<Engine::Entity> Scene::getEntitiesByTag(std::string tag) {
		std::vector<Engine::Entity> found;
		
		m_Registry.view<Engine::Components::InfoComponent>().each([&, tag](const auto ent, const auto& info) {
			if (Engine::findIndex(info.tags, tag) != -1) {
				found.push_back({ ent, &m_Registry });
			}
		});

		return found;
	}

	void Scene::update() {
		m_DeltaTime = GetFrameTime();
		/*
			1. Update scripts
			3. Update rigidbodies
			2. Update animations
		*/
		m_Registry.view<Components::ScriptComponent, Engine::Components::InfoComponent>().each([=](auto ent, auto &script, auto &info) {
			if (!script.instance) {
				script.instance = script.createFunction();
				script.instance->m_Owner = { ent, &m_Registry };
				script.instance->m_Scene = this;
				script.instance->onCreate();
			}
			if (!info.disabled && !script.paused) {
				script.instance->onUpdate(deltaTime());
			}
		});
		m_RigidMganager.update(&m_Registry, deltaTime());
		Engine::Systems::Animator::updateAnimations(m_Registry, deltaTime(), unscaledDeltaTime());
	}

	void Scene::render() {
		BeginDrawing();
		ClearBackground(m_BackgroundColor);
		m_Renderer2D->render(&m_Registry);
		m_RendererUI->render(&m_Registry);
		EndDrawing();
	}

	void Scene::setCamera(Entity& ent) {
		m_Renderer2D->setCamera(ent);
	}

	void Scene::destroy() {
		if (!m_Registry.empty()) {
			m_Registry.view<Components::ScriptComponent>().each([=](auto ent, auto& script) {
				if (script.instance != nullptr) {
					script.instance->onDestroy();
					script.destroyFunction(&script);
				}
			});
		}
	}

}


