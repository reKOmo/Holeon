#include "WorldScene.h"
#include "CameraController.h"
#include "PlayerController.h"
#include "SpriteAnimator.h"
#include <memory>

void createWorldScene(Engine::Scene& scene) {
    scene.setBackgroundColor({3, 3, 5, 255});

    printf("Scene add before: %p\n", &scene);

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
    map.walkableTiles = { 0, 9, 10, 11, 12, 13 };
    Engine::Renderer::Material terrainMat = { scene.m_TextureManager->getTexture(1), { 0.0, 0.0, 32.0, 32.0 } };
    terrain.addComponent<Engine::Components::TerrainComponent>(terrainMat, map);
    auto& terrainTrans = terrain.addComponent<Engine::Components::TransformComponent>();
    terrainTrans.Position = { 0.0, 0.0 };
    

    auto player = scene.createEntity("player");
    auto& playerTransform = player.addComponent<Engine::Components::TransformComponent>();
    playerTransform.Position = { 608.0, 288.0 };
    Engine::Renderer::Material playerSpriteMat = { scene.m_TextureManager->getTexture(3), { 0.0, 0.0, 32.0, 32.0 } };
    auto& playerSprite = player.addComponent<Engine::Components::SpriteComponent>(playerSpriteMat);
    playerSprite.scale = 1.0;
    playerSprite.zIndex = 1;
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
    colider.plot = {9.0, 18.0, 14.0, 10.0};
    colider.trigger = true;
    auto& rigidbody = player.addComponent<Engine::Components::RigidbodyComponent>();

    auto shadowEnt = scene.createEntity("shadow");
    auto& shadowTrans = shadowEnt.addComponent<Engine::Components::TransformComponent>();
    shadowTrans.Position = { 8.0, 24.0 };
    Engine::Renderer::Material shadowMat = { scene.m_TextureManager->getTexture(8), { 0.0, 0.0, 16.0, 6.0 } };
    auto& shadowSprite = shadowEnt.addComponent<Engine::Components::SpriteComponent>(shadowMat);
    shadowEnt.setParent(player);
    

    auto testColider = scene.createEntity("testColider");
    auto& testColTrans = testColider.addComponent<Engine::Components::TransformComponent>();
    testColTrans.Position = { 330.0, 288.0 };
    auto& coliderTest = testColider.addComponent<Engine::Components::ColiderComponent>();
    coliderTest.plot = { 9.0, 18.0, 14.0, 10.0 };
    //coliderTest.trigger = true;
    testColider.addComponent<Engine::Components::RigidbodyComponent>();

    class Tp : public Engine::Script {
    public:
        void onTrigger(Engine::Entity colided) {
            getSceneManager().loadScene(1);
        }
    };

    auto testColiderTp = scene.createEntity("testColider");
    auto& testColTransTp = testColiderTp.addComponent<Engine::Components::TransformComponent>();
    testColTransTp.Position = { 330.0, 400.0 };
    auto& coliderTestTp = testColiderTp.addComponent<Engine::Components::ColiderComponent>();
    coliderTestTp.plot = { 9.0, 18.0, 14.0, 10.0 };
    coliderTestTp.trigger = true;
    testColiderTp.addComponent<Engine::Components::ScriptComponent>().bind<Tp>();
}
