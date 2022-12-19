#pragma once
#include "raylib-cpp.hpp"
#include "memory"

namespace Engine::Renderer {
	class FontManager {
	public:
		FontManager() : m_Fonts(std::make_unique<raylib::Font[]>(1)) {};
		~FontManager();
		void loadFiles(char* files[], int n);
		raylib::Font* getFont(int index);
	private:
		std::unique_ptr<raylib::Font[]> m_Fonts;
		int m_NumOfFonts = 0;

	};
}