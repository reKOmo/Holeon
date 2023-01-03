#pragma once
#include "Engine.h"
#include "tweeny.h"

class SleepAnimation : public Engine::AnimationClip {
public:
	void onCreate();
	void onUpdate(float delta);
private:
	tweeny::tween<double> animator;
};