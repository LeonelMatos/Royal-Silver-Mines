#include "game.hpp"
#include "map.hpp"
#include "camera.hpp"
#include "tiles.hpp"
#include "ui.hpp"
#include "logs.hpp"

using namespace blit;

using namespace game;

//Basic procedural generated map + camera
//tile size 12x12

static game::Map world_map(128, 64);
static game::Camera cam(0, 0);

void init() {
    set_screen_mode(ScreenMode::hires);
    world_map.generate();

    cam = game::Camera(screen.bounds.w, screen.bounds.h);
    cam.x = (world_map.width / 2) * TILE_SIZE - cam.view_w / 2;
    cam.y = 0;
    

    game::add_log("Lorem ipsum dolor sit amet consectetur adipiscing elit. Quisque faucibus ex sapien vitae pellentesque sem placerat. In id cursus mi pretium tellus duis convallis. Tempus leo eu aenean sed diam urna tempor. Pulvinar vivamus fringilla lacus nec metus bibendum egestas. Iaculis massa nisl malesuada lacinia integer nunc posuere. Ut hendrerit semper vel class aptent taciti sociosqu. Ad litora torquent per conubia nostra inceptos himenaeos. " \
        "Lorem ipsum dolor sit amet consectetur adipiscing elit. Quisque faucibus ex sapien vitae pellentesque sem placerat. In id cursus mi pretium tellus duis convallis. Tempus leo eu aenean sed diam urna tempor. Pulvinar vivamus fringilla lacus nec metus bibendum egestas. Iaculis massa nisl malesuada lacinia integer nunc posuere. Ut hendrerit semper vel class aptent taciti sociosqu. Ad litora torquent per conubia nostra inceptos himenaeos." \
    "Lorem ipsum dolor sit amet consectetur adipiscing elit. Quisque faucibus ex sapien vitae pellentesque sem placerat. In id cursus mi pretium tellus duis convallis. Tempus leo eu aenean sed diam urna tempor. Pulvinar vivamus fringilla lacus nec metus bibendum egestas. Iaculis massa nisl malesuada lacinia integer nunc posuere. Ut hendrerit semper vel class aptent taciti sociosqu. Ad litora torquent per conubia nostra inceptos himenaeos.");

}

void render(uint32_t time) {
    screen.pen = Pen(0,0,0);
    screen.clear();

    // compute visible tile bounds
    int tx0 = cam.x / TILE_SIZE;
    int ty0 = cam.y / TILE_SIZE;
    int tx1 = (cam.x + cam.view_w) / TILE_SIZE + 1;
    int ty1 = (cam.y + cam.view_h) / TILE_SIZE + 1;

    for(int ty = ty0; ty <= ty1; ty++) {
    for(int tx = tx0; tx <= tx1; tx++) {
        uint8_t t = world_map.tile_at(tx, ty);
        int sx = int(tx * TILE_SIZE - cam.x);
        int sy = int(ty * TILE_SIZE - cam.y);
        draw_tile(sx, sy, t);
    }
    }

    draw_ui(world_map, cam);
}

void update(uint32_t time) {
    //camera pan with D-pad
    if (CURRENT_MENU == MenuState::None) {
        if(pressed(Button::DPAD_LEFT))  cam.move(-cam.speed, 0, world_map.width * TILE_SIZE, world_map.height * TILE_SIZE);
        if(pressed(Button::DPAD_RIGHT)) cam.move(cam.speed, 0,  world_map.width * TILE_SIZE, world_map.height * TILE_SIZE);
        if(pressed(Button::DPAD_UP))    cam.move(0, -cam.speed, world_map.width * TILE_SIZE, world_map.height * TILE_SIZE);
        if(pressed(Button::DPAD_DOWN))  cam.move(0, cam.speed,  world_map.width * TILE_SIZE, world_map.height * TILE_SIZE);
        
    }

    //Menu toggling
    if(buttons.pressed & Button::Y) {
        if(CURRENT_MENU == MenuState::Logs)
            CURRENT_MENU = MenuState::None;
        else
            CURRENT_MENU = MenuState::Logs;
    }

    // If in logs menu, delegate input
    if(CURRENT_MENU == MenuState::Logs) {
        game::handle_log_input();
    }
}