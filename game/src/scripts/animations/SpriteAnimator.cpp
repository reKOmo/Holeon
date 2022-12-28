#include "SpriteAnimator.h"

SpriteAnimator::SpriteAnimator() {
    auto& sprite = m_Owner.getComponent<Engine::Components::SpriteComponent>();
    beginFrame = 0;
    endFrame = sprite.totalFrames;
}

SpriteAnimator::SpriteAnimator(int fps, int beginFrame, int endFrame, bool looping = false)
    : beginFrame(beginFrame), endFrame(endFrame)
{
    frameTime = 1.0 / (float)fps;
    if (looping) {
        loop = true;
        duration = -1;
    }
}

void SpriteAnimator::onCreate() {

}

void SpriteAnimator::onInit() {
    auto& sprite = m_Owner.getComponent<Engine::Components::SpriteComponent>();
    sprite.imageIndex = beginFrame;
}

void SpriteAnimator::onUpdate(float delta) {
    sinceLastFrame += delta;
    if (sinceLastFrame >= frameTime) {
        auto& sprite = m_Owner.getComponent<Engine::Components::SpriteComponent>();
        int newFrame = (sprite.imageIndex - beginFrame + 1) % (endFrame - beginFrame + 1);
        sprite.imageIndex = beginFrame + newFrame;
        sinceLastFrame = 0;
    }
}

void SpriteAnimator::onEnd() {
    sinceLastFrame = 0;
    auto& sprite = m_Owner.getComponent<Engine::Components::SpriteComponent>();
    sprite.imageIndex = beginFrame;
}
