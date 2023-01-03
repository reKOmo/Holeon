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
        RESOURCE_PATH("shadow.png"),
        RESOURCE_PATH("rat-nest.png"),
        RESOURCE_PATH("battle-transition.png"),
        RESOURCE_PATH("rat.png"),
        RESOURCE_PATH("battle-ground.png"),
        RESOURCE_PATH("fireplace.png"),
        RESOURCE_PATH("white-screen.png"),
        RESOURCE_PATH("chest.png"),
    };

    char* fonts[] = {
        RESOURCE_PATH("font/pixeleum - 48.ttf")
    };

    Engine::Renderer::TextureManager textureMgr;
    textureMgr.loadFiles(files, 16);

    Engine::Renderer::FontManager fontMgr;
    fontMgr.loadFiles(fonts, 1);

    Engine::SceneManager sceneMgr(&textureMgr, &fontMgr);

    sceneMgr.addScene(createWorldScene);
    sceneMgr.addScene(createBattleScene);

    sceneMgr.loadScene(0);

    const int targetFps = 120;

    SetTargetFPS(targetFps);
    while (!WindowShouldClose()) {
        sceneMgr.tick();
        //printf("Frametime: %f, Target: %f\n", GetFrameTime(), 1.0 / (float)targetFps);
    }

    CloseWindow();
    
    return 0;
}
