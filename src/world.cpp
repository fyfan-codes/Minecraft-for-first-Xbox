#include "World.h"
#include <cmath>
#include <cstring>

World::World() {
    seed_ = 0;
    memset(blocks, 0, sizeof(blocks));
}

void World::init(unsigned int seed) {
    seed_ = seed;
    // Generate deterministic terrain using simple functions (fast)
    for (int x = 0; x < WIDTH; ++x) {
        for (int z = 0; z < DEPTH; ++z) {
            int h = heightAt(x, z);
            for (int y = 0; y < HEIGHT; ++y) {
                if (y > h) {
                    blocks[x][y][z] = BLOCK_AIR;
                } else if (y == h) {
                    // top block: grass or sand if low
                    if (h <= 2) blocks[x][y][z] = BLOCK_SAND;
                    else blocks) {
                    blocks[x][y][z] = BLOCK_DIRT;
                } else {
                    blocks[x][y][z] = BLOCK_STONE;
                }
            }
        }
    }

    // Simple trees scatter
    for (int x = 2; x < WIDTH - 2; ++x) {
        for (int z = 2; z < DEPTH - 2; ++z) {
            unsigned int h = hashPos(x, z);
            if ((h % 100) < 6) { // sparse trees
                int top = heightAt(x, z);
                if (top > 1 && get(x, top, z) == BLOCK_GRASS) {
                    // trunk
                    for (int t = 1; t <= 3 && top + t < HEIGHT; ++t) set(x, top + t, z, BLOCK_WOOD);
                    // leaves layer
                    for (int lx = -2; lx <= 2; ++lx)
                        for (int lz = -2; lz <= 2; ++lz)
                            if (abs(lx) + abs(lz) < 3) {
                                int ly = top + 4;
                                if (ly < HEIGHT) {
                                    if (get(x + lx, ly, z + lz) == BLOCK_AIR) set(x + lx, ly, z + lz, BLOCK_LEAVES);
                                }
                            }
                }
            }
        }
    }

    // Simple cave carving: random tunnels
    for (int i = 0; i < 60; ++i) {
        int cx = (hashPos(i, seed_) % WIDTH);
        int cz = (hashPos(i + 7, seed_) % DEPTH);
        int cy = 2 + (hashPos(i + 13, seed_) % (HEIGHT - 4));
        int radius = 1 + (hashPos(i + 19, seed_) % 2);
        for (int dx = -radius; dx <= radius; ++dx)
            for (int dy = -radius; dy <= radius; ++dy)
                for (int dz = -radius; dz <= radius; ++dz) {
                    int wx = cx + dx, wy = cy + dy, wz = cz + dz;
                    if (inBounds(wx, wy, wz)) set(wx, wy, wz, BLOCK_AIR);
                }
    }
}

int World::heightAt(int x, int z) const {
    // map x,z in [0,WIDTH) [0,DEPTH) into world coordinates
    // simple deterministic pseudo-noise mixture: sin/cos + hashed bumps
    double base = 4.0 + 4.0 * (sin((x + seed_ * 7) * 0.45) + cos((z + seed_ * 11) * 0.35));
    unsigned int h = (unsigned int)fabs(base);
    // noise bump via hash
    unsigned int hv = hashPos(x * 73856093, z * 19349663);
    h += (hv % 4);
    if ((int)h >= HEIGHT - 1) h = HEIGHT - 2;
    return (int)h;
}

unsigned int World::hashPos(int x, int z) const {
    unsigned int h = (unsigned int)(x * 374761393u + z * 668265263u + seed_ * 2246822519u);
    h = (h ^ (h >> 13)) * 1274126177u;
    return h ^ (h >> 16);
}

Block World::get(int x, int y, int z) const {
    if (!inBounds(x, y, z)) return BLOCK_AIR;
    return blocks[x][y][z];
}

void World::set(int x, int y, int z, Block b) {
    if (!inBounds(x, y, z)) return;
    blocks[x][y][z] = b;
}

bool World::inBounds(int x, int y, int z) const {
    return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && z >= 0 && z < DEPTH;
}

void World::spawnPosition(float outPos[3]) const {
    // spawn near center, above ground
    int sx = WIDTH / 2;
    int sz = DEPTH / 2;
    int h = heightAt(sx, sz);
    outPos[0] = (float)sx + 0.5f;
    outPos[1] = (float)h + 2.0f;
    outPos[2] = (float)sz + 0.5f;
}