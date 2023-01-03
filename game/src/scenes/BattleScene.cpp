#include "BattleScene.h"

#include "ButtonManager.h"
#include "BattleManager.h"
#include "EntityStats.h"
#include "GameUtils.h"
#include "HpBarAnimation.h"
#include "StatsDisplay.h"
#include "CameraController.h"
#include "BattlePlayerUI.h"

Engine::Entity ceateEntStatsDisplay(std::string name, Engine::Scene& scene) {
    //background
    auto uiBg = scene.createEntity(name);
    auto& uiTrans = uiBg.addComponent<Engine::Components::TransformComponent>();
    Engine::Renderer::Material dialogFrame = { scene.m_TextureManager->getTexture(4), { 0.0, 0.0, 12.0, 12.0 } };
    auto& bg = uiBg.addComponent<Engine::Components::BackgroundComponent>(dialogFrame);
    bg.size = { 230, 65 };
    bg.scale = { 4.0, 4.0 };

    // Name display
    auto uiText = scene.createEntity("name");
    auto& uiTextTrans = uiText.addComponent<Engine::Components::TransformComponent>();
    uiTextTrans.Position = { 15, 10 };
    uiText.setParent(uiBg);

    auto& textComp = uiText.addComponent<Engine::Components::TextComponent>("Placeholder");
    textComp.fontSize = 24;
    textComp.spacing = 2;
    textComp.font = scene.m_FontManager->getFont(0);
    textComp.color = BLACK;


    float levelY = 35.0;
    // Level display
    auto lvText = scene.createEntity("level");
    auto& uiTextTransv = lvText.addComponent<Engine::Components::TransformComponent>();
    uiTextTransv.Position = { 15.0, levelY };
    lvText.setParent(uiBg);
    auto& lvTextComp = lvText.addComponent<Engine::Components::TextComponent>("Lv.  2");
    lvTextComp.fontSize = 20;
    lvTextComp.spacing = 2;
    lvTextComp.font = scene.m_FontManager->getFont(0);
    lvTextComp.color = BLACK;

    // hp bar
    raylib::Vector2 hpBarLengthSize = { 140.0, 15.0 };
    auto uiHPFrame = scene.createEntity("hp-frame");
    auto& uiHPTrans = uiHPFrame.addComponent<Engine::Components::TransformComponent>();
    uiHPTrans.Position = { 80.0, levelY + 2 };
    Engine::Renderer::Material hpFrame = { scene.m_TextureManager->getTexture(7), { 0.0, 0.0, 12.0, 12.0 } };
    auto& hpBg = uiHPFrame.addComponent<Engine::Components::BackgroundComponent>(hpFrame);
    hpBg.zIndex = 1;
    hpBg.size = hpBarLengthSize;
    uiHPFrame.setParent(uiBg);

    auto uiHPFill = scene.createEntity("hp-bar");
    auto& uiHPFTrans = uiHPFill.addComponent<Engine::Components::TransformComponent>();
    uiHPFTrans.Position = { 2.0, 2.0 };
    Engine::Renderer::Material hpFill = { scene.m_TextureManager->getTexture(6), { 0.0, 0.0, 12.0, 12.0 } };
    auto& hpBgF = uiHPFill.addComponent<Engine::Components::BackgroundComponent>(hpFill);
    hpBgF.zIndex = 2;
    hpBgF.size = hpBarLengthSize - (raylib::Vector2(4.0, 4.0));
    uiHPFill.setParent(uiHPFrame);

    //hp bar animation
    auto& hpAnimMgr = uiHPFill.addComponent<Engine::Components::AnimationManager>();
    hpAnimMgr.animations = new std::shared_ptr<Engine::AnimationClip>[1] {
        std::make_shared<HpBarAnimation>()
    };

    //script
    uiBg.addComponent<Engine::Components::ScriptComponent>().bind<StatsDisplay>();

    return uiBg;
}

void createBattleScene(Engine::Scene& scene) {
    auto camera = scene.createEntity("camera");
    auto& camComp = camera.addComponent<raylib::Camera2D>();
    scene.setCamera(camera);
    camComp.target = { 590.0, 340.0 };
    camComp.offset = { 640, 360 };
    camComp.zoom = 0.9;
    camComp.rotation = 0.0;

    scene.setBackgroundColor({18, 25, 37, 255});


    auto battleMgr = scene.createEntity("battleManager");
    battleMgr.addComponent<Engine::Components::ScriptComponent>().bind<BattleManager>();

    auto player = scene.createEntity("player");
    // remove stats

    auto opponent = scene.createEntity("opponent");

    EntityStats* opponentStats = scene.m_GlobalStorage->emplace<EntityStats>("opponentStats");
    opponentStats->name = "Rat";
    opponentStats->level = 8 + Engine::rand() * 5.0;
    opponentStats->currentWeapon.attacks[0] = {
        "Slash",
        3.0f + 2.0f * Engine::rand(),
        Attack::DAMAGE
    };
    opponentStats->currentWeapon.attacks[1] = {
        "Boost",
        0.3,
        Attack::ATK_BOOST
    };

    raylib::Font* f = scene.m_FontManager->getFont(0);
    const int fontSize = 24;
    const int spacing = 2;


    auto uiBg = scene.createEntity("battleDialog");
    auto& uiTrans = uiBg.addComponent<Engine::Components::TransformComponent>();
    uiTrans.Position = { 850.0, 460.0 };
    Engine::Renderer::Material dialogFrame = { scene.m_TextureManager->getTexture(4), { 0.0, 0.0, 12.0, 12.0 } };
    auto& bg = uiBg.addComponent<Engine::Components::BackgroundComponent>(dialogFrame);
    bg.size = { 400, 100 };
    bg.scale = { 4.0, 4.0 };

    auto uiText = scene.createEntity("battleDialog-text");
    auto& uiTextTrans = uiText.addComponent<Engine::Components::TransformComponent>();
    uiTextTrans.Position = { 20, 20 };
    uiText.setParent(uiBg);

    auto& textComp = uiText.addComponent<Engine::Components::TextComponent>("Player turn");
    textComp.fontSize = fontSize;
    textComp.spacing = spacing;
    textComp.font = f;
    textComp.color = BLACK;


    auto statsOne = ceateEntStatsDisplay("statsDisplayTop", scene);
    auto& statsOneTrans = statsOne.getComponent<Engine::Components::TransformComponent>();
    statsOneTrans.Position = { 170.0, 200.0 };
    auto statsTwo = ceateEntStatsDisplay("statsDisplayBottom", scene);
    auto& statsTwoTrans = statsTwo.getComponent<Engine::Components::TransformComponent>();
    statsTwoTrans.Position = { 440.0, 410.0 };


    // player sprite
    auto playerSprite = scene.createEntity("playerSprite");
    auto& plSTrans = playerSprite.addComponent<Engine::Components::TransformComponent>();
    plSTrans.Position = { 60.0, 320.0 };
    Engine::Renderer::Material plSprite = { scene.m_TextureManager->getTexture(3), { 256.0, 0.0, 32.0,32.0 } };
    auto& plSSprite = playerSprite.addComponent<Engine::Components::SpriteComponent>(plSprite);
    plSSprite.scale = { 8.0, 8.0 };
    plSSprite.zIndex = 1;
    auto playerShadow = scene.createEntity();
    auto& plShadowTrans = playerShadow.addComponent<Engine::Components::TransformComponent>();
    plShadowTrans.Position = { 50.0, 185.0 };
    playerShadow.setParent(playerSprite);
    Engine::Renderer::Material shadow = { scene.m_TextureManager->getTexture(8), { 0.0, 0.0, 16.0, 6.0 } };
    auto& plSShadowprite = playerShadow.addComponent<Engine::Components::SpriteComponent>(shadow);
    plSShadowprite.scale = { 10.0, 10.0 }; 
    auto battleGroundBottom = scene.createEntity();
    auto& battleGroundBTrans = battleGroundBottom.addComponent<Engine::Components::TransformComponent>();
    battleGroundBTrans.Position = { -180.0, 75.0 };
    battleGroundBottom.setParent(playerSprite);
    Engine::Renderer::Material battleGroundMat = { scene.m_TextureManager->getTexture(12), { 0.0, 0.0, 64.0, 32.0 } };
    auto& battleGroundBSprite = battleGroundBottom.addComponent<Engine::Components::SpriteComponent>(battleGroundMat);
    battleGroundBSprite.scale = { 10.0, 10.0 };
    
    //rat sprite
    auto ratSprite = scene.createEntity("ratSprite");
    auto& ratSTrans = ratSprite.addComponent<Engine::Components::TransformComponent>();
    ratSTrans.Position = { 440.0 , 90.0 };
    Engine::Renderer::Material ratSSprite = { scene.m_TextureManager->getTexture(11), { 0.0, 0.0, 32.0,32.0 } };
    auto& ratSSSprite = ratSprite.addComponent<Engine::Components::SpriteComponent>(ratSSprite);
    ratSSSprite.scale = { 6.0, 6.0 };
    ratSSSprite.zIndex = 1;
    auto ratShadow = scene.createEntity();
    auto& ratShadowTrans = ratShadow.addComponent<Engine::Components::TransformComponent>();
    ratShadowTrans.Position = { 30.0, 165.0 };
    ratShadow.setParent(ratSprite);
    auto& ratSShadowprite = ratShadow.addComponent<Engine::Components::SpriteComponent>(shadow);
    ratSShadowprite.scale = { 8.0, 8.0 };
    auto battleGroundTop = scene.createEntity();
    auto& battleGroundTTrans = battleGroundTop.addComponent<Engine::Components::TransformComponent>();
    battleGroundTTrans.Position = { -150.0, 60.0 };
    battleGroundTop.setParent(ratSprite);
    auto& battleGroundTSprite = battleGroundTop.addComponent<Engine::Components::SpriteComponent>(battleGroundMat);
    battleGroundTSprite.scale = { 8.0, 8.0 };

    auto uiManager = scene.createEntity("uiManager");
    uiManager.addComponent<Engine::Components::ScriptComponent>().bind<BattleUIPlayer>();
    
}
