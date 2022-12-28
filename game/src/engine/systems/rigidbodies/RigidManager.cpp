#include "RigidManager.h"
#include "Components.h"
#include "Renderer.h"

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
		
		raylib::Rectangle absPlot = { absTrans.Position.x, absTrans.Position.y, colider.plot.width, colider.plot.height };
		if (!colider.inserted) {
			coliderGrid.addObject(absPlot, ent);
			colider.inserted = true;
		}
		
		//check trigger collisions
		if (colider.trigger && scene->all_of<Engine::Components::ScriptComponent>(ent)) {
			auto found = coliderGrid.getObjectsInRange({ absTrans.Position.x, absTrans.Position.y }, std::max(colider.plot.width, colider.plot.height) + 64);
			for (auto& susEnt : found) {
				if (susEnt != ent) {
					auto& susColider = scene->get<Engine::Components::ColiderComponent>(susEnt);
					Engine::Entity se = { susEnt, scene };
					auto aT = Renderer::getAbsoluteTransform(se);
					raylib::Rectangle susAbsPlot = { aT.Position.x, aT.Position.y, susColider.plot.width, susColider.plot.height };
					if (absPlot.CheckCollision(susAbsPlot)) {
						auto& script = scene->get<Engine::Components::ScriptComponent>(ent);
						script.instance->onTrigger({ susEnt, scene });
					}
				}
			}
		}
	});
	
}
