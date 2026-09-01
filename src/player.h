#ifndef SRC_PLAYER_H
#define SRC_PLAYER_H

#include "Input.h"
#include "World.h"
#include <cmath>

class Player {
public:
    Player();
    void init(const float spawnPos[3]);
    void update(float lx, float ly, double dt, const Input &input, World &world);
    void rotate(float yawDelta, float pitchDelta);
    void tryBreakBlock(World &world);
    void tryPlaceBlock(World &world);

    const float* position() const { return pos; }
    float yaw() const { return yaw_; }
    float pitch() const { return pitch_; }

private:
    float pos[3];
    float velY;
    float yaw_;
    float pitch_;
    int hotbarSlot;
    // helper: raycast from camera, returns block coords hit and normal
    bool raycastHit(const World &world, int &bx, int &by, int &bz, int &nx, int &ny, int &nz) const;
};

#endif // SRC_PLAYER_H