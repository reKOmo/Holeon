#include "TextureManager.h"
#include <memory>

namespace Engine {
	Renderer::TextureManager::~TextureManager() {
		printf("Unloading textures...\n");
		for (int i = 0; i < m_NumOfTextures; i++) {
			UnloadTexture(m_Textures[i]);
		}
	}

	void Renderer::TextureManager::loadFiles(char* files[], int n) {
		printf("Loading textures...\n");
		int succLoads = 0;
		//unload old textures
		for (int i = 0; i < m_NumOfTextures; i++) {
			UnloadTexture(m_Textures[i]);
		}
		m_Textures.reset();

		m_Textures = std::make_unique<raylib::Texture[]>(n);
		m_NumOfTextures = n;
		for (int i = 0; i < n; i++) {
			m_Textures[i] = LoadTexture(files[i]);
			if (m_Textures[i].height !=  0) {
				succLoads++;
			}
		}

		printf("Loaded %d/%d textures\n", succLoads, m_NumOfTextures);

	}

	raylib::Texture* Renderer::TextureManager::getTexture(int id) {
		if (id < 0 || id > m_NumOfTextures) return nullptr;
		raylib::Texture* a = &m_Textures[id];
		return a;
	}

	raylib::Rectangle Renderer::getPlotByIndex(int index, Engine::Renderer::Material& mat) {
		const int numOfTiles = mat.texture->width / mat.tilePlot.width;
		int x = index % numOfTiles;
		int y = index / numOfTiles;
		return { (float)(x * mat.tilePlot.width), (float)(y * mat.tilePlot.height), (float)mat.tilePlot.width, (float)mat.tilePlot.height };
	}

	

}
