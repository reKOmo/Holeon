#include "SceneManager.h"

void Engine::SceneManager::loadScene(int id) {
	if (id < 0 || id >= m_Scenes.size()) return;
	
	m_SwitchScenes = true;

	if (m_ActicveSceneId == -1) {
		loadSceneNow(id);
	}

	m_ActicveSceneId = id;
}

void Engine::SceneManager::tick() {
	if (m_SwitchScenes) {
		loadSceneNow(m_ActicveSceneId);
		return;
	}

	m_ActiveScene.update();
	m_ActiveScene.render();
}

void Engine::SceneManager::loadSceneNow(int id) {
	printf("Loading scene %d...\n", id);
	Engine::Scene scene(m_TxtManager, m_FontManager, this);

	scene.m_Renderer2D = &m_Renderer2D;
	scene.m_RendererUI = &m_RendererUI;

	m_ActiveScene = std::move(scene);

	m_Scenes[id](m_ActiveScene);

	m_SwitchScenes = false;
}
