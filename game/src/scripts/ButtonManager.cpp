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
    if (currentButton) {
        auto& currComp = currentButton.getComponent<Engine::Components::ButtonComponent>();
        if (!currComp.isSelected) {
            currComp.isSelected = true;
            auto& children = currentButton.getChildren();
            children[0].getComponent<Engine::Components::TextComponent>().color = BLACK;
        }

        if (IsKeyPressed(KEY_S) && currComp.bottom) {
            switchToButton(currComp.bottom);
        }
        else if (IsKeyPressed(KEY_W) && currComp.top) {
            switchToButton(currComp.top);
        }
        else if (IsKeyPressed(KEY_A) && currComp.left) {
            switchToButton(currComp.left);
        }
        else if (IsKeyPressed(KEY_D) && currComp.right) {
            switchToButton(currComp.right);
        }

        if (IsKeyPressed(KEY_ENTER)) {
            m_onSelect(currComp.value);
        }
    }
}

void ButtonManager::onSelect(std::function<void(int)> func) {
    m_onSelect = func;

    if (currentButton != m_Owner.getComponent<Engine::Components::ButtonManagerData>().currentButton) {
        switchToButton(m_Owner.getComponent<Engine::Components::ButtonManagerData>().currentButton);
    }
}

void ButtonManager::switchToButton(Engine::Entity& next) {
    if (currentButton && currentButton.hasComponent<Engine::Components::InfoComponent>()) {
        auto& currComp = currentButton.getComponent<Engine::Components::ButtonComponent>().isSelected = false;
        auto& children = currentButton.getChildren();
        children[0].getComponent<Engine::Components::TextComponent>().color = { 100, 100, 100, 255 };
    }
    currentButton = next;
    if (next) {
        currentButton.getComponent<Engine::Components::ButtonComponent>().isSelected = true;
        auto& newChildren = currentButton.getChildren();
        newChildren[0].getComponent<Engine::Components::TextComponent>().color = BLACK;
    }
}
