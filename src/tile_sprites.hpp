#pragma once
#include <cstdint>
#include <string>
#include "sprites.hpp"
#include "sprite_names.hpp"

namespace game {

struct TileSpriteInfo {
    bool has_sprite;
    sprites::SpriteID sprite;
};

//Single source of info: which sprite represents each Tile
const TileSpriteInfo& tile_sprite_info(uint8_t tile);

//"ROCK_FLOOR" → "Rock Floor"
std::string humanize_token(const char *token);

}