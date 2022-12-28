#include "raylib-cpp.hpp"
#include "Engine.h"

#include "WorldScene.h"
#include "BattleScene.h"

int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    Engine::initHelpers();
    InitWindow(screenWidth, screenHeight, "Holeon"); 

    char* files[] = {
        RESOURCE_PATH("basic_sword.png"),
        RESOURCE_PATH("Holeon-tileset.png"),
        RESOURCE_PATH("Grass.png"),
        RESOURCE_PATH("player.png"),
        RESOURCE_PATH("dialog-frame.png"),
        RESOURCE_PATH("select-arrow.png"),
        RESOURCE_PATH("hp-fill.png"),
        RESOURCE_PATH("hp-frame.png"),
    };

    char* fonts[] = {
        RESOURCE_PATH("font/pixeleum - 48.ttf")
    };

    Engine::Renderer::TextureManager textureMgr;
    textureMgr.loadFiles(files, 7);

    Engine::Renderer::FontManager fontMgr;
    fontMgr.loadFiles(fonts, 1);

    Engine::Scene ballteScene(&textureMgr, &fontMgr);
    Engine::Scene worldScene(&textureMgr, &fontMgr);

    createWorldScene(worldScene);

   SetTargetFPS(60);
    while (!WindowShouldClose()) {
        worldScene.update();
        worldScene.render();
    }

    CloseWindow();
    
    return 0;
}
