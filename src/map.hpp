#pragma once
#include <vector>
#include <cstdint>

namespace game {

enum Tile : uint8_t {
    TILE_EMPTY = 0,
    TILE_FLOOR,
    TILE_WALL,
    TILE_WOOD,
    TILE_GRASS,
    TILE_DIRT
};

struct Map {
    int width;
    int height;
    std::vector<uint8_t> data;
    std::vector<uint8_t> discovered;

    Map(int w, int h);

    void generate();
    uint8_t tile_at(int x, int y) const;
    void set_tile(int x, int y, uint8_t tile);

    bool is_discovered(int x, int y) const;
    void reveal(int tx0, int ty0, int tx1, int ty1);
};

}