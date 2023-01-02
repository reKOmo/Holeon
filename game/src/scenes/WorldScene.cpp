#include "WorldScene.h"
#include "CameraController.h"
#include "PlayerController.h"
#include "SpriteAnimator.h"
#include "LevelManager.h"
#include "SceneSwitcher.h"
#include "EntityStats.h"
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
    

    auto player = scene.createEntity("player");
    auto& info = player.getComponent<Engine::Components::InfoComponent>();
    info.tags.push_back("pauseable");
    auto& playerTransform = player.addComponent<Engine::Components::TransformComponent>();
    playerTransform.Position = { 608.0, 288.0 };
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
    colider.plot = {9.0, 18.0, 14.0, 10.0};
    auto& rigidbody = player.addComponent<Engine::Components::RigidbodyComponent>();

    // player stats
    if (!scene.m_GlobalStorage->has("playerStats")) {
        EntityStats* playerStats = scene.m_GlobalStorage->emplace<EntityStats>("playerStats");
        playerStats->name = "Player";
        playerStats->xp = 330;
        playerStats->currentWeapon.attacks[0] = {
            "Slash",
            5.0,
            Attack::DAMAGE
        };
        playerStats->currentWeapon.attacks[1] = {
            "Boost",
            0.2,
            Attack::ATK_BOOST
        };
    }





    auto shadowEnt = scene.createEntity("shadow");
    auto& shadowTrans = shadowEnt.addComponent<Engine::Components::TransformComponent>();
    shadowTrans.Position = { 8.0, 24.0 };
    Engine::Renderer::Material shadowMat = { scene.m_TextureManager->getTexture(8), { 0.0, 0.0, 16.0, 6.0 } };
    auto& shadowSprite = shadowEnt.addComponent<Engine::Components::SpriteComponent>(shadowMat);
    shadowEnt.setParent(player);

    auto levelMgr = scene.createEntity("levelMgr");
    levelMgr.addComponent<Engine::Components::ScriptComponent>().bind<LevelManager>();

    auto transition = scene.createEntity("battle-transition");
    Engine::Renderer::Material transitionMat = { scene.m_TextureManager->getTexture(10), { 0.0, 0.0, 32.0, 32.0 } };
    auto& transitionMatComp = transition.addComponent<Engine::Components::ImageComponent>(transitionMat);
    transitionMatComp.zIndex = 2;
    transitionMatComp.imageIndex = 0;
    transitionMatComp.origin = { 16, 16 };
    transitionMatComp.scale = { 0.0, 0.0 };
    auto& transitionTrans = transition.addComponent<Engine::Components::TransformComponent>();
    transitionTrans.Position = { 640, 360 };
    transition.addComponent<Engine::Components::ScriptComponent>().bind<SceneSwitcher>();
    auto& transitionAnimMgr = transition.addComponent<Engine::Components::AnimationManager>();
    transitionAnimMgr.useUnscaledTime = true;

    auto transitionBG = scene.createEntity("battle-transitionBG");
    Engine::Renderer::Material transitionMatBG = { scene.m_TextureManager->getTexture(10), { 0.0, 0.0, 32.0, 32.0 } };
    auto& transitionMatCompBG = transitionBG.addComponent<Engine::Components::ImageComponent>(transitionMat);
    transitionMatCompBG.zIndex = 1;
    transitionMatCompBG.imageIndex = 1;
    transitionMatCompBG.origin = { 16, 16 };
    transitionMatCompBG.scale = { 0.0, 0.0 };
    auto& transitionTransBG = transitionBG.addComponent<Engine::Components::TransformComponent>();
    transitionBG.setParent(transition);

}
