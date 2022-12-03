#include "raylib-cpp.hpp"
#include "entt.hpp"
#include "Engine.h"
#include "TerrainMap.h"
#include <iostream>


int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;

   InitWindow(screenWidth, screenHeight, "Holeon");

    char* files[] = {
        RESOURCE_PATH("basic_sword.png"),
        RESOURCE_PATH("Holeon-tileset.png"),
        RESOURCE_PATH("Grass.png"),
    };

    Engine::Renderer::TextureManager textureMgr;
    textureMgr.loadFiles(files, 3);

    Engine::Scene scene(&textureMgr);


    auto ent = scene.createEntity("sword");
    Engine::Renderer::Material swordMaterial = { textureMgr.getTexture(0), {0.0, 0.0, 32.0, 32.0} };
    ent.addComponent<Engine::Components::SpriteComponent>(swordMaterial);
    auto& swordTrans = ent.addComponent<Engine::Components::TransformComponent>();
    swordTrans.Position = { 0.0, 0.0 };

    auto sword2 = scene.createEntity("sword2");
    Engine::Renderer::Material grassMat = { textureMgr.getTexture(2), {0.0, 0.0, 32.0, 32.0} };
    auto& grassSprite = sword2.addComponent<Engine::Components::SpriteComponent>(grassMat);
    grassSprite.zIndex = 2;
    auto& swordTrans2 = sword2.addComponent<Engine::Components::TransformComponent>();
    swordTrans2.Position = { 40.0, 40.0 };
    auto& infoSword = sword2.getComponent<Engine::Components::InfoComponent>();
    infoSword.parent = ent;

    auto cam = scene.createEntity("NotCamera");
    auto& camComp = cam.addComponent<raylib::Camera2D>();
    camComp.target = { 0.0, 0.0 };
    camComp.offset = { -16.0, -16.0 };
    camComp.zoom = 0.5;
    camComp.rotation = 0.0;

    scene.setCamera(cam);

    class PlayerController : public Engine::Script {
        void onCreate() {
        }
        void onUpdate(float delta) {
            auto& trans = m_Owner.getComponent<Engine::Components::TransformComponent>();
            const float speed = 200.0 * delta;
            if (IsKeyDown(KEY_A)) trans.Position.x -= speed;
            if (IsKeyDown(KEY_D)) trans.Position.x += speed;
            if (IsKeyDown(KEY_W)) trans.Position.y -= speed;
            if (IsKeyDown(KEY_S)) trans.Position.y += speed;
        }
        void onDestroy() {
           
        }
    };

    class PlayerController2 : public Engine::Script {
        void onCreate() {
        }
        void onUpdate(float delta) {
            auto& trans = m_Owner.getComponent<Engine::Components::TransformComponent>();
            const float speed = 200.0 * delta;
            if (IsKeyDown(KEY_LEFT)) trans.Position.x -= speed;
            if (IsKeyDown(KEY_RIGHT)) trans.Position.x += speed;
            if (IsKeyDown(KEY_UP)) trans.Position.y -= speed;
            if (IsKeyDown(KEY_DOWN)) trans.Position.y += speed;
        }
        void onDestroy() {

        }
    };


    class CameraController : public Engine::Script {
    public:
        void onCreate() {
            auto& ent = getEntityByName("sword");
            std::cout << ent.getComponent<Engine::Components::InfoComponent>().name << "\n";
            auto& m_camComp = m_Owner.getComponent<raylib::Camera2D>();
            m_camComp.offset = { (float)screenWidth / 2.0f, (float)screenHeight / 2.0f };
            auto& tag = m_Owner.getComponent<Engine::Components::InfoComponent>();
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

    cam.addComponent<Engine::Components::ScriptComponent>().bind<CameraController>();
    ent.addComponent<Engine::Components::ScriptComponent>().bind<PlayerController>();
    sword2.addComponent<Engine::Components::ScriptComponent>().bind<PlayerController2>();


    auto terrain = scene.createEntity();
    Engine::TerrainMap map;
    map.loadMap(RESOURCE_PATH("Holeon - test map 1.csv"));
    Engine::Renderer::Material terrainMat = { textureMgr.getTexture(1), { 0.0, 0.0, 32.0, 32.0 } };
    printf("%d",terrainMat.texture->id);
    terrain.addComponent<Engine::Components::TerrainComponent>(terrainMat, map);
    auto& terrainTrans = terrain.addComponent<Engine::Components::TransformComponent>();
    terrainTrans.Position = { 0.0, 0.0 };

    

   SetTargetFPS(60);
    while (!WindowShouldClose()) {

        //swordTrans.Rotation++;
        scene.update();
        scene.render();
    }

    CloseWindow();
    
    return 0;
}
