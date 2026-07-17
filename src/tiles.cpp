#include "tiles.hpp"
#include "32blit.hpp"
#include "sprites.hpp"
#include "assets.hpp"
#include "tile_decor.hpp"

using namespace blit;

namespace game {
const int TILE_SIZE = 12;

static Surface *tileset = nullptr;

void load_tileset() {
    tileset = Surface::load(asset_tileset);
}

static void blit_sprite(sprites::SpriteID id, int sx, int sy) {
    auto r = sprites::rect_of(id);
    screen.blit(tileset, Rect(r.x, r.y, r.w, r.h), Point(sx, sy));
}

void draw_tile(int sx, int sy, int tx, int ty, uint8_t tile, bool discovered) {
    if (!discovered) {
        screen.pen = Pen(0, 0, 0);
        screen.rectangle(Rect(sx, sy, TILE_SIZE, TILE_SIZE));
        return;
    }

    switch(tile) {
        case TILE_FLOOR:
            blit_sprite(sprite_for_floor(), sx, sy);
            break;
        case TILE_WALL:
            blit_sprite(sprite_for_wall(), sx, sy);
            break;
        case TILE_WOOD:
            screen.pen = Pen(120, 70, 30); // wood / support beams - no sprite yet
            screen.rectangle(Rect(sx, sy, TILE_SIZE, TILE_SIZE));
            break;
        case TILE_GRASS: {
            GrassDecoration d = grass_decoration_at(tx, ty);
            blit_sprite(d.background, sx, sy);
            if (d.has_tuft) blit_sprite(d.tuft, sx, sy);
            if (d.has_feature) blit_sprite(d.feature, sx, sy);
            break;
        }
        case TILE_DIRT:
            blit_sprite(dirt_variant_at(tx, ty), sx, sy);
            break;
        default:
            screen.pen = Pen(0, 0, 0);
            screen.rectangle(Rect(sx, sy, TILE_SIZE, TILE_SIZE));
            break;
    }

    if (tile != TILE_GRASS && tile != TILE_DIRT) {
        screen.pen = Pen(0, 0, 0, 130);
        screen.rectangle(Rect(sx, sy, TILE_SIZE, TILE_SIZE));
    }
}
}