#include "tiles.hpp"
#include "32blit.hpp"
#include "sprites.hpp"
#include "assets.hpp"

using namespace blit;

namespace game {
const int TILE_SIZE = 12;

static Surface *tileset = nullptr;

void load_tileset() {
    tileset = Surface::load(asset_tileset);
}

void draw_tile(int sx, int sy, uint8_t tile) {
    switch(tile) {
        case TILE_FLOOR: {
            auto r = sprites::rect_of(sprites::SpriteID::ROCK_BACKGROUND);
            screen.blit(tileset, Rect(r.x, r.y, r.w, r.h), Point(sx, sy));
            break;
        }
        case TILE_WALL: {
            auto r = sprites::rect_of(sprites::SpriteID::ROCK_FLOOR);
            screen.blit(tileset, Rect(r.x, r.y, r.w, r.h), Point(sx, sy));
            break;
        }
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