#include "Animator.h"
#include "Components.h"

namespace Engine {
    namespace Animator {
        void updateAnimations(entt::registry& scene, float delta) {
            scene.view<Engine::Components::AnimationManager>().each([delta](entt::entity ent, Engine::Components::AnimationManager mgr) {
                for (const auto& clip : mgr.currentlyPlaying) {
                    if (!clip->initialized) {
                        clip->onCreate();
                        clip->onInit();
                        clip->initialized = true;
                    }
                    clip->onUpdate(delta);
                }
                for (int i = mgr.currentlyPlaying.size(); i >= 0; i--) {
                    const auto& clipPtr = mgr.currentlyPlaying[i];
                    if (clipPtr->currentPlaytime >= clipPtr->duration) {
                        if (clipPtr->loop) {
                            clipPtr->onInit();
                            clipPtr->currentPlaytime = 0;
                        } else {
                            clipPtr->onDestroy();
                            mgr.currentlyPlaying.erase(mgr.currentlyPlaying.begin() + i);
                        }
                    }    
                } 
            });
        }
    }
}