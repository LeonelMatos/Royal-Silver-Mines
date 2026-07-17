#pragma once
#include <vector>
#include <string>
#include <cstdint>

namespace game {

enum class MenuCategory : uint8_t {
    Root,
    Build,
    Carve,
    Manage
};

//Pending player action to confirm in selecting
enum class PendingAction : uint8_t {
    None,
    CarveFloor,
    CarveShaft,
    BuildWall,
    BuildStorage,
    BuildHousing,
    ManageAssignWorker,
    ManageSetPriority
};

enum class LeafKind : uint8_t {
    Action,     //apply a PendingAction via the selecting state
    OpenLogs    //jump straight to Logs
};

enum class MenuResult : uint8_t {
    None,       //no state change needed
    OpenLogs,   //player picked Logs from root
    ActionChosen //player picked a leaf action, move to Selecting
};

struct MenuItem {
    std::string label;
    bool is_leaf;
    LeafKind kind;      //only if is_leaf
    PendingAction action; //only if kind == Action
    MenuCategory submenu; //only if !is_leaf
};

extern PendingAction pending_action;

//reset to Root
void open_menu();
//D-pad nav + A select
MenuResult handle_menu_input();
//returns true if this closed menu was Root
bool menu_go_back();
//called when Selecting is cancelled with B-button
void cancel_pending_action();
void draw_menu();

}