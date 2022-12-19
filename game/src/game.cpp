#include "raylib-cpp.hpp"
#include "entt.hpp"
#include "Engine.h"
#include <iostream>

#include "ButtonManager.h"
#include "BattleManager.h"
#include "EntityStats.h"



int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;

   InitWindow(screenWidth, screenHeight, "Holeon");

    char* files[] = {
        RESOURCE_PATH("basic_sword.png"),
        RESOURCE_PATH("Holeon-tileset.png"),
        RESOURCE_PATH("Grass.png"),
        RESOURCE_PATH("player_forward.png"),
        RESOURCE_PATH("dialog-frame.png"),
        RESOURCE_PATH("select-arrow.png")
    };

    char* fonts[] = {
        RESOURCE_PATH("font/pixeleum - 48.ttf")
    };

    Engine::Renderer::TextureManager textureMgr;
    textureMgr.loadFiles(files, 5);

    Engine::Renderer::FontManager fontMgr;
    fontMgr.loadFiles(fonts, 1);

    Engine::Scene scene(&textureMgr, &fontMgr);
   
    auto battleMgr = scene.createEntity("battleManager");
    battleMgr.addComponent<Engine::Components::ScriptComponent>().bind<BattleManager>();

    auto player = scene.createEntity("player");
    auto& playerStats = player.addComponent<EntityStats>();
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

   SetTargetFPS(60);
    while (!WindowShouldClose()) {
        scene.update();
        scene.render();
    }

    CloseWindow();
    
    return 0;
}
