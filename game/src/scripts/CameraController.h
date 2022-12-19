#pragma once
#include "Engine.h"

class CameraController : public Engine::Script {
public:
    void onCreate() {
    }
    void onUpdate(float delta) {
        auto& m_camComp = m_Owner.getComponent<raylib::Camera2D>();
        // Camera rotation controls
        //const float speed = 200.0 * delta;
        //if (IsKeyDown(KEY_A)) m_camComp.target.x -= speed;
        //if (IsKeyDown(KEY_D)) m_camComp.target.x += speed;
        //if (IsKeyDown(KEY_W)) m_camComp.target.y -= speed;
        //if (IsKeyDown(KEY_S)) m_camComp.target.y += speed;

        // Camera zoom controls
        m_camComp.zoom += ((float)GetMouseWheelMove() * 0.05f);

        if (m_camComp.zoom > 3.0f) m_camComp.zoom = 3.0f;
        else if (m_camComp.zoom < 0.1f) m_camComp.zoom = 0.1f;

        // Camera reset (zoom and rotation)
        if (IsKeyPressed(KEY_R))
        {
            m_camComp.zoom = 1.0f;
            m_camComp.rotation = 0.0f;
        }
    }
    void onDestroy() {
        printf("Destroy\n");
    }
};