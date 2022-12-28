#include "WorldScene.h"
#include "CameraController.h"
#include "PlayerController.h"
#include "SpriteAnimator.h"
#include <memory>

void createWorldScene(Engine::Scene& scene) {
    scene.setBackgroundColor({3, 3, 5, 255});

    auto camera = scene.createEntity("camera");
    auto& camComp = camera.addComponent<raylib::Camera2D>();
    camComp.target = { 0.0, 0.0 };
    camComp.offset = { 640, 360};
    camComp.zoom = 4.0;
    camComp.rotation = 0.0;
    camera.addComponent<Engine::Components::ScriptComponent>().bind<CameraController>();
    scene.setCamera(camera);


    
    auto terrain = scene.createEntity();
    Engine::TerrainMap map;
    map.loadMap(RESOURCE_PATH("Holeon - test map 1.csv"));
    Engine::Renderer::Material terrainMat = { scene.m_TextureManager->getTexture(1), { 0.0, 0.0, 32.0, 32.0 } };
    terrain.addComponent<Engine::Components::TerrainComponent>(terrainMat, map);
    auto& terrainTrans = terrain.addComponent<Engine::Components::TransformComponent>();
    terrainTrans.Position = { 0.0, 0.0 };
    

    auto player = scene.createEntity("player");
    auto& playerTransform = player.addComponent<Engine::Components::TransformComponent>();
    playerTransform.Position = { 0.0, 0.0 };
    Engine::Renderer::Material playerSpriteMat = { scene.m_TextureManager->getTexture(3), { 0.0, 0.0, 32.0, 32.0 } };
    auto& playerSprite = player.addComponent<Engine::Components::SpriteComponent>(playerSpriteMat);
    playerSprite.scale = 1.0;
    player.addComponent<Engine::Components::ScriptComponent>().bind<PlayerController>();
    auto& playerAnimator = player.addComponent<Engine::Components::AnimationManager>();
    playerAnimator.animations = new std::shared_ptr<Engine::AnimationClip>[4] {
        std::make_shared<SpriteAnimator>(10, 0, 3, true),
        std::make_shared<SpriteAnimator>(10, 4, 7, true),
        std::make_shared<SpriteAnimator>(10, 8, 11, true),
        std::make_shared<SpriteAnimator>(10, 12, 15, true)
    };
    playerAnimator.animationsSize = 4;
    auto& colider = player.addComponent<Engine::Components::ColiderComponent>();
    colider.plot = {0.0, 0.0, 20.0, 20.0};
    colider.trigger = true;

    
}
