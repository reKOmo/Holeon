#include "Renderer2D.h"
#include "Components.h"
#include "raylib.hpp"
#include "Renderer.h"

void Engine::Renderer::Renderer2D::render(entt::registry* registry) {
	if (m_Camera) {
		auto& cam = m_Camera.getComponent<raylib::Camera2D>();

		BeginMode2D(cam);
		//render terrain
		auto terrainGruop = registry->group<Engine::Components::TerrainComponent>(entt::get<Engine::Components::TransformComponent>);

		if (terrainGruop.size() > 0) {
			for (auto ent : terrainGruop) {
				auto& [terrain, transform] = terrainGruop.get<Engine::Components::TerrainComponent, Engine::Components::TransformComponent>(ent);

				for (int x = 0; x < terrain.map.width(); x++) {
					for (int y = 0; y < terrain.map.height(); y++) {
						int textureId = terrain.map(x, y);
						if (textureId != -1) {
							raylib::Rectangle plot = getPlotByIndex(textureId, terrain.material);
							raylib::Rectangle destinaion = { transform.Position.x + terrain.material.tilePlot.width * (float)x, transform.Position.y + terrain.material.tilePlot.height * (float)y, plot.width, plot.height };
							DrawTexturePro(Texture(*terrain.material.texture), plot, destinaion, { 0.0, 0.0 }, transform.Rotation, WHITE);
						}
					}
				}

			}
		}


		//render sprites
		auto group = registry->group<Engine::Components::SpriteComponent>(entt::get<Engine::Components::TransformComponent, Engine::Components::InfoComponent>);
		group.sort<Engine::Components::SpriteComponent, Engine::Components::TransformComponent>([](const auto& lhs, const auto& rhs) {
			auto& [lSprite, lTrans] = lhs;
			auto& [rSprite, rTrans] = rhs;

			bool res;
			if (lSprite.layer == rSprite.layer && lSprite.zIndex == rSprite.zIndex)
				res = lTrans.Position.y < rTrans.Position.y;
			else
				res = lSprite.layer < rSprite.layer || lSprite.zIndex < rSprite.zIndex;
			return res;
		});
		for (auto ent : group) {
			auto& [sprite, transform, info] = group.get<Engine::Components::SpriteComponent, Engine::Components::TransformComponent, Engine::Components::InfoComponent>(ent);

			Entity e = { ent, registry };
			auto absTransform = getAbsoluteTransform(e);

			raylib::Rectangle source = sprite.imageIndex == 0 ? sprite.material.tilePlot : getPlotByIndex(sprite.imageIndex, sprite.material);

			raylib::Rectangle destination = { absTransform.Position.x - sprite.origin.x * sprite.scale.x, absTransform.Position.y - sprite.origin.y * sprite.scale.y, sprite.material.tilePlot.width * sprite.scale.x, sprite.material.tilePlot.height * sprite.scale.x };


			DrawTexturePro(Texture(*sprite.material.texture), source, destination, sprite.origin, absTransform.Rotation, sprite.tint);
		}

		//render coliders -- debug
		/*
		registry->view<Engine::Components::ColiderComponent>().each([&](auto ent, Engine::Components::ColiderComponent& colider) {
			Engine::Entity e = { ent, registry };
			auto pos = getAbsoluteTransform(e);
			raylib::Color c;
			if (colider.trigger)
				c = { 255, 0, 0, 30 };
			else
				c ={ 0, 255, 0, 30 };
			DrawRectangle(pos.Position.x + colider.plot.x, pos.Position.y + colider.plot.y, colider.plot.width, colider.plot.height, c);
		});
		*/
		

		EndMode2D();
	}
}

void Engine::Renderer::Renderer2D::setCamera(Entity c) {
	m_Camera = c;
}


