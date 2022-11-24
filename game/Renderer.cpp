#include "Renderer.h"
#include "Components.h"

void Engine::Renderer::Renderer2D::render() {
	auto& cam = m_Camera->getComponent<Camera2D>();
	

	BeginDrawing();
	ClearBackground(RAYWHITE);
	BeginMode2D(cam);
	//render terrain
	auto terrainGruop = m_Registry->group<Engine::Components::TerrainComponent>(entt::get<TransformComponent>);
	for (auto ent : terrainGroup) {
		auto& [terrain, transform] = terrainGroup.get<Engine::Components::TerrainComponent, Engine::Components::TransformComponent>(ent);

	}
	//render sprites
	auto group = m_Registry->group<Engine::Components::SpriteComponent>(entt::get<Engine::Components::TransformComponent>);
	for (auto ent : group) {
		auto& [sprite, transform] = group.get<Engine::Components::SpriteComponent, Engine::Components::TransformComponent>(ent);

		Rectangle destination = {transform.Position.x, transform.Position.y, sprite.Material.baseTilePlot.width * sprite.Scale.x, sprite.Material.baseTilePlot.height * sprite.Scale.x };


		DrawTexturePro(Texture(*sprite.material.texture), sprite.Material.plot, destination, sprite.origin, transform.Rotation, sprite.tint);
	}
	EndMode2D();
	EndDrawing();
}

void Engine::Renderer::Renderer2D::setCamera(Entity& c) {
	m_Camera = &c;
}

Engine::Renderer::Renderer2D::Renderer2D(const Renderer2D& r)
{
}

Engine::Renderer::Renderer2D::~Renderer2D()
{
}


