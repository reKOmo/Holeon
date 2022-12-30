#pragma once
#include <vector>

#include "Scene.h"
#include "Renderer2D.h"
#include "UIRenderer.h"

namespace Engine {
	class SceneManager {
	public:
		SceneManager(Engine::Renderer::TextureManager* txt, Engine::Renderer::FontManager* ft)
			: m_TxtManager(txt), m_FontManager(ft), m_ActiveScene(txt, ft, this) {}

		void loadScene(int id);
		Engine::Scene& getActveScene() {
			return m_ActiveScene;
		}
		int addScene(std::function<void(Engine::Scene&)> sc) {
			m_Scenes.push_back(sc);

			return m_Scenes.size() - 1;
		}
		void tick();
		friend Engine::Scene;
	private:
		void loadSceneNow(int id);
		bool m_SwitchScenes = false;
		int m_ActicveSceneId = -1;
		std::vector<std::function<void(Engine::Scene&)>> m_Scenes;
		Engine::Scene m_ActiveScene;
		Engine::Renderer::TextureManager* m_TxtManager;
		Engine::Renderer::FontManager* m_FontManager;
		Engine::Renderer::Renderer2D m_Renderer2D;
		Engine::Renderer::UIRenderer m_RendererUI;
	};
}