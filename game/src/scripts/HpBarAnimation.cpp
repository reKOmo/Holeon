#include "HpBarAnimation.h"
#include "Engine.h"

void HpBarAnimation::onCreate() {
	duration = 1.0;
}

void HpBarAnimation::onInit() {
	tween = tweeny::from(currentBase).to(target);//.via(tweeny::easing::quinticOut);
}



void HpBarAnimation::onUpdate(float delta) {
	float progress = delta / duration;
	printf("%f\n", progress);
	float p = tween.step(progress);
	auto& bg = m_Owner.getComponent<Engine::Components::BackgroundComponent>();
	bg.size.x = baseWidth * p;
}

void HpBarAnimation::onDestroy() {
	currentBase = target;
}
