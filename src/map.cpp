#include "map.hpp"
#include "32blit.hpp"

namespace game {

Map::Map(int w, int h) : width(w), height(h), data(w * h, TILE_WALL), discovered(w * h, 0) {}

//0-99 roll, replaces dist(rng)
static inline int roll() {
    return blit::random() % 100;
}

void Map::generate() {
    //generation params
    static const int FILL_PROB = 67; //def:45 for wide
    static const int SMOOTH_ITERS = 5;
    static const int SURFACE_ROWS = 14; //baseline entrance level
    static const int CAVE_START = SURFACE_ROWS + 4;
    
    int entrance_x = width / 2;

    //ondulating ground line
    std::vector<int> ground_level(width);
    int level = SURFACE_ROWS;
    for (int x = 0; x < width; x++) {
        if (roll() < 25) level += (roll() < 50) ? -1 : 1;
        level = std::max(SURFACE_ROWS - 3, std::min(SURFACE_ROWS + 3, level));
        ground_level[x] = level;
    }
    //surface, grass under ondulating line, with dirt path
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < ground_level[x] && y < height; y++) {
            data[y * width + x] = TILE_GRASS;
        }
    }
    for (int x = entrance_x - 1; x <= entrance_x + 1; x++) {
        if (x < 0 || x >= width) continue;
        for (int y = 0; y < ground_level[x] && y < height; y++) {
            data[y * width + x] = TILE_DIRT;
        }
    }

    //random fill for underground
    for (int y = CAVE_START + 1; y < height; y++) {
        for (int x = 1; x < width -1; x++) {
            int i = y * width + x;
            data[i] = (roll() < FILL_PROB) ? TILE_WALL : TILE_FLOOR;
        }
    }
    
    //smooth cellular automata
    for(int iter = 0; iter < SMOOTH_ITERS; iter++) {
        std::vector<uint8_t> next = data;
        for (int y = CAVE_START + 1; y < height - 1; y++) {
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
        data[CAVE_START * width + x] = TILE_WALL;
        data[(height - 1) * width + x] = TILE_WALL;
    }
    for (int y = CAVE_START; y < height; y++) {
        data[y * width] = TILE_WALL;
        data[y * width + (width-1)] = TILE_WALL;
    }

    //entrance shaft
    for (int y = 0; y < 8 && (CAVE_START + y) < height; y++)
        data[(CAVE_START + y) * width + entrance_x] = TILE_FLOOR;
    for (int y = 0; y < 4 && (CAVE_START + 8 + y) < height; y++)
        data[(CAVE_START + 8 + y) * width + entrance_x] = TILE_WOOD;

    //entrance pre-explored/discovered
    discovered.assign(width * height, 0);
    for (int y = 0; y < 8 && (CAVE_START + y) < height; y++)
        discovered[(CAVE_START + y) * width + entrance_x] = 1;
    for (int y = 0; y < 4 && (CAVE_START + 8 + y) < height; y++)
        discovered[(CAVE_START + 8 + y) * width + entrance_x] = 1;

    //frontier tiles bordering the surface, visible from outside
    static const int fx[4] = {1, -1, 0, 0};
    static const int fy[4] = {0, 0, 1, -1};

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint8_t t = data[y * width + x];
            if(t == TILE_GRASS || t == TILE_DIRT) continue;
            for (int d = 0; d < 4; d++) {
                int nx = x + fx[d], ny = y + fy[d];
                if(nx < 0 || nx >= width || ny < 0 || ny >= height) continue;
                uint8_t nt = data[ny * width + nx];
                if(nt == TILE_GRASS || nt == TILE_DIRT) {
                    discovered[y * width + x] = 1;
                    break;
                }
            }
        }
    }
        
}

uint8_t Map::tile_at(int x, int y) const {
    if (x < 0 || y < 0 || x >= width || y >= height) return TILE_WALL;
    return data[y * width + x];
}

void Map::set_tile(int x, int y, uint8_t tile) {
    if(x < 0 || y < 0 || x >= width || y >= height) return;
    data[y * width + x] = tile;
}

bool Map::is_discovered(int x, int y) const {
    if (x < 0 || y < 0 || x >= width || y >= height) return false;
    uint8_t t = data[y * width + x];
    if(t == TILE_GRASS || t == TILE_DIRT) return true;
    return discovered[y * width + x] != 0;
}

void Map::reveal(int tx0, int ty0, int tx1, int ty1) {
    tx0 = std::max(0, std::min(tx0, width - 1));
    ty0 = std::max(0, std::min(ty0, height - 1));
    tx1 = std::max(0, std::min(tx1, width - 1));
    ty1 = std::max(0, std::min(ty1, height - 1));

    struct QueuePos {int x, y; };
    static std::vector<QueuePos> queue;
    queue.clear();

    //Seed with anything already discovered inside view
    for (int y = ty0; y <= ty1; y++) {
        for (int x = tx0; x <= tx1; x++) {
            uint8_t t = tile_at(x, y);
            if ((t == TILE_FLOOR || t == TILE_WOOD) && discovered[y * width + x]) {
                queue.push_back({x, y});
            }
        }
    }

    static const int dx[4] = {1, -1, 0, 0};
    static const int dy[4] = {0, 0, 1, -1};

    size_t head = 0;
    while (head < queue.size()) {
        int x = queue[head].x, y = queue[head].y;
        head++;
        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d], ny = y + dy[d];
            if (nx < tx0 || nx > tx1 || ny < ty0 || ny > ty1) continue; //limit to current view
            uint8_t t = tile_at(nx, ny);
            int idx = ny * width + nx;
            if (t == TILE_FLOOR || t == TILE_WOOD) {
                if(!discovered[idx]) {
                    discovered[idx] = 1;
                    queue.push_back({nx, ny});
                }
            }
            //contour wall
            else if (t == TILE_WALL) {
                discovered[idx] = 1;
            }
        }
    }
}

}