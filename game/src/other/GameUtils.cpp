#include "GameUtils.h"

Engine::Entity createDialogButton(std::string text, std::string name, int value, Engine::Scene* scene) {
    raylib::Font* f = scene->m_FontManager->getFont(0);
    const int fontSize = 24;
    const int spacing = 2;
    raylib::Vector2 textSize = f->MeasureText(text, fontSize, spacing);
    const float scale = 1.0;
    const float textOffset = 20;



    auto uiBg = scene->createEntity(name);
    auto& uiTrans = uiBg.addComponent<Engine::Components::TransformComponent>();
    uiTrans.Position = { 0.0, 0.0 };
    Engine::Renderer::Material dialogFrame = { scene->m_TextureManager->getTexture(4), { 0.0, 0.0, 12.0, 12.0 } };
    auto& bg = uiBg.addComponent<Engine::Components::BackgroundComponent>(dialogFrame);
    bg.size = { textSize.x + textOffset * 2, textSize.y + textOffset * 2 };
    bg.scale = { scale, scale };

    auto uiText = scene->createEntity(name + "-text");
    auto& uiTextTrans = uiText.addComponent<Engine::Components::TransformComponent>();
    uiTextTrans.Position = { textOffset * scale, textOffset * scale };
    uiText.setParent(uiBg);

    auto& textComp = uiText.addComponent<Engine::Components::TextComponent>(text);
    textComp.fontSize = fontSize;
    textComp.spacing = spacing;
    textComp.font = f;
    textComp.color = { 100, 100, 100, 255 };

    uiBg.addComponent<Engine::Components::ButtonComponent>(value);

    return uiBg;
}
