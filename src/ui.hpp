#pragma once
#include "map.hpp"
#include "camera.hpp"
#include "game.hpp"

namespace game {
    void draw_ui(const Map &map, const Camera &cam);

    void draw_input_debug();
}