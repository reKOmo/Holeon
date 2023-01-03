#include "ButtonManager.h"

void ButtonManager::onCreate() {
    currentButton = m_Owner.getComponent<Engine::Components::ButtonManagerData>().currentButton;
    if (currentButton) {
        auto& currComp = currentButton.getComponent<Engine::Components::ButtonComponent>();
        currComp.isSelected = true;
        auto& children = currentButton.getChildren();
        children[0].getComponent<Engine::Components::TextComponent>().color = BLACK;
    }
}

void ButtonManager::onUpdate(float delta) {
    if (currentButton != m_Owner.getComponent<Engine::Components::ButtonManagerData>().currentButton) {
        currentButton = m_Owner.getComponent<Engine::Components::ButtonManagerData>().currentButton;
        auto& currComp = currentButton.getComponent<Engine::Components::ButtonComponent>();
        currComp.isSelected = true;
        auto& children = currentButton.getChildren();
        children[0].getComponent<Engine::Components::TextComponent>().color = BLACK;
    }

    if (currentButton) {
        auto& currComp = currentButton.getComponent<Engine::Components::ButtonComponent>();
        if (!currComp.isSelected) {
            currComp.isSelected = true;
            auto& children = currentButton.getChildren();
            children[0].getComponent<Engine::Components::TextComponent>().color = BLACK;
        }

        if (IsKeyPressed(KEY_S)) {
            switchToButton(currComp.bottom);
        }
        else if (IsKeyPressed(KEY_W)) {
            switchToButton(currComp.top);
        }
        else if (IsKeyPressed(KEY_A)) {
            switchToButton(currComp.left);
        }
        else if (IsKeyPressed(KEY_D)) {
            switchToButton(currComp.right);
        }

        if (IsKeyPressed(KEY_ENTER)) {
            m_onSelect(currComp.value);
        }
    }
}

void ButtonManager::onSelect(std::function<void(int)> func) {
    m_onSelect = func;
}

void ButtonManager::switchToButton(Engine::Entity& next) {
    if (next) {
        auto& currComp = currentButton.getComponent<Engine::Components::ButtonComponent>().isSelected = false;
        auto& children = currentButton.getChildren();
        children[0].getComponent<Engine::Components::TextComponent>().color = { 100, 100, 100, 255 };

        currentButton = next;
        currentButton.getComponent<Engine::Components::ButtonComponent>().isSelected = true;
        auto& newChildren = currentButton.getChildren();
        newChildren[0].getComponent<Engine::Components::TextComponent>().color = BLACK;
    }
}
