#include "FontManager.h"

Engine::Renderer::FontManager::~FontManager() {
	printf("Unloading fonts...\n");
	for (int i = 0; i < m_NumOfFonts; i++) {
		m_Fonts[i].Unload();
	}
}

void Engine::Renderer::FontManager::loadFiles(char* files[], int n) {
	printf("Loading fonts...\n");
	int succLoads = 0;
	//unload old textures
	for (int i = 0; i < m_NumOfFonts; i++) {
		UnloadFont(m_Fonts[i]);
	}
	m_Fonts.reset();

	m_Fonts = std::make_unique<raylib::Font[]>(n);
	m_NumOfFonts = n;
	for (int i = 0; i < n; i++) {
		m_Fonts[i] = LoadFont(files[i]);
		if (m_Fonts[i].glyphs != 0) {
			succLoads++;
		}
	}

	printf("Loaded %d/%d fonts\n", succLoads, m_NumOfFonts);
}

raylib::Font* Engine::Renderer::FontManager::getFont(int index) {
	if (index < 0 || index > m_NumOfFonts) return nullptr;
	raylib::Font* a = &m_Fonts[index];
	return a;
}
