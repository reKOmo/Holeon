#pragma once
#include "Engine.h"
#include "tweeny.h"

class HpBarAnimation : public Engine::AnimationClip {
public:
	void onCreate();
	void onInit();
	void onUpdate(float delta);
	void onDestroy();
	float target = 1.0;
private:
	const float baseWidth = 136.0;
	float currentBase = 1.0;
	tweeny::tween<double> tween;

};