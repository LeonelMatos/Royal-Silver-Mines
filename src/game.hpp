#pragma once
#include "32blit.hpp"

//Global debug flag
inline constexpr bool DEBUG = true;
inline bool MENU_OPEN = false;

enum class MenuState{
    None,
    Logs,
    Pause
};
inline MenuState CURRENT_MENU = MenuState::None;

void init();
void update(uint32_t time);
void render(uint32_t time);