#pragma once
#include "32blit.hpp"

//Global debug flag
inline constexpr bool DEBUG = true;
inline bool MENU_OPEN = false;

void init();
void update(uint32_t time);
void render(uint32_t time);