#ifndef SRC_BLOCKS_H
#define SRC_BLOCKS_H

#include <cstdint>

// Block IDs for the prototype.
// Keep values small so they can be stored in uint8_t arrays.
enum Block : uint8_t {
    BLOCK_AIR    = 0,
    BLOCK_GRASS  = 1,
    BLOCK_DIRT   = 2,
    BLOCK_STONE  = 3,
    BLOCK_SAND   = 4,
    BLOCK_WOOD   = 5,
    BLOCK_LEAVES = 6,
    // Add new blocks here (keep <= 255)
};

static inline bool isOpaque(Block b) {
    // Leaves are non-opaque for simple rendering/collision rules in the prototype.
    return b != BLOCK_AIR && b != BLOCK_LEAVES;
}

#endif // SRC_BLOCKS_H