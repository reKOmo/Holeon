#include "SleepAnimation.h"

void SleepAnimation::onCreate() {
	duration = 1.5;
	animator = tweeny::from(0.0).to(255.0).during(0.4 * SecToMs).to(255.0).during(0.7*SecToMs).to(0.0).during(0.4 * SecToMs);
}

void SleepAnimation::onUpdate(float delta) {
	unsigned int alpha = (int)animator.step((int)(delta * SecToMs));
	m_Owner.getComponent<Engine::Components::ImageComponent>().tint = { 0, 0, 0, (unsigned char)alpha };
}
