#pragma once
#include <functional>
#include "Script.h"
#include "Animator.h"

namespace Engine {
    namespace Components {
        struct AnimationManager;
    }
    class AnimationClip : public Script {
    public:
        virtual ~AnimationClip() {}
        /*
            Run after onCreate() and for reseting looping animations
            Shold hold initializers for tweeny animations
        */
        virtual void onInit() {}
        double duration = 0;
        friend void Engine::Systems::Animator::updateAnimations(entt::registry& scene, float delta, float);
        friend struct Engine::Components::AnimationManager;
    protected:
        bool loop = false;
        double currentPlaytime = 0;
    private:
        bool initialized = false;
        std::function<void()> onEnd;
    };
}