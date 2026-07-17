#include "tile_sprites.hpp"
#include "tile_decor.hpp"
#include "sprite_names.hpp"
#include <cctype>

namespace game {

std::string humanize_token(const char *token) {
    std::string out;
    bool start_of_word = true;
    for (const char *p = token; *p; ++p) {
        char c = *p;
        if (c == '_') { out += ' '; start_of_word = true; }
        else if (start_of_word) { out += (char)std::toupper((unsigned char)c); start_of_word = false; }
        else out += (char)std::tolower((unsigned char)c);
    }
    return out;
}

static std::string name_of(sprites::SpriteID id) {
    return humanize_token(sprites::NAMES[(size_t)id]);
}

std::string describe_tile(const Map &map, int tx, int ty) {
    switch (map.tile_at(tx, ty)) {
        case TILE_FLOOR: return name_of(sprite_for_floor());
        case TILE_WALL:  return name_of(sprite_for_wall());
        case TILE_WOOD:  return "Wood support"; // flat-colour tile, no sprite backing it yet
        case TILE_GRASS: {
            GrassDecoration d = grass_decoration_at(tx, ty);
            if (d.has_feature) return name_of(d.feature);
            if (d.has_tuft)    return name_of(d.tuft);
            return name_of(d.background);
        }
        case TILE_DIRT:
            return name_of(dirt_variant_at(tx, ty));
        default:
            return "Empty";
    }
}

}