#include "UIRenderer.h"
#include "Components.h"
#include "Renderer.h"
#include "raylib.hpp"

Engine::Renderer::UIRenderer::UIRenderer() {
	positionLocation = frameShader.GetLocation("spriteSize");
}

void Engine::Renderer::UIRenderer::render(entt::registry* registry) {
	//render all backgrounds
	auto backgrounds = registry->group<Engine::Components::BackgroundComponent>(entt::get<Engine::Components::TransformComponent, Engine::Components::InfoComponent>);
	backgrounds.sort<Engine::Components::BackgroundComponent>([](const Engine::Components::BackgroundComponent& a, const Engine::Components::BackgroundComponent& b) {
		return a.zIndex < b.zIndex;
		});
	for (const auto ent : backgrounds) {
		auto [background, transform, info] = backgrounds.get(ent);

		if (!info.disabled) {
			frameShader.SetValue(positionLocation, &background.size, SHADER_UNIFORM_VEC2);
			BeginShaderMode(frameShader);
			Entity e = { ent, registry };
			auto absTransform = getAbsoluteTransform(e);

			raylib::Rectangle source = background.material.tilePlot;

			raylib::Rectangle destination = { absTransform.Position.x, absTransform.Position.y, background.size.x * background.scale.x, background.size.y * background.scale.y };

			DrawTexturePro(Texture(*background.material.texture), source, destination, background.origin, absTransform.Rotation, background.tint);
			EndShaderMode();
		}
	}
	
	//render all text
	//auto text = registry->group<Engine::Components::TextComponent>(entt::get<Engine::Components::TransformComponent>);
	auto text = registry->group<Engine::Components::TextComponent>(entt::get<Engine::Components::TransformComponent, Engine::Components::InfoComponent>);
	/*
	text.sort<Engine::Components::TextComponent>([](const Engine::Components::TextComponent& a, const Engine::Components::TextComponent& b) {
		return a.zIndex < b.zIndex;
	});
	*/
	
	for (const auto ent : text) {
		auto& [textComp, transform, info] = text.get(ent);

		if (!info.disabled) {
			Entity e = { ent, registry };
			auto absTransform = getAbsoluteTransform(e);

			DrawTextPro((*textComp.font), textComp.text, absTransform.Position, textComp.origin, textComp.rotation, textComp.fontSize, textComp.spacing, textComp.color);
		}
	}
}
