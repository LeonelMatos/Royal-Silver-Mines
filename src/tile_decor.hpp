#pragma once
#include <cstdint>
#include "sprites.hpp"

namespace game {

// Deterministic hash of map coordinates, without storing decoration in Map::data
uint32_t tile_hash(int x, int y);

struct GrassDecoration {
    sprites::SpriteID background;   // always present
    bool has_tuft;
    sprites::SpriteID tuft;
    bool has_feature;
    sprites::SpriteID feature;      // tree / flower / mushroom
};

GrassDecoration grass_decoration_at(int tx, int ty);
sprites::SpriteID dirt_variant_at(int tx, int ty);
sprites::SpriteID sprite_for_floor();
sprites::SpriteID sprite_for_wall();

}