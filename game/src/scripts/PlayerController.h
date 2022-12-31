#pragma once
#include "Engine.h"
#include <stack>

class PlayerController : public Engine::Script {
    void onCreate();
    void onUpdate(float delta);
    void onTriggerEnter(Engine::Entity e) {
        auto& info = e.getComponent<Engine::Components::InfoComponent>();
        printf("Entered collided with %s\n", info.name.c_str());
    }
    
    void onTriggerExit(Engine::Entity e) {
        auto& info = e.getComponent<Engine::Components::InfoComponent>();
        printf("Exited collided with %s\n", info.name.c_str());
    }
private:
    int currentAnimId = -1;
    raylib::Vector2 direction;
    std::stack<KeyboardKey> keystrokes;
    KeyboardKey currentKey = KEY_NULL;
    const float speed = 80.0;
};