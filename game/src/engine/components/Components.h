#pragma once
#include "raylib-cpp.hpp"
#include "TextureManager.h"
#include "Script.h"
#include "TerrainMap.h"
#include <memory.h>
#include "AnimationClip.h"
#include "Helpers.h"
#include <functional>

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
			raylib::Vector2 scale = { 1.0, 1.0 };
			raylib::Color tint = WHITE;
			raylib::Vector2 origin = { 0,0 };
			int layer = 0;
			int zIndex = 0;

			int imageIndex = 0;
			int totalFrames = 1;

			SpriteComponent() = default;
			SpriteComponent(const SpriteComponent&) = default;
			SpriteComponent(Renderer::Material m, raylib::Vector2 scale = { 1.0f }, raylib::Vector2 o = { 0.0, 0.0 }, raylib::Color t = WHITE) : material(m), scale(scale), origin(o), tint(t) {
				totalFrames = (m.texture->height / (int)m.tilePlot.height) * (m.texture->width / (int)m.tilePlot.width);
			}

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
			bool paused = false;
			
			Engine::Script*(*createFunction)();
			void(*destroyFunction)(ScriptComponent*);

			template<typename T>
			void bind() {
				createFunction = []() { return static_cast<Script*>(new T()); };
				destroyFunction = [](ScriptComponent* comp) { delete comp->instance; comp->instance = nullptr; };
			}

			operator bool() const {
				return instance != nullptr;
			}
		};

		struct InfoComponent {
			std::string name = "";
			std::vector<std::string> tags = {};
			bool disabled = false;
			inline uint32_t id() {
				return m_Id;
			}

			InfoComponent() {}
			InfoComponent(uint32_t i) : m_Id(i) {}
			//InfoComponent(const InfoComponent&) = default;
			InfoComponent(std::string n, uint32_t i) : m_Id(i), name(n) {}
		private:
			Engine::Entity m_Parent = {};
			std::vector<Engine::Entity> m_Children = {};
			friend class Engine::Entity;
			uint32_t m_Id = 0;
		};

		struct AnimationManager {
			bool useUnscaledTime = false;
			std::shared_ptr<Engine::AnimationClip>* animations;
			int animationsSize = 0;
			std::vector<std::shared_ptr<Engine::AnimationClip>> currentlyPlaying = {};

			int playAnimation(int index, std::function<void()> onEnd = []() {}) {
				if (index > animationsSize || index < 0) return -1;
				currentlyPlaying.push_back(animations[index]);
				animations[index]->onEnd = onEnd;
				return currentlyPlaying.size() - 1;
			}

			template<typename T>
			int playAnimation(std::function<void()> onEnd = []() {}) {
				std::shared_ptr<AnimationClip> anim = static_cast<std::shared_ptr<AnimationClip>>(std::make_shared<T>());
				currentlyPlaying.push_back(anim);
				anim->onEnd = onEnd;

				return currentlyPlaying.size() - 1;
			}

			void stopAnimation(int index) {
				currentlyPlaying[index]->onDestroy();
				currentlyPlaying[index]->initialized = false;
				currentlyPlaying[index]->onEnd();

				currentlyPlaying.erase(currentlyPlaying.begin() + index);
			}

			~AnimationManager() {
				printf("Animation Manager out of scope\n");
				for (int i = 0; i < animationsSize; i++) {
					animations[i] = nullptr;
				}
			}
		};

		// Rigidbody, movement and stuff
		struct ColiderComponent {
			raylib::Rectangle plot;
			bool inserted = false;
			bool trigger = false;

			ColiderComponent() {}
			ColiderComponent(const ColiderComponent&) = default;
			ColiderComponent(raylib::Rectangle r) : plot(r) {}
			
		private:
			raylib::Vector2 prevPosition;
			std::vector<entt::entity> colidingWith = {};
			friend Engine::Systems::RigidbodyManager;
		};

		struct RigidbodyComponent {
			raylib::Vector2 velocity;

			RigidbodyComponent() {}
			RigidbodyComponent(const RigidbodyComponent&) = default;
		};

		// UI components
		struct BackgroundComponent {
			Renderer::Material material;
			raylib::Color tint = WHITE;
			raylib::Vector2 origin = { 0, 0 };
			raylib::Vector2 size = { 32.0, 16.0 };
			raylib::Vector2 scale = { 1.0, 1.0 };
			int zIndex = 0;

			int imageIndex = 0;
			int totalFrames = 1;

			BackgroundComponent() = default;
			BackgroundComponent(const BackgroundComponent&) = default;
			BackgroundComponent(Renderer::Material m, raylib::Vector2 size = { 32.0, 16.0 }, raylib::Vector2 o = { 0.0, 0.0 }, raylib::Color t = WHITE) : material(m), size(size), origin(o), tint(t) {
				totalFrames = (m.texture->height / (int)m.tilePlot.height) * (m.texture->width / (int)m.tilePlot.width);
			}

		};
		
		struct ImageComponent {
			Renderer::Material material;
			raylib::Vector2 scale = { 1.0, 1.0 };
			raylib::Color tint = WHITE;
			raylib::Vector2 origin = { 0,0 };
			int layer = 0;
			int zIndex = 0;

			int imageIndex = 0;
			int totalFrames = 1;

			ImageComponent() = default;
			ImageComponent(const ImageComponent&) = default;
			ImageComponent(Renderer::Material m, raylib::Vector2 scale = { 1.0f }, raylib::Vector2 o = { 0.0, 0.0 }, raylib::Color t = WHITE) : material(m), scale(scale), origin(o), tint(t) {
				totalFrames = (m.texture->height / (int)m.tilePlot.height) * (m.texture->width / (int)m.tilePlot.width);
			}

		};

		struct TextComponent {
			std::string text = "Empty text";
			raylib::Vector2 origin;
			float rotation = 0.0;
			float spacing = 0.0;
			float fontSize = 14.0;
			raylib::Color color;
			raylib::Font* font = nullptr;
			int zIndex = 0;

			TextComponent() = default;
			TextComponent(const TextComponent&) = default;
			TextComponent(std::string t) : text(t) {}
		};

		struct ButtonComponent {
			bool isSelected = false;
			Engine::Entity top = {};
			Engine::Entity bottom = {};
			Engine::Entity left = {};
			Engine::Entity right = {};

			int value = 0;

			ButtonComponent() = default;
			ButtonComponent(const ButtonComponent&) = default;
			ButtonComponent(int v) : value(v) {}
		};

		struct ButtonManagerData {
			Engine::Entity currentButton;

			ButtonManagerData() = default;
			ButtonManagerData(const ButtonManagerData&) = default;
			ButtonManagerData(Engine::Entity v) : currentButton(v) {}
		};

	}
}