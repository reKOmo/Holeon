#include "Renderer.h"
#include "Components.h"

void Engine::Renderer::Renderer2D::render() {
	auto group = m_Registry->group<Engine::Components::SpriteComponent>(entt::get<Engine::Components::TransformComponent>);

	BeginDrawing();
	ClearBackground(RAYWHITE);
	for (auto ent : group) {
		auto& [sprite, transform] = group.get<Engine::Components::SpriteComponent, Engine::Components::TransformComponent>(ent);

		Texture t = Texture(*m_TextureMgr.getTexture(sprite.Material.textureId));
		Rectangle destination = {transform.Position.x, transform.Position.y, sprite.Material.plot.width * sprite.Scale.x, sprite.Material.plot.height * sprite.Scale.x };


		DrawTexturePro(t, sprite.Material.plot, destination, sprite.origin, transform.Rotation, sprite.tint);
	}
	EndDrawing();
}

Engine::Renderer::Renderer2D::~Renderer2D()
{
}


