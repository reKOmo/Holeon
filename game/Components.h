#pragma once
#include "raylib.h"
#include "Renderer.h"
#include "Script.h"
#include "TerrainMap.h"
#include <memory.h>

namespace Engine {
	namespace Components {
		struct TransformComponent {
			Vector3 Position;
			float Rotation;

			TransformComponent() = default;
			TransformComponent(const TransformComponent&) = default;
			TransformComponent(const Vector3 p, float r = 0.0f) : Position(p), Rotation(r) {}
		};

		struct SpriteComponent {
			Renderer::Material Material;
			Vector2 Scale;
			Color tint;
			Vector2 origin;

			SpriteComponent() = default;
			SpriteComponent(const SpriteComponent&) = default;
			SpriteComponent(Renderer::Material m, Vector2 scale = { 1.0f }, Vector2 o = {0.0, 0.0}, Color t = WHITE) : Material(m), Scale(scale), origin(o), tint(t) {}
		};

		struct TerrainComponent {
			Renderer::Material material;
			TerrainMap map;

			TerrainComponent() = default;
			TerrainComponent(const TerrainComponent&) = default;
			TerrainComponent(Renderer::Material m, TerrainMap& map) : material(m), map(map) {}
		};

		struct ScriptComponent {
			Script* instance = nullptr;
			
			Script*(*createFunction)();
			void(*destroyFunction)(ScriptComponent*);

			template<typename T>
			void bind() {
				createFunction = []() { return static_cast<Script*>(new T()); };
				destroyFunction = [](ScriptComponent* comp) { delete comp->instance; comp->instance = nullptr; };
			}
		};
	}
}