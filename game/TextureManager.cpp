#include "TextureManager.h"
#include <memory>

namespace Engine {
	Renderer::TextureManager::~TextureManager() {
		printf("Unloading textures...");
		for (int i = 0; i < m_NumOfTextures; i++) {
			UnloadTexture(m_Textures[i]);
		}
	}

	void Renderer::TextureManager::loadFiles(char* files[], int n) {
		printf("Loading textures...");
		//unload old textures
		for (int i = 0; i < m_NumOfTextures; i++) {
			UnloadTexture(m_Textures[i]);
		}
		m_Textures.reset();

		m_Textures = std::make_unique<Texture[]>(n);
		for (int i = 0; i < n; i++) {
			m_Textures[i] = LoadTexture(files[i]);
		}

		printf("Loaded %d textures", n);

	}

	Texture* Renderer::TextureManager::getTexture(int id) {
		if (id < 0 || id > m_NumOfTextures) return nullptr;
		Texture* a = &m_Textures[id];
		return a;
	}

	Renderer::Material::getPlotByIndex(int index) {
		int x = (index * baseTilePlot.width) % textureId->width;
		int y = baseTilePlot.height * (index / (textureId->width / baseTilePlot.width));
		return {}
	}

}
