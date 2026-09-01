#pragma once
#include <cstdint>
#include "World.h"
#include "Input.h"
#include "Player.h"

struct IDirect3D8;
struct IDirect3DDevice8;
struct IDirect3DTexture8;

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init();
    void shutdown();

    void beginFrame();
    void endFrame();

    uint32_t getTicks() const;

    // Camera
    void setCamera(const float pos[3], float yawDeg, float pitchDeg);

    // World rendering
    void renderWorld(const World &world);

    // UI
    void drawCrosshair();
    void drawDebugText(const Input &input, const World &world, const Player &player);

private:
    // internal helpers
    bool createDevice();
    bool createResources();
    void releaseResources();
    void uploadAtlas();

    // draw helpers
    void drawFace(float x0, float y0, float z0, float x1, float y1, float z1,
                  float u0, float v0, float u1, float v1);
    void drawQuad(const float verts[18], const float uvs[8]);

    // simple projection/view math helpers
    void computeViewProjection();

private:
    IDirect3D8 *d3d_;
    IDirect3DDevice8 *dev_;
    IDirect3DTexture8 *atlasTex_;

    // camera state
    float camPos_[3];
    float camYawDeg_;
    float camPitchDeg_;

    // matrices (row-major D3D)
    float matView[16];
    float matProj[16];

    // simple timing
    uint32_t lastTicks_;
    uint32_t fpsCounter_;
    uint32_t fpsTimer_;
    uint32_t lastFPS_;

    // atlas info
    int atlasSize_;
    int tileSize_;
};