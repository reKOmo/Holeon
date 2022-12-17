#include "Animator.h"
#include "Components.h"

namespace Engine {
    namespace Systems {
        namespace Animator {
            void updateAnimations(entt::registry& scene, float delta) {
                scene.view<Engine::Components::AnimationManager>().each([&](entt::entity ent, Engine::Components::AnimationManager& mgr) {
                    for (auto& clip : mgr.currentlyPlaying) {
                        if (!clip->initialized) {
                            clip->m_Owner = { ent, &scene };
                            clip->onCreate();
                            clip->onInit();
                            clip->initialized = true;
                        }
                        clip->onUpdate(delta);
                    }
                for (int i = mgr.currentlyPlaying.size() - 1; i >= 0; i--) {
                    const auto& clipPtr = mgr.currentlyPlaying[i];
                    if (clipPtr->currentPlaytime >= clipPtr->duration && clipPtr->duration != -1) {
                        if (clipPtr->loop) {
                            clipPtr->onInit();
                            clipPtr->currentPlaytime = 0;
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