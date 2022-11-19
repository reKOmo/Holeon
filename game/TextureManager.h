#pragma once
#include <memory>
#include "raylib.h"

namespace Engine {
	namespace Renderer {
		class TextureManager {
		public:
			TextureManager() : m_NumOfTextures(0), m_Textures(std::make_unique<Texture[]>(1)) {};
			~TextureManager();
			void loadFiles(char* files[], int n);
			Texture* getTexture(int id);
		private:
			std::unique_ptr<Texture[]> m_Textures;
			int m_NumOfTextures;

		};

		struct Material {
			int textureId;
			Rectangle plot;	// {origin_x, origin_y, width, height}

			Material() = default;
			Material(const Material&) = default;
			Material(int id, Rectangle p) : textureId(id), plot(p) {}
		};

	}
}