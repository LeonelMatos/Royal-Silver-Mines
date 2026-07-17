#include "menu.hpp"
#include "32blit.hpp"
#include "game.hpp"

using namespace blit;

namespace game {

namespace {
    MenuCategory current_category = MenuCategory::Root;
    int menu_cursor = 0;

    const std::vector<MenuItem>& items_for(MenuCategory cat) {
        static const std::vector<MenuItem> root = {
            {"Build", false, LeafKind::Action, PendingAction::None, MenuCategory::Build},
            {"Carve", false, LeafKind::Action, PendingAction::None, MenuCategory::Carve},
            {"Manage", false, LeafKind::Action, PendingAction::None, MenuCategory::Manage},
            {"Logs", true, LeafKind::OpenLogs, PendingAction::None, MenuCategory::Root},
        };
        static const std::vector<MenuItem> build = {
            {"Wall", true, LeafKind::Action, PendingAction::BuildWall, MenuCategory::Root},
            {"Storage", true, LeafKind::Action, PendingAction::BuildStorage, MenuCategory::Root},
            {"Housing", true, LeafKind::Action, PendingAction::BuildHousing, MenuCategory::Root},
        };
        static const std::vector<MenuItem> carve = {
            {"Floor", true, LeafKind::Action, PendingAction::CarveFloor, MenuCategory::Root},
            {"Shaft", true, LeafKind::Action, PendingAction::CarveShaft, MenuCategory::Root},
        };
        static const std::vector<MenuItem> manage = {
            {"Assign worker", true, LeafKind::Action, PendingAction::ManageAssignWorker, MenuCategory::Root},
            {"Set Priority", true, LeafKind::Action, PendingAction::ManageSetPriority, MenuCategory::Root},
        };

        switch(cat) {
            case MenuCategory::Build: return build;
            case MenuCategory::Carve: return carve;
            case MenuCategory::Manage: return manage;
            default: return root;
        }
    }
}

PendingAction pending_action = PendingAction::None;

void open_menu() {
    current_category = MenuCategory::Root;
    menu_cursor = 0;
}

bool menu_go_back() {
    if (current_category == MenuCategory::Root)
        return true;
    
    current_category = MenuCategory::Root;
    menu_cursor = 0;
    return false;
}

void cancel_pending_action() {
    pending_action = PendingAction::None;
}

MenuResult handle_menu_input() {
    const auto &items = items_for(current_category);
    int count = (int)items.size();
    if (count == 0) return MenuResult::None;

    if(buttons.pressed & Button::DPAD_UP)
        menu_cursor = (menu_cursor - 1 + count) % count;
    if(buttons.pressed & Button::DPAD_DOWN)
        menu_cursor = (menu_cursor + 1) % count;

    if(buttons.pressed & Button::A) {
        const MenuItem &chosen = items[menu_cursor];

        if (chosen.is_leaf) {
            if (chosen.kind == LeafKind::OpenLogs) {
                open_menu();
                return MenuResult::OpenLogs;
            }
            pending_action = chosen.action;
            open_menu();
            return MenuResult::ActionChosen;
        }
        else {
            current_category = chosen.submenu;
            menu_cursor = 0;
        }
    }
    return MenuResult::None;
}

void draw_menu() {
    if (CURRENT_MENU != MenuState::Menu) return;

    const auto &items = items_for(current_category);

    int panel_w = 140;
    int panel_h = 20 + (int)items.size() * 14 + 10;
    int panel_x = 10;
    int panel_y = 20;

    screen.pen = Pen(0, 0, 0, 200);
    screen.rectangle(Rect(panel_x, panel_y, panel_w, panel_h));

    Point cursor(panel_x + 8, panel_y + 8);
    for (int i = 0; i < (int)items.size(); i++) {
        screen.pen = (i == menu_cursor) ? Pen(255, 220, 120) : Pen(255, 255, 255);
        std::string prefix = (i == menu_cursor) ? "> " : "  ";
        screen.text(prefix + items[i].label, minimal_font, cursor);
        cursor.y += 14;
    }
}

}