#include "game.hpp"
#include "map.hpp"
#include "camera.hpp"
#include "tiles.hpp"
#include "ui.hpp"
#include "logs.hpp"
#include "menu.hpp"

using namespace blit;

using namespace game;

//Basic procedural generated map + camera
//tile size 12x12

static game::Map world_map(128, 128);
static game::Camera cam(0, 0);

void init() {
    set_screen_mode(ScreenMode::hires);
    game::load_tileset();
    world_map.generate();

    cam = game::Camera(screen.bounds.w, screen.bounds.h);
    cam.x = (world_map.width / 2) * TILE_SIZE - cam.view_w / 2;
    cam.y = 0;
    

    game::add_log("Once upon a time there was a lovely princess. But she had an enchantment upon her of a fearful sort which could only be broken by love's first kiss. She was locked away in a castle guarded by a terrible fire-breathing dragon. Many brave knigts had attempted to free her from this dreadful prison, but non prevailed. She waited in the dragon's keep in the highest room of the tallest tower for her true love and true love's first kiss. {Laughing} Like that's ever gonna happen. {Paper Rusting, Toilet Flushes} What a load of - Somebody once told me the world is gonna roll me I ain't the sharpest tool in the shed");

}

void render(uint32_t time) {
    screen.pen = Pen(0,0,0);
    screen.clear();

    // compute visible tile bounds
    int tx0 = cam.x / TILE_SIZE;
    int ty0 = cam.y / TILE_SIZE;
    int tx1 = (cam.x + cam.view_w) / TILE_SIZE + 1;
    int ty1 = (cam.y + cam.view_h) / TILE_SIZE + 1;

    static int last_tx0 = -1, last_ty0 = -1;
    if (tx0 != last_tx0 || ty0 != last_ty0) {
        world_map.reveal(tx0, ty0, tx1, ty1);
        last_tx0 = tx0;
        last_ty0 = ty0;
    }

    for(int ty = ty0; ty <= ty1; ty++) {
    for(int tx = tx0; tx <= tx1; tx++) {
        uint8_t t = world_map.tile_at(tx, ty);
        int sx = int(tx * TILE_SIZE - cam.x);
        int sy = int(ty * TILE_SIZE - cam.y);
        draw_tile(sx, sy, tx, ty, t, world_map.is_discovered(tx, ty));
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
        
        if (buttons.pressed & Button::X) {
            game::open_menu();
            CURRENT_MENU = MenuState::Menu;
        }
        if (buttons.pressed & Button::B) {
            CURRENT_MENU = MenuState::Inspecting;
        }
    }
    if (buttons.pressed & Button::Y) {
        switch(CURRENT_MENU) {
            case MenuState::Menu:
                if(game::menu_go_back())
                    CURRENT_MENU = MenuState::None;
                break;
            case MenuState::Selecting: 
                game::cancel_pending_action();
                CURRENT_MENU = MenuState::None;
                break;
            case MenuState::Inspecting:
                CURRENT_MENU = MenuState::None;
                break;
            case MenuState::Logs:
                CURRENT_MENU = MenuState::None;
                break;
            default: break;
        }
    }

    switch(CURRENT_MENU) {
        case MenuState::None:
            break;
        case MenuState::Menu: {
            game::MenuResult r = game::handle_menu_input();
            if (r == game::MenuResult::OpenLogs)
                CURRENT_MENU = MenuState::Logs;
            else if (r == game::MenuResult::ActionChosen)
                CURRENT_MENU = MenuState::Selecting;
            break;
        }
        case MenuState::Selecting:
            break;
        case MenuState::Inspecting:
            break;
        case MenuState::Logs:
            game::handle_log_input();
            break;
        default: break;
    }
}