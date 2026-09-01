#ifndef SRC_WORLD_H
#define SRC_WORLD_H

#include <cstdint>
#include "Blocks.h"

// Small fixed-size world for stage-1 prototype.
// Later stages will replace this with chunked storage.
class World {
public:
    // keep sizes small for prototype; change later for chunking
    static const int WIDTH = 32;
    static const int HEIGHT = 16;
    static const int DEPTH = 32;

    World();
    void init(unsigned int seed);
    Block get(int x, int y, int z) const;
    void set(int x, int y, int z, Block b);
    bool inBounds(int x, int y, int z) const;
    // spawn position for player (outPos must be length 3)
    void spawnPosition(float outPos[3]) const;

private:
    Block blocks[WIDTH][HEIGHT][DEPTH];
    unsigned int seed_;
    int heightAt(int x, int z) const;
    // simple pseudo-random deterministic hash
    unsigned int hashPos(int x, int z) const;
};

#endif // SRC_WORLD_H