#pragma once
#include <cstdint>
#include "map.hpp"

namespace game {
    void load_tileset();
    void draw_tile(int sx, int sy, uint8_t tile);
    extern const int TILE_SIZE;
}