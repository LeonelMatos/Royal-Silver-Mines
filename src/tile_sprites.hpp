#pragma once
#include <cstdint>
#include <string>
#include "map.hpp"

namespace game {

std::string humanize_token(const char *token); // "ROCK_FLOOR" -> "Rock Floor"

// Describes exactly what's rendered at (tx, ty) - pulls from the same
// tile_decor functions tiles.cpp uses, so it can't drift from the render.
std::string describe_tile(const Map &map, int tx, int ty);

}