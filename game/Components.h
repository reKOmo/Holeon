#pragma once
#include "raylib-cpp.hpp"
#include "Renderer.h"
#include "Script.h"
#include "TerrainMap.h"
#include <memory.h>
#include "Engine.h"
#include "AnimationClip.h"

namespace Engine {
	namespace Components {
		struct TransformComponent {
			raylib::Vector2 Position;
			float Rotation = 0;

			TransformComponent() = default;
			TransformComponent(const TransformComponent&) = default;
			TransformComponent(const raylib::Vector2 p, float r = 0.0f) : Position(p), Rotation(r) {}
		};

		struct SpriteComponent {
			Renderer::Material material;
			raylib::Vector2 scale = {0,0};
			raylib::Color tint = WHITE;
			raylib::Vector2 origin = {0,0};
			int layer = 0;
			int zIndex = 0;

			SpriteComponent() = default;
			SpriteComponent(const SpriteComponent&) = default;
			SpriteComponent(Renderer::Material m, raylib::Vector2 scale = { 1.0f }, raylib::Vector2 o = {0.0, 0.0}, raylib::Color t = WHITE) : material(m), scale(scale), origin(o), tint(t) {}
		};

		struct AnimatedSpriteComponent {
			Renderer::Material material;
			raylib::Vector2 scale = { 0,0 };
			raylib::Color tint = WHITE;
			raylib::Vector2 origin = { 0,0 };
			int layer = 0;
			int zIndex = 0;

			int imageIndex = 0;

			AnimatedSpriteComponent() = default;
			AnimatedSpriteComponent(const AnimatedSpriteComponent&) = default;
			AnimatedSpriteComponent(Renderer::Material m, raylib::Vector2 scale = { 1.0f }, raylib::Vector2 o = { 0.0, 0.0 }, raylib::Color t = WHITE) : material(m), scale(scale), origin(o), tint(t) {}

		};

		struct TerrainComponent {
			Renderer::Material material;
			TerrainMap map;

			TerrainComponent() = default;
			TerrainComponent(const TerrainComponent&) = default;
			TerrainComponent(Renderer::Material m, TerrainMap& map) : material(m), map(map) {}
		};

		struct ScriptComponent {
			Engine::Script* instance = nullptr;
			
			Engine::Script*(*createFunction)();
			void(*destroyFunction)(ScriptComponent*);

			template<typename T>
			void bind() {
				createFunction = []() { return static_cast<Script*>(new T()); };
				destroyFunction = [](ScriptComponent* comp) { delete comp->instance; comp->instance = nullptr; };
			}
		};

		struct InfoComponent {
			std::string name = "";
			std::vector<std::string> tags = {};
			Engine::Entity parent = {};

			InfoComponent() = default;
			InfoComponent(const InfoComponent&) = default;
			InfoComponent(std::string n) : name(n) {}
		};

		struct AnimationManager {
			std::shared_ptr<Engine::AnimationClip>* animations;
			int animationsSize = 0;
			std::vector<std::shared_ptr<Engine::AnimationClip>> currentlyPlaying;

			void playAnimation(int index) {
				if (index > animationsSize || index < 0) return;
				currentlyPlaying.push_back(animations[index]);
			}

			template<typename T>
			void playAnimation() {
				currentlyPlaying.push_back(make_shared<T>());
			}
		};
	}
}