#include "RigidManager.h"
#include "Components.h"
#include "Renderer.h"

/*

	@param a - point outside rect
	@param vel - veclocity
	@param rect - Colider rectangle
*/

raylib::Vector2 resolveCollision(raylib::Vector2 a, raylib::Vector2 vel, raylib::Rectangle rec) {
	float txN = vel.x != 0.0 ? std::min((rec.x - a.x) / vel.x, (rec.x + rec.width - a.x) / vel.x) : 0.0;
	float tyN = vel.y != 0.0 ? std::min((rec.y - a.y) / vel.y, (rec.y + rec.height - a.y) / vel.y) : 0.0;

	//time of collision
	float tcol = std::max(txN, tyN);

	return vel * tcol;
}

void Engine::Systems::RigidbodyManager::update(entt::registry* scene, float delta) {
	entt::entity tmp = entt::null;
	// updaye colider positions
	scene->view<Engine::Components::ColiderComponent>().each([&](auto ent, Engine::Components::ColiderComponent& colider) {
		Engine::Entity e = { ent, scene };
		auto absTrans = Renderer::getAbsoluteTransform(e);

		// re-add object if moved
		if (colider.prevPosition != absTrans.Position) {
			raylib::Rectangle plot = { colider.prevPosition.x, colider.prevPosition.y, colider.plot.width, colider.plot.height };
			coliderGrid.removeObject(plot, ent);
			colider.inserted = false;
			colider.prevPosition = absTrans.Position;
		}
		
		raylib::Rectangle absPlot = { absTrans.Position.x + colider.plot.x, absTrans.Position.y + colider.plot.y, colider.plot.width, colider.plot.height };
		if (!colider.inserted) {
			coliderGrid.addObject(absPlot, ent);
			colider.inserted = true;
		}
		// TODO resolve non existing entities !!!!!!!!!!!!!!!!!!!
		
		//check trigger collisions
		if (colider.trigger && scene->all_of<Engine::Components::ScriptComponent>(ent)) {
			auto found = coliderGrid.getObjectsInRange({ absTrans.Position.x, absTrans.Position.y }, std::max(colider.plot.width, colider.plot.height) + 64);
			std::vector<entt::entity> colidesWith;
			for (auto& susEnt : found) {
				if (susEnt != ent) {
					auto& susColider = scene->get<Engine::Components::ColiderComponent>(susEnt);
					Engine::Entity se = { susEnt, scene };
					auto aT = Renderer::getAbsoluteTransform(se);
					raylib::Rectangle susAbsPlot = { aT.Position.x + susColider.plot.x, aT.Position.y + susColider.plot.y, susColider.plot.width, susColider.plot.height };
					if (absPlot.CheckCollision(susAbsPlot)) {
						colidesWith.push_back(susEnt);
						auto& script = scene->get<Engine::Components::ScriptComponent>(ent);
						if (script) {
							script.instance->onTrigger({ susEnt, scene });
						}
					}
				}
			}
			// new coliders
			for (auto& col : colidesWith) {
				int index = Engine::findIndex(colider.colidingWith, col);
				if (index != -1) {
					colider.colidingWith.erase(colider.colidingWith.begin() + index);
				}
				else {
					auto& script = scene->get<Engine::Components::ScriptComponent>(ent);
					if (script) {
						script.instance->onTriggerEnter({ col, scene });
					}
				}
			}
			// exited coliders
			for (auto& col : colider.colidingWith) {
				auto& script = scene->get<Engine::Components::ScriptComponent>(ent);
				if (script) {
					script.instance->onTriggerExit({ col, scene });
				}
			}
			// replace
			colider.colidingWith = std::move(colidesWith);
		}
	});


	auto rigidGroup = scene->group<Engine::Components::ColiderComponent>(entt::get<Engine::Components::RigidbodyComponent, Engine::Components::TransformComponent, Engine::Components::InfoComponent>);
	for (auto& ent : rigidGroup) {
		auto& [colider, rigidbody, transform, info] = rigidGroup.get<Engine::Components::ColiderComponent, Engine::Components::RigidbodyComponent, Engine::Components::TransformComponent, Engine::Components::InfoComponent>(ent);

		if (!info.disabled) {
			Engine::Entity e = { ent, scene };
			auto absTransform = Renderer::getAbsoluteTransform(e);
			raylib::Vector2 coliderPos = { colider.plot.x, colider.plot.y };
			raylib::Vector2 coliderAbsPos = coliderPos + absTransform.Position;

			/*
				THIS APPROACH HAS PROBLEM WITH OBJECTS MOVING FASTER THEN 1 UNIT (32) PER SEC
			*/

			bool hit = false;
			raylib::Rectangle colidedWith;
			raylib::Vector2 coliderHalfSize = { colider.plot.width / 2, colider.plot.height / 2 };
			raylib::Vector2 coldierCenter = coliderAbsPos + coliderHalfSize;

			/*
				===============================
						TERRAIN COLLISION
				===============================
			*/
			//check collision with terrain
			auto terrains = scene->view<Engine::Components::TerrainComponent, Engine::Components::TransformComponent>();
			for (auto& terrainEnt : terrains) {
				auto& [terrain, terrainTrans] = scene->get<Engine::Components::TerrainComponent, Engine::Components::TransformComponent>(terrainEnt);
				Engine::Entity eTerr = { terrainEnt, scene };
				auto terrAbsTransform = Renderer::getAbsoluteTransform(eTerr);
				//colider position to terrain
				raylib::Vector2 offsetRgigidPosition = coliderAbsPos - terrAbsTransform.Position + rigidbody.velocity * delta;

				raylib::Vector2 tlCorner = { offsetRgigidPosition.x / terrain.material.tilePlot.width, offsetRgigidPosition.y / terrain.material.tilePlot.height };
				raylib::Vector2 brCorner = { 
					(offsetRgigidPosition.x + colider.plot.width) / terrain.material.tilePlot.width,
					(offsetRgigidPosition.y + colider.plot.height) / terrain.material.tilePlot.height 
				};

				int x = tlCorner.x >= 0 ? (int)tlCorner.x : -1;
				int y;
				for (; x <= (int)brCorner.x; x++) {
					y = tlCorner.y >= 0 ? (int)tlCorner.y : -1;
					for (; y <= (int)brCorner.y; y++) {
						int onTile = terrain.map(x, y);
						if (Engine::findIndex(terrain.map.walkableTiles, onTile) == -1) {
							hit = true;
							break;
						}
					}
					if (hit) break;
				}

				if (hit) {
					colidedWith = {
						terrAbsTransform.Position.x + x * terrain.material.tilePlot.width,
						terrAbsTransform.Position.y + y * terrain.material.tilePlot.height,
						terrain.material.tilePlot.width + coliderHalfSize.x,
						terrain.material.tilePlot.height + coliderHalfSize.y
					};
				}
			}

			/*
				===============================
						COLIDER COLLISION
				===============================
			*/
			raylib::Rectangle absPlot = { coliderAbsPos.x + rigidbody.velocity.x * delta, coliderAbsPos.y + rigidbody.velocity.y * delta, colider.plot.width, colider.plot.height };
			auto found = coliderGrid.getObjectsInRange({ absPlot.x, absPlot.y }, std::max(colider.plot.width, colider.plot.height) + 64);
			for (auto& susEnt : found) {
				if (susEnt != ent && scene->all_of<Engine::Components::RigidbodyComponent>(susEnt)) {
					auto& susColider = scene->get<Engine::Components::ColiderComponent>(susEnt);
					Engine::Entity se = { susEnt, scene };
					auto aT = Renderer::getAbsoluteTransform(se);
					raylib::Rectangle susAbsPlot = { aT.Position.x + susColider.plot.x, aT.Position.y + susColider.plot.y, susColider.plot.width, susColider.plot.height };
					if (absPlot.CheckCollision(susAbsPlot)) {
						hit = true;
						colidedWith = {susAbsPlot.x, susAbsPlot.y, susAbsPlot.width + coliderHalfSize.x, susAbsPlot.width + coliderHalfSize.y};
						break;
					}
				}
			}



			/*
				===============================
						MOVE OBJECT
				===============================
			*/
			if (!hit) {
				transform.Position += rigidbody.velocity * delta;
			}
			else {
				raylib::Vector2 ajustedVelocity = resolveCollision(coldierCenter, rigidbody.velocity * delta, colidedWith);
				if (ajustedVelocity.x != 0.0 && ajustedVelocity.y != 0.0) {
					transform.Position += ajustedVelocity;
				}
			}
		}
	}
}
