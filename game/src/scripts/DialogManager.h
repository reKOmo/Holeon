#pragma once
#include "Engine.h"

class DialogManager : public Engine::Script {
public:
	void onCreate();
	void onUpdate(float delta);
	void showDialog(std::string msg, std::function<void()> onEnd = []() {});
	void showDialog(std::vector<std::string>& msg, std::function<void()> onEnd = []() {});
	void pickDialog(std::string question, std::string yes, std::string no, std::function<void(bool)> onEnd = [](bool a) {});
private:
	void pauseGame();
	void unpauseGame();
	void resetCallbacks();
	bool isDisplaying = false;
	std::function<void()> onEnd = []() {};
	std::function<void(bool)> onEndQuestion = [](bool a) {};
	Engine::Entity dialogContainer;
	bool endInteraction = false;
	Engine::Entity optionsEnt;
	bool optionsSelectOn = false;
	bool pickedValue;
};