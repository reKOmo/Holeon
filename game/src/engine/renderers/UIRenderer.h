#pragma once
#include "entt.hpp"
#include "raylib-cpp.hpp"
#include "Helpers.h"

namespace Engine::Renderer {
	class UIRenderer {
	public:
		UIRenderer();
		void render(entt::registry* reg);

	private:
		raylib::Shader frameShader = LoadShader(0, RESOURCE_PATH("frame-renderer.fs"));
		int positionLocation = -1;
		int scaleLocation = -1;
	};
}