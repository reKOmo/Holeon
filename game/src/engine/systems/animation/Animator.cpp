#include "Animator.h"
#include "Components.h"

namespace Engine {
    namespace Systems {
        namespace Animator {
            void updateAnimations(entt::registry& scene, float delta, float unscaledDelta) {
                scene.view<Engine::Components::AnimationManager>().each([&](entt::entity ent, Engine::Components::AnimationManager& mgr) {
                    float d = mgr.useUnscaledTime ? unscaledDelta : delta;
                    for (auto& clip : mgr.currentlyPlaying) {
                        if (!clip->m_Owner) {
                            clip->m_Owner = { ent, &scene };
                            clip->onCreate();
                        }
                        if (!clip->initialized) {
                            clip->onInit();
                            clip->initialized = true;
                        }
                        clip->onUpdate(d);
                        clip->currentPlaytime += d;
                    }
                for (int i = mgr.currentlyPlaying.size() - 1; i >= 0; i--) {
                    const auto& clipPtr = mgr.currentlyPlaying[i];
                    if (clipPtr->currentPlaytime >= clipPtr->duration && clipPtr->duration != -1) {
                        // reset control varibles
                        clipPtr->currentPlaytime = 0;
                        clipPtr->initialized = false;
                        if (clipPtr->loop) {
                            clipPtr->onInit();
                        }
                        else {
                            clipPtr->onDestroy();
                            clipPtr->onEnd();
                            mgr.currentlyPlaying.erase(mgr.currentlyPlaying.begin() + i);
                        }
                    }
                }
                    });
            }
        }
    }
}