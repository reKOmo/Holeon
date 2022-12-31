#pragma once
#include "Engine.h"
#include "tweeny.h"

class TransitionAnimation : public Engine::AnimationClip {
public:
	void onCreate();
	void onInit();
	void onUpdate(float delta);
private:
	Engine::Entity background;
	tweeny::tween<double, double> animator;
};