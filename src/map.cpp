#include "map.hpp"
#include <random>

namespace game {

Map::Map(int w, int h) : width(w), height(h), data(w * h, TILE_WALL) {}

void Map::generate() {
    //generation params
    static const int FILL_PROB = 60; //def:45 for wide
    static const int SMOOTH_ITERS = 5;

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, 99);

    //random fill
    for (int y = 1; y < height; y++) {
        for (int x = 1; x < width -1; x++) {
            int i = y * width + x;
            data[i] = (dist(rng) < FILL_PROB) ? TILE_WALL : TILE_FLOOR;
        }
    }

    //entrance shaft (top middle)
    int entrance_x = width / 2;
    for (int y = 0; y < 8 && y < height; y++)
        data[y * width + entrance_x] = TILE_FLOOR;
    for (int y = 0; y < 4 && (y+8) < height; y++)
        data[(y+8) * width + entrance_x] = TILE_WOOD;
    
    //smooth cellular automata
    for(int iter = 0; iter < SMOOTH_ITERS; iter++) {
        std::vector<uint8_t> next = data;
        for (int y = 1; y < height - 1; y++) {
            for (int x = 1; x < width - 1; x++) {
                int walls = 0;
                for (int oy = -1; oy <= 1; oy++) {
                    for (int ox = -1; ox <= 1; ox++) {
                        if (ox == 0 && oy == 0) continue;
                        if(data[(y+oy) * width + (x+ox)] == TILE_WALL) walls++;
                    }
                }
                int idx = y * width + x;
                if (walls >= 5) next[idx] = TILE_WALL;
                else next[idx] = TILE_FLOOR;
            }
        }
        data.swap(next);
    }

    //enforce solid borders
    for (int x = 0; x < width; x++) {
        data[x] = TILE_WALL;
        data[(height - 1) * width + x] = TILE_WALL;
    }
    for (int y = 0; y < height; y++) {
        data[y * width] = TILE_WALL;
        data[y * width + (width-1)] = TILE_WALL;
    }
}

uint8_t Map::tile_at(int x, int y) const {
    if (x < 0 || y < 0 || x >= width || y >= height) return TILE_WALL;
    return data[y * width + x];
}

}