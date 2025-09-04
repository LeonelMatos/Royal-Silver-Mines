#pragma once
#include "map.hpp"
#include "camera.hpp"
#include "game.hpp"

namespace game {
    void draw_ui(const Map &map, const Camera &cam);

    int get_max_log_lines();

    void draw_input_debug();
    void draw_logs();
}