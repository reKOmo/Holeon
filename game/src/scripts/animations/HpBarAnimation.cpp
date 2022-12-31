#include "HpBarAnimation.h"
#include "Engine.h"

constexpr auto SCALE = 100.0;

void HpBarAnimation::onCreate() {
	duration = 0.5;
}

void HpBarAnimation::onInit() {
	tween = tweeny::from(currentBase * SCALE).to(target * SCALE).during(duration * SecToMs).via(tweeny::easing::elasticOut);
}

void HpBarAnimation::onUpdate(float delta) {
	float p = tween.step((int)(delta * SecToMs)) / SCALE;
	auto& bg = m_Owner.getComponent<Engine::Components::BackgroundComponent>();
	bg.size.x = baseWidth * p;
}

void HpBarAnimation::onDestroy() {
	currentBase = target;
}
