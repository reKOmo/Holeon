#include "raylib-cpp.hpp"
#include "entt.hpp"
#include "Engine.h"
#include <iostream>

#include "ButtonManager.h"

Engine::Entity createDialogButton(std::string text, std::string name, int value, Engine::Scene& scene, Engine::Renderer::TextureManager& textureMgr, Engine::Renderer::FontManager& fontMgr) {
    raylib::Font* f = fontMgr.getFont(0);
    const int fontSize = 24;
    const int spacing = 2;
    raylib::Vector2 textSize = f->MeasureText(text, fontSize, spacing);
    const float scale = 1.0;
    const float textOffset = 10;

    

    auto uiBg = scene.createEntity(name);
    auto& uiTrans = uiBg.addComponent<Engine::Components::TransformComponent>();
    uiTrans.Position = { 0.0, 0.0 };
    Engine::Renderer::Material dialogFrame = { textureMgr.getTexture(4), { 0.0, 0.0, 12.0, 12.0 } };
    auto& bg = uiBg.addComponent<Engine::Components::BackgroundComponent>(dialogFrame);
    bg.size = { textSize.x + textOffset * 2, textSize.y + textOffset * 2 };
    bg.scale = { scale, scale };

    auto uiText = scene.createEntity(name + "-text");
    auto& uiTextTrans = uiText.addComponent<Engine::Components::TransformComponent>();
    uiTextTrans.Position = { textOffset * scale, textOffset * scale};
    uiText.setParent(uiBg);

    auto& textComp = uiText.addComponent<Engine::Components::TextComponent>(text);
    textComp.fontSize = fontSize;
    textComp.spacing = spacing;
    textComp.font = f;
    textComp.color = { 100, 100, 100, 255 };

    uiBg.addComponent<Engine::Components::ButtonComponent>(value);

    return uiBg;
}


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

    Engine::Scene scene(&textureMgr);

    

   SetTargetFPS(60);
    while (!WindowShouldClose()) {
        scene.update();
        scene.render();
    }

    CloseWindow();
    
    return 0;
}
