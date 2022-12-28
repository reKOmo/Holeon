#pragma once
#include "Engine.h"

class SpriteAnimator : public Engine::AnimationClip {
public:
    SpriteAnimator();
    SpriteAnimator(int fps, int beginFrame, int endFrame, bool looping);
    void onCreate();
    void onInit();
    void onUpdate(float delta);
    void onEnd();
private:
    float sinceLastFrame = 0;
    float frameTime = 0.081;
    int beginFrame;
    int endFrame;
};