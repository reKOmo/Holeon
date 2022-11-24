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
			Texture* getTexture(int index);
		private:
			std::unique_ptr<Texture[]> m_Textures;
			int m_NumOfTextures;

		};

		class Material {
		private:
			Texture* textureId = nullptr;
			Rectangle baseTilePlot;	// {origin_x, origin_y, width, height}
		
		public:
			Material();
			Material(const Material&) = default;
			Material(Texture* id, Rectangle p) : textureId(id), plot(p) {}

			Rectangle getPlotByIndex(int index);
		};

	}
}