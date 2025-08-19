#include "tiles.hpp"
#include "32blit.hpp"

using namespace blit;

namespace game {
const int TILE_SIZE = 12;

void draw_tile(int sx, int sy, uint8_t tile) {
    switch(tile) {
        case TILE_FLOOR:
        screen.pen = Pen(90, 90, 90); // grey stone floor
        screen.rectangle(Rect(sx, sy, TILE_SIZE, TILE_SIZE));
        break;
        case TILE_WALL:
        screen.pen = Pen(40, 40, 40); // darker stone
        screen.rectangle(Rect(sx, sy, TILE_SIZE, TILE_SIZE));
        break;
        case TILE_WOOD:
        screen.pen = Pen(120, 70, 30); // wood / support beams
        screen.rectangle(Rect(sx, sy, TILE_SIZE, TILE_SIZE));
        break;
        default:
        // empty/void
        screen.pen = Pen(0, 0, 0);
        screen.rectangle(Rect(sx, sy, TILE_SIZE, TILE_SIZE));
        break;
    }
}
}