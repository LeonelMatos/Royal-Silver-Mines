#include "selection.hpp"
#include "32blit.hpp"
#include "tiles.hpp"
#include "logs.hpp"
#include <algorithm>

using namespace blit;

namespace game {

TilePos cursor{0, 0};

namespace {
    bool anchor_set = false;
    TilePos anchor{0, 0};
}

bool is_area_action(PendingAction action) {
    return action == PendingAction::CarveFloor || action == PendingAction::CarveShaft;
}

void selection_enter(const Camera &cam) {
    cursor.x = int((cam.x + cam.view_w / 2) / TILE_SIZE);
    cursor.y = int((cam.y + cam.view_h / 2) / TILE_SIZE);
    anchor_set = false;
}

void handle_cursor_input(Camera &cam, const Map &map) {
    int dx = 0, dy = 0;
    if (buttons.pressed & Button::DPAD_LEFT) dx = -1;
    if (buttons.pressed & Button::DPAD_RIGHT) dx = 1;
    if (buttons.pressed & Button::DPAD_UP) dy = -1;
    if (buttons.pressed & Button::DPAD_DOWN) dy = 1;
    if (dx == 0 && dy == 0) return;

    cursor.x += dx;
    cursor.y += dy;
    if(cursor.x < 0) cursor.x = 0;
    if(cursor.x > map.width - 1) cursor.x = map.width - 1;
    if(cursor.y < 0) cursor.y = 0;
    if(cursor.y > map.height - 1) cursor.y = map.height - 1;

    float px = cursor.x * TILE_SIZE + TILE_SIZE / 2;
    float py = cursor.y * TILE_SIZE + TILE_SIZE / 2;
    cam.ensure_visible(px, py, TILE_SIZE * 2, map.width * TILE_SIZE, map.height * TILE_SIZE);
}

namespace {
    void apply_action_at(Map &map, PendingAction action, int tx, int ty) {
        switch(action) {
            case PendingAction::CarveFloor: map.set_tile(tx, ty, TILE_FLOOR); break;
            case PendingAction::CarveShaft: map.set_tile(tx, ty, TILE_WOOD); break;
            case PendingAction::BuildWall: map.set_tile(tx, ty, TILE_WALL); break;
            case PendingAction::BuildStorage:
            case PendingAction::BuildHousing:
            case PendingAction::ManageAssignWorker:
            case PendingAction::ManageSetPriority:
                add_log("(placeholder) no system implemented, action not applied");
                break;
            default: break;
        }
    }
}

void selection_confirm(Map &map) {
    if(is_area_action(pending_action)) {
        if(!anchor_set) {
            anchor = cursor;
            anchor_set = true;
            return;
        }
        int x0 = std::min(anchor.x, cursor.x), x1 = std::max(anchor.x, cursor.x);
        int y0 = std::min(anchor.y, cursor.y), y1 = std::max(anchor.y, cursor.y);

        for(int y = y0; y <= y1; y++)
            for(int x = x0; x <= x1; x++)
                apply_action_at(map, pending_action, x, y);
        anchor_set = false;
    }
    else {
        apply_action_at(map, pending_action, cursor.x, cursor.y);
    }
}

//returns true when nothing pending, returns to menu
bool selection_cancel() {
    if(anchor_set) {
        anchor_set = false;
        return false;
    }
    return true;
}

const char* inspect_label(const Map &map) {
    switch(map.tile_at(cursor.x, cursor.y)) {
        case TILE_FLOOR: return "Floor";
        case TILE_WALL: return "Wall";
        case TILE_WOOD: return "Wood support";
        default: return "Empty";
    }
}

void draw_selection_cursor(const Camera &cam) {
    int sx = int(cursor.x * TILE_SIZE - cam.x);
    int sy = int(cursor.y * TILE_SIZE - cam.y);

    screen.pen = Pen(255, 255, 255);
    screen.rectangle(Rect(sx, sy, TILE_SIZE, 1));
    screen.rectangle(Rect(sx, sy + TILE_SIZE - 1, TILE_SIZE, 1));
    screen.rectangle(Rect(sx, sy, 1, TILE_SIZE));
    screen.rectangle(Rect(sx + TILE_SIZE - 1, sy, 1, TILE_SIZE));

    if(anchor_set) {
        int ax = int(anchor.x * TILE_SIZE - cam.x);
        int ay = int(anchor.y * TILE_SIZE - cam.y);
        int x0 = std::min(ax, sx), x1 = std::max(ax, sx) + TILE_SIZE;
        int y0 = std::min(ay, sy), y1 = std::max(ay, sy) + TILE_SIZE;
        screen.pen = Pen(255, 255, 0, 80);
        screen.rectangle(Rect(x0, y0, x1 - x0, y1 - y0));
    }
}

}