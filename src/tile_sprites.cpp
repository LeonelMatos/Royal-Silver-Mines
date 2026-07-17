#include "tile_sprites.hpp"
#include "map.hpp"
#include <cctype>

namespace game {

const TileSpriteInfo& tile_sprite_info(uint8_t tile) {
    static const TileSpriteInfo floor_info = { true, sprites::SpriteID::ROCK_FLOOR };
    static const TileSpriteInfo wall_info  = { true, sprites::SpriteID::BRICK_WALL };
    static const TileSpriteInfo wood_info  = { true, sprites::SpriteID::WOOD_TIMBER };
    static const TileSpriteInfo empty_info = { false, sprites::SpriteID::COUNT };

    switch (tile) {
        case TILE_FLOOR: return floor_info;
        case TILE_WALL:  return wall_info;
        case TILE_WOOD:  return wood_info;
        default:         return empty_info;
    }
}

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

}