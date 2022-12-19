#pragma once
#include "Engine.h"

class ButtonManager : public Engine::Script {
public:
    Engine::Entity currentButton;
    void onCreate();
    void onUpdate(float);

    void onSelect(std::function<void(int)>);
private:
    void switchToButton(Engine::Entity& next);
    std::function<void(int)> m_onSelect = [](int a) { return; };
};