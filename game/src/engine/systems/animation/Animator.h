#pragma once
#include "entt.hpp"

namespace Engine {
    namespace Systems {
        namespace Animator {
            void updateAnimations(entt::registry& scene, float delta, float unscaledDelta);
        }
    }
}