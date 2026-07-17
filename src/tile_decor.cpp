#include "tile_decor.hpp"

namespace game {

uint32_t tile_hash(int x, int y) {
    uint32_t h = static_cast<uint32_t>(x) * 374761393u + static_cast<uint32_t>(y) * 668265263u;
    h = (h ^ (h >> 13)) * 1274126177u;
    return h ^ (h >> 16);
}

sprites::SpriteID sprite_for_floor() { return sprites::SpriteID::ROCK_BACKGROUND; }
sprites::SpriteID sprite_for_wall()  { return sprites::SpriteID::ROCK_FLOOR; }

GrassDecoration grass_decoration_at(int tx, int ty) {
    uint32_t h = tile_hash(tx, ty);

    static const sprites::SpriteID bg_variants[] = {
        sprites::SpriteID::GRASS_BACKGROUND, sprites::SpriteID::GRASS_BACKGROUND_2,
        sprites::SpriteID::GRASS_BACKGROUND_3
    };
    static const sprites::SpriteID tuft_variants[] = {
        sprites::SpriteID::GRASS, sprites::SpriteID::GRASS_2, sprites::SpriteID::GRASS_3,
        sprites::SpriteID::GRASS_4, sprites::SpriteID::GRASS_5, sprites::SpriteID::GRASS_6
    };

    GrassDecoration d{};
    d.background = bg_variants[h % (sizeof(bg_variants)/sizeof(bg_variants[0]))];

    d.has_tuft = (h % 8 == 0);
    if (d.has_tuft)
        d.tuft = tuft_variants[(h >> 4) % (sizeof(tuft_variants)/sizeof(tuft_variants[0]))];

    uint32_t f = h >> 8;
    d.has_feature = true;
    if (f % 23 == 0)      d.feature = sprites::SpriteID::TREE;
    else if (f % 17 == 0) d.feature = sprites::SpriteID::FLOWER_RED;
    else if (f % 29 == 0) d.feature = sprites::SpriteID::MUSHROOM;
    else                  d.has_feature = false;

    return d;
}

sprites::SpriteID dirt_variant_at(int tx, int ty) {
    uint32_t h = tile_hash(tx, ty);
    static const sprites::SpriteID path_variants[] = {
        sprites::SpriteID::ROCK_PATHWAY, sprites::SpriteID::ROCK_PATHWAY_2, sprites::SpriteID::ROCK_PATHWAY_3
    };
    return path_variants[h % (sizeof(path_variants)/sizeof(path_variants[0]))];
}

}