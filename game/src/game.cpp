#include "raylib.h"
#include "entt.hpp"
#include "Engine.h"
#include "TerrainMap.h"
#include <iostream>

int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;


    Engine::TerrainMap map;
    map.loadMap(RESOURCE_PATH("Holeon - test map 1.csv"));

    //InitWindow(screenWidth, screenHeight, "Holeon");

    //char* files[] = {
    //    RESOURCE_PATH("basic_sword.png"),
    //};

    //Engine::Renderer::TextureManager textureMgr;
    //textureMgr.loadFiles(files, 1);

    //Engine::Scene scene(&textureMgr);


    //auto ent = scene.createEntity();
    //Engine::Renderer::Material swordMaterial(0, { 0.0, 0.0, 32.0, 32.0 });
    //ent.addComponent<Engine::Components::SpriteComponent>(swordMaterial);
    //auto& swordTrans = ent.addComponent<Engine::Components::TransformComponent>();
    //swordTrans.Position = { 0.0, 0.0 };

    //auto cam = scene.createEntity();
    //auto& camComp = cam.addComponent<Camera2D>();
    //camComp.target = { 0.0, 0.0 };
    //camComp.offset = { -16.0, -16.0 };
    //camComp.zoom = 0.5;
    //camComp.rotation = 0.0;

    //scene.setCamera(cam);

    //class CameraController : public Engine::Script {
    //public:
    //    void onCreate() {
    //        printf("Created\n");
    //    }
    //    void onUpdate(float delta) {
    //        auto& m_camComp = m_Owner.getComponent<Camera2D>();
    //        // Camera rotation controls
    //        if (IsKeyDown(KEY_A)) m_camComp.rotation--;
    //        else if (IsKeyDown(KEY_S)) m_camComp.rotation++;

    //        // Limit camera rotation to 80 degrees (-40 to 40)
    //        /*if (m_camComp.rotation > 40) m_camComp.rotation = 40;
    //        else if (m_camComp.rotation < -40) m_camComp.rotation = -40;*/
    //        m_camComp.rotation = (float)(((int)m_camComp.rotation + 1) % 360);

    //        // Camera zoom controls
    //        m_camComp.zoom += ((float)GetMouseWheelMove() * 0.05f);

    //        if (m_camComp.zoom > 3.0f) m_camComp.zoom = 3.0f;
    //        else if (m_camComp.zoom < 0.1f) m_camComp.zoom = 0.1f;

    //        // Camera reset (zoom and rotation)
    //        if (IsKeyPressed(KEY_R))
    //        {
    //            m_camComp.zoom = 1.0f;
    //            m_camComp.rotation = 0.0f;
    //        }
    //        printf("%f\n", m_camComp.zoom);
    //    }
    //    void onDestroy() {
    //        printf("Destroy\n");
    //    }
    //};

    //cam.addComponent<Engine::Components::ScriptComponent>().bind<CameraController>();

    //

    //SetTargetFPS(60);
    //while (!WindowShouldClose()) {

    //    //swordTrans.Rotation++;
    //    scene.update();
    //    scene.render();
    //}

    //CloseWindow();
    return 0;
}
