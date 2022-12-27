#include "HpBarAnimation.h"
#include "Engine.h"

constexpr float SCALE = 100.0;

void HpBarAnimation::onCreate() {
	duration = 0.6;
}

void HpBarAnimation::onInit() {
	tween = tweeny::from(currentBase * SCALE).to(target * SCALE).during(duration * SCALE).via(tweeny::easing::circularOut);
}



void HpBarAnimation::onUpdate(float delta) {
	float progress = (currentPlaytime + delta) / duration;
	float p = tween.step(progress) / SCALE;
	auto& bg = m_Owner.getComponent<Engine::Components::BackgroundComponent>();
	bg.size.x = baseWidth * p;
}

void HpBarAnimation::onDestroy() {
	currentBase = target;
}
