#pragma once
#include "map.hpp"
#include "camera.hpp"
#include "menu.hpp"

namespace game {

struct TilePos { int x, y; };
extern TilePos cursor;

bool is_area_action(PendingAction action);

//call when entering Selecting/Inspecting
void selection_enter(const Camera &cam);

//D-pad moves cursor
void handle_cursor_input(Camera &cam, const Map &map);

//A-button pressed while Selecting
void selection_confirm(Map &map, const Camera &cam);

//Y-button pressed while Selecting
bool selection_cancel();

//tile name under cursor
std::string inspect_label(const Map &map);

//rectangle preview
void draw_selection_cursor(const Camera &cam);

}