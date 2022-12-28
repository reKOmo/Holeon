#include "BattleScene.h"

#include "ButtonManager.h"
#include "BattleManager.h"
#include "EntityStats.h"
#include "GameUtils.h"
#include "HpBarAnimation.h"
#include "StatsDisplay.h"

Engine::Entity ceateEntStatsDisplay(std::string name, Engine::Scene& scene) {
    //background
    auto uiBg = scene.createEntity(name);
    auto& uiTrans = uiBg.addComponent<Engine::Components::TransformComponent>();
    uiTrans.Position = { 600.0, 300.0 };
    Engine::Renderer::Material dialogFrame = { scene.m_TextureManager->getTexture(4), { 0.0, 0.0, 12.0, 12.0 } };
    auto& bg = uiBg.addComponent<Engine::Components::BackgroundComponent>(dialogFrame);
    bg.size = { 230, 65 };

    // Name display
    auto uiText = scene.createEntity("name");
    auto& uiTextTrans = uiText.addComponent<Engine::Components::TransformComponent>();
    uiTextTrans.Position = { 10, 10 };
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
    uiTextTransv.Position = { 10.0, levelY };
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
    uiHPTrans.Position = { 75.0, levelY + 2 };
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
    auto battleMgr = scene.createEntity("battleManager");
    battleMgr.addComponent<Engine::Components::ScriptComponent>().bind<BattleManager>();

    auto player = scene.createEntity("player");
    auto& playerStats = player.addComponent<EntityStats>();
    playerStats.name = "Player";
    playerStats.currentWeapon.attacks[0] = {
        "Slash",
        2.0,
        Attack::DAMAGE
    };
    playerStats.currentWeapon.attacks[1] = {
        "Boost",
        0.2,
        Attack::ATK_BOOST
    };

    auto opponent = scene.createEntity("opponent");
    auto& opponentStats = opponent.addComponent<EntityStats>();
    opponentStats.name = "Rat";
    opponentStats.currentWeapon.attacks[0] = {
        "Slash",
        2.0,
        Attack::DAMAGE
    };
    opponentStats.currentWeapon.attacks[1] = {
        "Boost",
        0.2,
        Attack::ATK_BOOST
    };

    raylib::Font* f = scene.m_FontManager->getFont(0);
    const int fontSize = 24;
    const int spacing = 2;


    auto uiBg = scene.createEntity("battleDialog");
    auto& uiTrans = uiBg.addComponent<Engine::Components::TransformComponent>();
    uiTrans.Position = { 200.0, 100.0 };
    Engine::Renderer::Material dialogFrame = { scene.m_TextureManager->getTexture(4), { 0.0, 0.0, 12.0, 12.0 } };
    auto& bg = uiBg.addComponent<Engine::Components::BackgroundComponent>(dialogFrame);
    bg.size = { 400, 200 };

    auto uiText = scene.createEntity("battleDialog-text");
    auto& uiTextTrans = uiText.addComponent<Engine::Components::TransformComponent>();
    uiTextTrans.Position = { 10, 10 };
    uiText.setParent(uiBg);

    auto& textComp = uiText.addComponent<Engine::Components::TextComponent>("Player turn");
    textComp.fontSize = fontSize;
    textComp.spacing = spacing;
    textComp.font = f;
    textComp.color = BLACK;


    auto statsOne = ceateEntStatsDisplay("statsDisplayTop", scene);
    auto statsTwo = ceateEntStatsDisplay("statsDisplayBottom", scene);
    auto& t = statsTwo.getComponent<Engine::Components::TransformComponent>();
    t.Position.y += 80;
}
