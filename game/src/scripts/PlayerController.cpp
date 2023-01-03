#include "PlayerController.h"

void PlayerController::onCreate() {
}

void PlayerController::onUpdate(float delta) {
    //check key pressed
    if (IsKeyPressed(KEY_A)) keystrokes.push(KEY_A);
    if (IsKeyPressed(KEY_D)) keystrokes.push(KEY_D);
    if (IsKeyPressed(KEY_W)) keystrokes.push(KEY_W);
    if (IsKeyPressed(KEY_S)) keystrokes.push(KEY_S);

    //update direction
    // remove unpressed keys
    while (keystrokes.size() > 0 && !IsKeyDown(keystrokes.top())) {
        keystrokes.pop();
    }
    //fix too big stack
    if (keystrokes.size() > 5) {
        KeyboardKey tmp = keystrokes.top();
        while (keystrokes.size() > 0) {
            keystrokes.pop();
        }
        if (IsKeyDown(KEY_A) && tmp != KEY_A) keystrokes.push(KEY_A);
        if (IsKeyDown(KEY_D) && tmp != KEY_D) keystrokes.push(KEY_D);
        if (IsKeyDown(KEY_W) && tmp != KEY_W) keystrokes.push(KEY_W);
        if (IsKeyDown(KEY_S) && tmp != KEY_S) keystrokes.push(KEY_S);
        keystrokes.push(tmp);
    }

    if (keystrokes.size() > 0) {
        if (currentKey != keystrokes.top()) {
            currentKey = keystrokes.top();
            auto& animManager = m_Owner.getComponent<Engine::Components::AnimationManager>();
            if (currentAnimId != -1) {
                animManager.stopAnimation(currentAnimId);
                currentAnimId = -1;
            }
            switch (keystrokes.top()) {
            case KEY_A:
                direction = { -speed, 0.0 };
                facing = W;
                currentAnimId = animManager.playAnimation(1);
                break;
            case KEY_D:
                direction = { speed, 0.0 };
                facing = E;
                currentAnimId = animManager.playAnimation(3);
                break;
            case KEY_W:
                direction = { 0.0, -speed };
                facing = N;
                currentAnimId = animManager.playAnimation(2);
                break;
            case KEY_S:
                direction = { 0.0, speed };
                facing = S;
                currentAnimId = animManager.playAnimation(0);
                break;
            }
        }
    }
    else {
        auto& animManager = m_Owner.getComponent<Engine::Components::AnimationManager>();
        if (currentAnimId != -1) {
            animManager.stopAnimation(currentAnimId);
            currentAnimId = -1;
        }
        direction = {0.0, 0.0};
        currentKey = KEY_NULL;
    }

    auto& rigid = m_Owner.getComponent<Engine::Components::RigidbodyComponent>();
    rigid.velocity = direction;

    updateInteractorPos();
}

void PlayerController::updateInteractorPos() {
    auto ent = getEntityByName("playerInteractor");
    auto& trans = ent.getComponent<Engine::Components::ColiderComponent>();
    switch (facing) {
    case N:
        trans.plot = { 11.0, 6.0, 10.0, 12.0 };
        break;
    case E:
        trans.plot = { 23.0, 18.0, 10.0, 10.0 };
        break;
    case S:
        trans.plot = { 11.0, 30.0, 10.0, 10.0 };
        break;
    case W:
        trans.plot = { -1.0, 18.0, 10.0, 10.0 };
        break;
    }
}
