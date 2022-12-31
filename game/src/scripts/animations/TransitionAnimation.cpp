#include "TransitionAnimation.h"

void TransitionAnimation::onCreate() {
	duration = 2.0;
	animator = tweeny::from(0.0, 0.0).to(4.0, 0.0).during(0.4 * SecToMs).via(tweeny::easing::elasticOut).to(4.0, 0.0).during(0.4 * SecToMs).to(4.0, 100.0).during(1.0 * SecToMs);
	background = m_Owner.getChildren()[0];
}

void TransitionAnimation::onInit() {
}

void TransitionAnimation::onUpdate(float delta) {
	auto& [a, b] = animator.step((int)(delta * SecToMs));
	auto& sprite = m_Owner.getComponent<Engine::Components::ImageComponent>();
	sprite.scale = { (float)a, (float)a };
	auto& back = background.getComponent<Engine::Components::ImageComponent>();
	back.scale = { (float)b, (float)b };
}
