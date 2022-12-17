#include "UIRenderer.h"
#include "Components.h"
#include "Renderer.h"
#include "raylib.hpp"

Engine::Renderer::UIRenderer::UIRenderer() {
	positionLocation = frameShader.GetLocation("spriteSize");
}

void Engine::Renderer::UIRenderer::render(entt::registry* registry) {
	//render all backgrounds
	
	auto backgrounds = registry->group<Engine::Components::BackgroundComponent>(entt::get<Engine::Components::TransformComponent>);
	backgrounds.sort<Engine::Components::BackgroundComponent>([](const Engine::Components::BackgroundComponent& a, const Engine::Components::BackgroundComponent& b){
		return a.zIndex < b.zIndex;
	});
	for (const auto ent : backgrounds) {
		auto& [background, transform] = backgrounds.get<Components::BackgroundComponent, Components::TransformComponent>(ent);

		frameShader.SetValue(positionLocation, &background.size, SHADER_UNIFORM_VEC2);
		BeginShaderMode(frameShader);
		Entity e = { ent, registry };
		auto absTransform = getAbsoluteTransform(e);

		raylib::Rectangle source = background.material.tilePlot;

		raylib::Rectangle destination = { absTransform.Position.x, absTransform.Position.y, background.size.x * background.scale.x, background.size.y * background.scale.y };

		DrawTexturePro(Texture(*background.material.texture), source, destination, background.origin, absTransform.Rotation, background.tint);
		EndShaderMode();
	}
	
	/*
	//render all text
	auto text = registry->group<Engine::Components::TextComponent>(entt::get<Engine::Components::TransformComponent>);
	text.sort<Engine::Components::TextComponent>([](const Engine::Components::TextComponent& a, const Engine::Components::TextComponent& b) {
		return a.zIndex < b.zIndex;
	});

	for (auto ent : text) {
		auto& [textComp, transform] = text.get<Engine::Components::TextComponent, Engine::Components::TransformComponent>(ent);

		Entity e = { ent, registry };
		auto absTransform = getAbsoluteTransform(e);

		DrawTextPro(textComp.font, textComp.text, absTransform.Position, textComp.origin, textComp.rotation, textComp.fontSize, textComp.spacing, textComp.color);
	}
	*/
}
