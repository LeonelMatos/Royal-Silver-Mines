#pragma once
#include "32blit.hpp"

//Global debug flag
inline constexpr bool DEBUG = true;

enum class MenuState{
    None,
    Menu,
    Selecting,
    Inspecting,
    Logs,
    Pause

};
inline MenuState CURRENT_MENU = MenuState::None;

inline const char *menu_state_to_str(MenuState s) {
    switch(s) {
        case MenuState::None: return "None";
        case MenuState::Menu: return "Menu";
        case MenuState::Selecting: return "Selecting";
        case MenuState::Inspecting: return "Inspecting";
        case MenuState::Logs: return "Logs";
        case MenuState::Pause: return "Pause";
        default:              return "Err";
    }
}

void init();
void update(uint32_t time);
void render(uint32_t time);