#include "raylib.h"
#include "entt.hpp"
#include "Engine.h"

int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Holeon");

    char* files[] = {
        RESOURCE_PATH("basic_sword.png"),
    };

    entt::registry scene;

    Engine::Renderer::TextureManager textureMgr;
    textureMgr.loadFiles(files, 1);

    Engine::Renderer::Renderer2D renderer(&scene, textureMgr);

    Engine::Renderer::Material swordMaterial(0, {0.0, 0.0, 32.0, 32.0});

    entt::entity sword = scene.create();
    scene.emplace<Engine::Components::SpriteComponent>(sword, swordMaterial);
    auto& swordTrans = scene.emplace<Engine::Components::TransformComponent>(sword);
    swordTrans.Position = { 640.0, 360.0 };

    

    SetTargetFPS(60);
    while (!WindowShouldClose()) {

        swordTrans.Rotation++;

        renderer.render();
    }

    CloseWindow();
    return 0;
}
