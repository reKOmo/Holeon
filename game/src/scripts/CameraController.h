#pragma once
#include "Engine.h"

class CameraController : public Engine::Script {
public:
    void onCreate();
    void onUpdate(float delta);
private:
    Engine::Entity player;
};