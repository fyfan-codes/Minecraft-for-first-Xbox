#include "Player.h"
#include <algorithm>

Player::Player() {
    pos[0] = pos[1] = pos[2] = 0.0f;
    velY = 0.0f;
    yaw_ = 0.0f;
    pitch_ = 0.0f;
    hotbarSlot = 1;
}

void Player::init(const float spawnPos[3]) {
    pos[0] = spawnPos[0];
    pos[1] = spawnPos[1];
    pos[2] = spawnPos[2];
    velY = 0.0f;
    yaw_ = 0.0f;
    pitch_ = 0.0f;
}

void Player::rotate(float yawDelta, float pitchDelta) {
    yaw_ += yawDelta;
    pitch_ += pitchDelta;
    if (pitch_ > 89.9f) pitch_ = 89.9f;
    if (pitch_ < -89.9f) pitch_ = -89.9f;
}

void Player::update(float lx, float ly, double dt, const Input &input, World &world) {
    // movement: lx/ly are in [-1,1]
    const float walkSpeed = 3.5f;
    float forward = ly;
    float strafe = lx;
    float rad = yaw_ * (3.14159265f / 180.0f);
    float siny = sinf(rad);
    float cosy = cosf(rad);

    float dx = (cosy * forward + siny * strafe) * walkSpeed * (float)dt;
    float dz = ( -siny * forward + cosy * strafe) * walkSpeed * (float)dt;

    // simple collision: check location after move with block occupancy underfoot and collision at eye level
    float newX = pos[0] + dx;
    float newZ = pos[2] + dz;
    int ix = (int)floorf(newX);
    int iz = (int)floorf(newZ);
    int iy = (int)floorf(pos[1]);

    // prevent walking into solid blocks at player height (y and y+1)
    if (world.get(ix, iy, iz) == BLOCK_AIR && world.get(ix, iy + 1, iz) == BLOCK_AIR) {
        pos[0] = newX;
        pos[2] = newZ;
    }

    // gravity
    velY -= 9.8f * (float)dt;
    pos[1] += velY * (float)dt;

    // ground check
    int belowY = (int)floorf(pos[1]);
    if (belowY < 0) {
        pos[1] = 1.0f;
        velY = 0.0f;
    } else {
        // if colliding with ground
        if (world.get((int)floorf(pos[0]), belowY - 1, (int)floorf(pos[2])) != BLOCK_AIR && pos[1] < belowY + 1.1f) {
            pos[1] = belowY + 1.1f;
            velY = 0.0f;
        }
    }

    // Jump - A button
    if (input.isPressed(XINPUT_GAMEPAD_A)) {
        // only jump if on ground
        int pBelow = (int)floorf(pos[1]) - 1;
        if (pBelow >= 0 && world.get((int)floorf(pos[0]), pBelow, (int)floorf(pos[2])) != BLOCK_AIR) {
            velY = 5.2f;
        }
    }

    // D-pad hotbar (simple per-frame change; later should handle edge triggers)
    if (input.isPressed(XINPUT_GAMEPAD_DPAD_LEFT)) {
        hotbarSlot = std::max(1, hotbarSlot - 1);
    } else if (input.isPressed(XINPUT_GAMEPAD_DPAD_RIGHT)) {
        hotbarSlot = std::min(8, hotbarSlot + 1);
    }
}

bool Player::raycastHit(const World &world, int &bx, int &by, int &bz, int &nx, int &ny, int &nz) const {
    // ray march along camera direction; step small increments up to range
    float radY = yaw_ * (3.14159265f / 180.0f);
    float radX = pitch_ * (3.14159265f / 180.0f);
    // camera direction
    float dx = cosf(radX) * sinf(radY);
    float dy = -sinf(radX);
    float dz = cosf(radX) * cosf(radY);

    const float maxRange = 6.0f;
    const float step = 0.1f;
    float sx = pos[0];
    float sy = pos[1] + 0.2f; // eye offset
    float sz = pos[2];

    for (float t = 0.0f; t <= maxRange; t += step) {
        float cx = sx + dx * t;
        float cy = sy + dy * t;
        float cz = sz + dz * t;
        int ix = (int)floorf(cx);
        int iy = (int)floorf(cy);
        int iz = (int)floorf(cz);
        if (!world.inBounds(ix, iy, iz)) continue;
        if (world.get(ix, iy, iz) != BLOCK_AIR) {
            // hit block; approximate normal by finding which face entry was nearest
            float localX = cx - ix;
            float localY = cy - iy;
            float localZ = cz - iz;
            nx = ny = nz = 0;
            float ax = fabsf(localX - 0.5f);
            float ay = fabsf(localY - 0.5f);
            float az = fabsf(localZ - 0.5f);
            if (ax > ay && ax > az) nx = (localX < 0.5f) ? -1 : 1;
            else if (ay > ax && ay > az) ny = (localY < 0.5f) ? -1 : 1;
            else nz = (localZ < 0.5f) ? -1 : 1;

            bx = ix; by = iy; bz = iz;
            return true;
        }
    }
    return false;
}

void Player::tryBreakBlock(World &world) {
    int bx, by, bz, nx, ny, nz;
    if (raycastHit(world, bx, by, bz, nx, ny, nz)) {
        world.set(bx, by, bz, BLOCK_AIR);
    }
}

void Player::tryPlaceBlock(World &world) {
    int bx, by, bz, nx, ny, nz;
    if (raycastHit(world, bx, by, bz, nx, ny, nz)) {
        int px = bx + nx;
        int py = by + ny;
        int pz = bz + nz;
        if (world.inBounds(px, py, pz) && world.get(px, py, pz) == BLOCK_AIR) {
            // place block of hotbar type - for v0.1 we place block id equal to hotbarSlot (1..)
            // clamp to known block ids
            int placeId = hotbarSlot;
            if (placeId < 1) placeId = 1;
            if (placeId > 6) placeId = 2; // default to dirt if out of range
            world.set(px, py, pz, (Block)placeId);
        }
    }
}