#pragma once
#include "Engine.h"
#include <stack>
#include "Directions.h"

class PlayerController : public Engine::Script {
    void onCreate();
    void onUpdate(float delta);
private:
    void updateInteractorPos();
    int currentAnimId = -1;
    raylib::Vector2 direction;
    DIRECTION facing = S;
    std::stack<KeyboardKey> keystrokes;
    KeyboardKey currentKey = KEY_NULL;
    const float speed = 80.0;
};