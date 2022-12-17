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
        RESOURCE_PATH("player_forward.png"),
        RESOURCE_PATH("dialog-frame.png")
    };

    Engine::Renderer::TextureManager textureMgr;
    textureMgr.loadFiles(files, 5);

    Engine::Scene scene(&textureMgr);


    class SpriteAnimator : public Engine::AnimationClip {
    public:
        SpriteAnimator() {}
        SpriteAnimator(int fps) {
            frameTime = 1.0 / (float)fps;
        }
        void onCreate() {
            loop = true;
            duration = -1;
        }
        void onUpdate(float delta) {
            sinceLastFrame += delta;
            if (sinceLastFrame >= frameTime) {
                auto& sprite = m_Owner.getComponent<Engine::Components::SpriteComponent>();
                sprite.imageIndex = (sprite.imageIndex + 1) % sprite.totalFrames;
                sinceLastFrame = 0;
            }
        }
    private:
        float sinceLastFrame = 0;
        float frameTime = 0.081;
    };

    class PlayerController : public Engine::Script {
        void onCreate() {
            auto& anim = m_Owner.getComponent<Engine::Components::AnimationManager>();
            currentAnimId = anim.playAnimation(0, []() {
                printf("Stopped animation");
            });
        }
        void onUpdate(float delta) {
            auto& trans = m_Owner.getComponent<Engine::Components::TransformComponent>();
            const float speed = 200.0 * delta;
            if (IsKeyDown(KEY_A)) trans.Position.x -= speed;
            if (IsKeyDown(KEY_D)) trans.Position.x += speed;
            if (IsKeyDown(KEY_W)) trans.Position.y -= speed;
            if (IsKeyDown(KEY_S)) trans.Position.y += speed;
            if (IsKeyDown(KEY_SPACE) && currentAnimId != -1) {
                printf("%d\n", currentAnimId);
                auto& anim = m_Owner.getComponent<Engine::Components::AnimationManager>();
                anim.stopAnimation(currentAnimId);
                currentAnimId = -1;
            }
        }
        void onDestroy() {

        }
    private: 
        int currentAnimId = -1;
    };

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

    auto player = scene.createEntity("player");
    Engine::Renderer::Material playerMaterial = { textureMgr.getTexture(3), {0.0, 0.0, 32.0, 32.0} };
    auto& animSprite = player.addComponent<Engine::Components::SpriteComponent>(playerMaterial);
    auto& transform = player.addComponent<Engine::Components::TransformComponent>();
    transform.Position = { 0.0, 0.0 };
    player.addComponent<Engine::Components::ScriptComponent>().bind<PlayerController>();
    auto& animManag = player.addComponent<Engine::Components::AnimationManager>();
    const int numOfAnims = 1;
    animManag.animations = new std::shared_ptr<Engine::AnimationClip>[numOfAnims] {
        std::make_shared<SpriteAnimator>(12)
    };
    animManag.animationsSize = numOfAnims;


    auto cam = scene.createEntity("NotCamera");
    auto& camComp = cam.addComponent<raylib::Camera2D>();
    camComp.target = { 0.0, 0.0 };
    camComp.offset = { 200.0, 100.0 };
    camComp.zoom = 0.5;
    camComp.rotation = 0.0;
    cam.addComponent<Engine::Components::ScriptComponent>().bind<CameraController>();

    scene.setCamera(cam);


    auto terrain = scene.createEntity();
    Engine::TerrainMap map;
    map.loadMap(RESOURCE_PATH("Holeon - test map 1.csv"));
    Engine::Renderer::Material terrainMat = { textureMgr.getTexture(1), { 0.0, 0.0, 32.0, 32.0 } };
    printf("%d",terrainMat.texture->id);
    terrain.addComponent<Engine::Components::TerrainComponent>(terrainMat, map);
    auto& terrainTrans = terrain.addComponent<Engine::Components::TransformComponent>();
    terrainTrans.Position = { 0.0, 0.0 };


    auto uiBg = scene.createEntity("uibg");
    auto& uiTrans = uiBg.addComponent<Engine::Components::TransformComponent>();
    uiTrans.Position = { 0.0, 0.0 };
    Engine::Renderer::Material dialogFrame = { textureMgr.getTexture(4), { 0.0, 0.0, 12.0, 12.0 } };
    auto& bg = uiBg.addComponent<Engine::Components::BackgroundComponent>(dialogFrame);
    bg.size = { 62.0, 32.0 };
    float scale = 4.0;
    bg.scale = { scale, scale };


    

   SetTargetFPS(60);
    while (!WindowShouldClose()) {

        //swordTrans.Rotation++;
        scene.update();
        scene.render();
    }

    CloseWindow();
    
    return 0;
}
