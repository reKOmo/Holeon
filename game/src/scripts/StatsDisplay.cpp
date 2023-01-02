#include "StatsDisplay.h"
#include "HpBarAnimation.h"

void StatsDisplay::onCreate() {
	for (auto& child : m_Owner.getChildren()) {
		auto& info = child.getComponent<Engine::Components::InfoComponent>();
		if (info.name == "name") {
			nameText = child;
		}
		else if (info.name == "level") {
			levelText = child;
		}
		else if (info.name == "hp-frame") {
			fillBar = child.getChildren()[0];
		}
	}

}

void StatsDisplay::onUpdate(float delta) {
}

void StatsDisplay::setTarget(EntityStats* t) {
	target = t;

	nameText.getComponent<Engine::Components::TextComponent>().text = target->name;
	std::string levelTxt = "Lv. ";
	levelText.getComponent<Engine::Components::TextComponent>().text = levelTxt + std::to_string(target->level);
	setHpInstant(target->health, target->maxHelath);
}

void StatsDisplay::updateHp(std::function<void()> call) {
	float p = target->health / target->maxHelath;

	auto& animMgr = fillBar.getComponent<Engine::Components::AnimationManager>();
	HpBarAnimation* a = dynamic_cast<HpBarAnimation*>(animMgr.animations[0].get());
	a->target = p;

	animMgr.playAnimation(0, std::move(call));
}

void StatsDisplay::setHpInstant(float currentHp, float maxHp) {
	float per = currentHp / maxHp;
	auto& filler = fillBar.getComponent<Engine::Components::BackgroundComponent>();
	const float defaultSize = 136.0;
	filler.size.x = defaultSize * per;
}
