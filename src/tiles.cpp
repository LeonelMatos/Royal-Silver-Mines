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

static uint32_t tile_hash(int x, int y) {
    uint32_t h = static_cast<uint32_t>(x) * 374761393u + static_cast<uint32_t>(y) * 668265263u;
    h = (h ^ (h >> 13)) * 1274126177u;
    return h ^ (h >> 16);
}

void draw_tile(int sx, int sy, int tx, int ty, uint8_t tile, bool discovered) {
    if (!discovered) {
        screen.pen = Pen(0, 0, 0);
        screen.rectangle(Rect(sx, sy, TILE_SIZE, TILE_SIZE));
        return;
    }

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
        case TILE_GRASS: {
            uint32_t h = tile_hash(tx, ty);

            // calm base fill on every tile, tufts are sparser layers on top
            static const sprites::SpriteID grass_bg_variants[] = {
                sprites::SpriteID::GRASS_BACKGROUND, sprites::SpriteID::GRASS_BACKGROUND_2,
                sprites::SpriteID::GRASS_BACKGROUND_3
            };
            auto bg = sprites::rect_of(grass_bg_variants[h % (sizeof(grass_bg_variants)/sizeof(grass_bg_variants[0]))]);
            screen.blit(tileset, Rect(bg.x, bg.y, bg.w, bg.h), Point(sx, sy));

            // roughly one tile in three gets a tuft on top, the rest stay open/blank
            if (h % 8 == 0) {
                static const sprites::SpriteID grass_variants[] = {
                    sprites::SpriteID::GRASS, sprites::SpriteID::GRASS_2, sprites::SpriteID::GRASS_3,
                    sprites::SpriteID::GRASS_4, sprites::SpriteID::GRASS_5, sprites::SpriteID::GRASS_6
                };
                auto r = sprites::rect_of(grass_variants[(h >> 4) % (sizeof(grass_variants)/sizeof(grass_variants[0]))]);
                screen.blit(tileset, Rect(r.x, r.y, r.w, r.h), Point(sx, sy));
            }

            // larger decorations stay sparse, same as before
            uint32_t d = h >> 8;
            if (d % 23 == 0) {
                auto tr = sprites::rect_of(sprites::SpriteID::TREE);
                screen.blit(tileset, Rect(tr.x, tr.y, tr.w, tr.h), Point(sx, sy));
            } else if (d % 17 == 0) {
                auto fr = sprites::rect_of(sprites::SpriteID::FLOWER_RED);
                screen.blit(tileset, Rect(fr.x, fr.y, fr.w, fr.h), Point(sx, sy));
            } else if (d % 29 == 0) {
                auto mr = sprites::rect_of(sprites::SpriteID::MUSHROOM);
                screen.blit(tileset, Rect(mr.x, mr.y, mr.w, mr.h), Point(sx, sy));
            }
            break;
        }
        case TILE_DIRT: {
            uint32_t h = tile_hash(tx, ty);
            static const sprites::SpriteID path_variants[] = {
                sprites::SpriteID::ROCK_PATHWAY, sprites::SpriteID::ROCK_PATHWAY_2, sprites::SpriteID::ROCK_PATHWAY_3
            };
            auto r = sprites::rect_of(path_variants[h % (sizeof(path_variants)/sizeof(path_variants[0]))]);
            screen.blit(tileset, Rect(r.x, r.y, r.w, r.h), Point(sx, sy));
            break;
        }
        default:
        // empty/void
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