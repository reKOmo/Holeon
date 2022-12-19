#pragma once
#include "Engine.h"

class SpriteAnimator : public Engine::AnimationClip {
public:
    SpriteAnimator() {}
    SpriteAnimator(int fps) {
        frameTime = 1.0 / (float)fps;
    }
    void onCreate() {
        loop = true;
        duration = -1;
    }
    void onUpdate(float delta) {
        sinceLastFrame += delta;
        if (sinceLastFrame >= frameTime) {
            auto& sprite = m_Owner.getComponent<Engine::Components::SpriteComponent>();
            sprite.imageIndex = (sprite.imageIndex + 1) % sprite.totalFrames;
            sinceLastFrame = 0;
        }
    }
private:
    float sinceLastFrame = 0;
    float frameTime = 0.081;
};