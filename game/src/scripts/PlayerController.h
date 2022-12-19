#pragma once
#include "Engine.h"

class PlayerController : public Engine::Script {
    void onCreate() {
        auto& anim = m_Owner.getComponent<Engine::Components::AnimationManager>();
        currentAnimId = anim.playAnimation(0, []() {
            printf("Stopped animation");
            });
    }
    void onUpdate(float delta) {
        auto& trans = m_Owner.getComponent<Engine::Components::TransformComponent>();
        const float speed = 200.0 * delta;
        if (IsKeyDown(KEY_A)) trans.Position.x -= speed;
        if (IsKeyDown(KEY_D)) trans.Position.x += speed;
        if (IsKeyDown(KEY_W)) trans.Position.y -= speed;
        if (IsKeyDown(KEY_S)) trans.Position.y += speed;
        if (IsKeyDown(KEY_SPACE) && currentAnimId != -1) {
            printf("%d\n", currentAnimId);
            auto& anim = m_Owner.getComponent<Engine::Components::AnimationManager>();
            anim.stopAnimation(currentAnimId);
            currentAnimId = -1;
        }
    }
    void onDestroy() {

    }
private:
    int currentAnimId = -1;
};