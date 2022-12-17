#pragma once
#include <memory>
#include "raylib-cpp.hpp"

namespace Engine {
	namespace Renderer {
		class TextureManager {
		public:
			TextureManager() : m_NumOfTextures(0), m_Textures(std::make_unique<raylib::Texture[]>(1)) {};
			~TextureManager();
			void loadFiles(char* files[], int n);
			raylib::Texture* getTexture(int index);
		private:
			std::unique_ptr<raylib::Texture[]> m_Textures;
			int m_NumOfTextures;

		};

		struct Material {
			raylib::Texture* texture = nullptr;
			raylib::Rectangle tilePlot = {};	// {origin_x, origin_y, width, height}
			
			Material() {}
			Material(const Material&) = default;
			Material(raylib::Texture* id, raylib::Rectangle p) : texture(id), tilePlot(p) {}
		};

		raylib::Rectangle getPlotByIndex(int index, Engine::Renderer::Material& mat);

	}
}