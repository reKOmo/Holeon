#pragma once
#include "Script.h"

namespace Engine {
    class AnimationClip : public Script {
    public:
        /*
            Run after onCreate() and for reseting looping animations
            Shold hold initializers for tweeny animations
        */
        virtual void onInit() {}
        double duration = 0;
    private:
        bool initialized = false;
        bool loop = false;
        double currentPlaytime = 0;
        friend void Engine::Animator::updateAnimations(entt::registry& scene, float delta);

    }
}