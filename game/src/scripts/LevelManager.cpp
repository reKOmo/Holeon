#include "LevelManager.h"
#include "NestTrap.h"

#include "json.hpp"
using json = nlohmann::json;
#include <fstream>

Engine::Entity createTerrain(Engine::Scene* scene) {
	auto terrain = scene->createEntity();
	terrain.addComponent<Engine::Components::TerrainComponent>();
	terrain.addComponent<Engine::Components::TransformComponent>();

	return terrain;
}

Engine::Entity createGrass(Engine::Scene* scene) {
	auto grass = scene->createEntity("grass");
	grass.addComponent<Engine::Components::TransformComponent>();
	Engine::Renderer::Material mat = { scene->m_TextureManager->getTexture(2), {0.0, 0.0, 32.0, 32.0} };
	grass.addComponent<Engine::Components::SpriteComponent>(mat);

	return grass;
}

Engine::Entity createNest(Engine::Scene* scene) {
	auto nest = scene->createEntity("rat-nest");
	nest.addComponent<Engine::Components::TransformComponent>();
	Engine::Renderer::Material mat = { scene->m_TextureManager->getTexture(9), {0.0, 0.0, 32.0, 32.0} };
	nest.addComponent<Engine::Components::SpriteComponent>(mat);
	auto& colider = nest.addComponent<Engine::Components::ColiderComponent>();
	colider.plot = { 8.0, 22.0, 17.0, 6.0 };
	nest.addComponent<Engine::Components::RigidbodyComponent>();

	auto nesttrigger = scene->createEntity("trigger");
	auto& triggerTrans = nesttrigger.addComponent<Engine::Components::TransformComponent>();
	triggerTrans.Position = { 11.0, 28.0 };
	auto& triggerColider = nesttrigger.addComponent<Engine::Components::ColiderComponent>();
	triggerColider.plot = { 0.0, 0.0, 12.0, 20.0 };
	triggerColider.trigger = true;
	nesttrigger.addComponent<Engine::Components::ScriptComponent>().bind<NestTrap>();
	nesttrigger.setParent(nest);

	return nest;
}

void LevelManager::onCreate() {
	std::fstream file;
	file.open(RESOURCE_PATH("Holeon - test map 1.json"), std::ios::in);
	
	json j;

	file >> j;

	for (auto& layer : j["layers"]) {
		if (layer["type"] == "tilelayer") {
			auto terrain = Instattiate(createTerrain);
			auto& terrainComp = terrain.getComponent<Engine::Components::TerrainComponent>();

			// re-offset tile id's
			for (int i = 0; i < layer["data"].size(); i++) {
				layer["data"][i] = layer["data"][i].get<int>() - 1;
			}


			terrainComp.map.setMap(layer["data"].get<std::vector<int>>(), layer["width"], layer["height"]);
			terrainComp.map.walkableTiles = { 0, 9, 10, 11, 12, 13 };
			Engine::Renderer::Material terrainMat = { getSceneManager().getActveScene().m_TextureManager->getTexture(1), {0.0, 0.0, 32.0, 32.0}};
			terrainComp.material = terrainMat;
			auto& trans = terrain.getComponent<Engine::Components::TransformComponent>();
			trans.Position = { layer["x"], layer["y"] };
		}
		else if (layer["type"] == "objectgroup") {
			if (layer["name"] == "grass") {
				for (auto& ob : layer["objects"]) {
					auto ent = Instattiate(createGrass);
					auto& trans = ent.getComponent<Engine::Components::TransformComponent>();
					trans.Position = { ob["x"].get<float>(), ob["y"].get<float>() - ob["height"].get<float>() };
				}
			}
			else if (layer["name"] == "nests") {
				for (auto& ob : layer["objects"]) {
					auto ent = Instattiate(createNest);
					auto& trans = ent.getComponent<Engine::Components::TransformComponent>();
					trans.Position = { ob["x"].get<float>(), ob["y"].get<float>() - ob["height"].get<float>() };
				}
			}
		}
	}
}
