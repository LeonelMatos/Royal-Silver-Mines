#pragma once
#include <vector>
#include <cstdint>

namespace game {

enum Tile : uint8_t {
    TILE_EMPTY = 0,
    TILE_FLOOR,
    TILE_WALL,
    TILE_WOOD
};

struct Map {
    int width;
    int height;
    std::vector<uint8_t> data;

    Map(int w, int h);

    void generate();
    uint8_t tile_at(int x, int y) const;
};

}