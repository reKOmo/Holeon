#include "CameraController.h"

void CameraController::onCreate() {
    player = getEntityByName("player");
}

void CameraController::onUpdate(float delta) {
    auto& camComp = m_Owner.getComponent<raylib::Camera2D>();
    auto& playerTrans = player.getComponent<Engine::Components::TransformComponent>();
    raylib::Vector2 offset = { 16.0, 16.0 };
    camComp.target = playerTrans.Position + offset;

}
