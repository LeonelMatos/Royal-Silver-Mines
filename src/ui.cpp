#include "ui.hpp"
#include "32blit.hpp"
#include "tiles.hpp"
#include "logs.hpp"
#include "menu.hpp"

using namespace blit;

namespace game {

    int get_max_log_lines() {
        int panel_h = screen.bounds.h - 40;
        int margin = 10;
        int line_h = 10;
        return std::max(1, (panel_h - margin * 2) / line_h);
    }

    void draw_input_debug() {
        int size = 10;
        int margin = 3;

        int start_x = margin;
        int start_y = screen.bounds.h - (2 * (size + margin)) - margin;

        struct Btn {
            blit::Button button;
            Pen color;
            int ox, oy;
        };

        //  [Y][X]
        //  [B][A]
        Btn buttons[] = {
            { blit::Button::Y, Pen(0,255,0),    0,0},
            { blit::Button::X, Pen(255,0,0),    1,0},
            { blit::Button::B, Pen(255,255,0),  0,1},
            { blit::Button::A, Pen(0, 0, 255),  1,1},
        };
        for (auto &b : buttons) {
            if (blit::buttons & b.button)
                screen.pen = b.color;
            else
                screen.pen = Pen(40,40,40); //Not pressed
            screen.rectangle(Rect(
                start_x + b.ox * (size + margin),
                start_y + b.oy * (size + margin),
                size, size
            ));
        } 
    }

    void draw_debug_ui() {
        if(!DEBUG) return;
        //-HUD TEXT-
        screen.pen = Pen(255,255,255);
        screen.text("Silver Mines", minimal_font, Point(4,4));
        screen.text("TILE_SIZE=" + std::to_string(TILE_SIZE), minimal_font, Point(4,16));
        screen.text("Menu=" + std::string(menu_state_to_str(CURRENT_MENU)), minimal_font, Point(4,28));

        draw_input_debug();
    }

    Pen minimap_tile_color(uint8_t tile) {
        switch(tile) {
            case TILE_FLOOR: return Pen(90, 90, 90);
            case TILE_WALL: return Pen(40, 40, 40);
            case TILE_WOOD: return Pen(120, 70, 30);
            case TILE_GRASS: return Pen(60, 130, 60);
            case TILE_DIRT: return Pen(110, 90, 60);
            default: return Pen(0, 0, 0);
        }
    }

    void draw_minimap(const Map &map, const Camera &cam) {
        #ifdef PICO_BUILD
            const int mm_w = cam.view_w / 3;
            const int mm_h = cam.view_h / 3;
        #else
            const int mm_w = 80;
            const int mm_h = 40;
        #endif
        const int mm_x = cam.view_w - mm_w - 4;
        const int mm_y = cam.view_h - mm_h - 4;

        //terrain overview sampling the full map
        for (int py = 0; py < mm_h; py++) {
            int ty0 = (py * map.height) / mm_h;
            int ty1 = ((py + 1) * map.height) / mm_h;
            if (ty1 <= ty0) ty1 = ty0 + 1;

            for(int px = 0; px < mm_w; px++) {
                int tx0 = (px * map.width) / mm_w;
                int tx1 = ((px + 1) * map.width) / mm_w;
                if (tx1 <= tx0) tx1 = tx0 + 1;

                //scans the whole block this minimap pixel represents
                bool found_open = false, found_wall = false;
                uint8_t open_tile = TILE_FLOOR;
                for (int ty = ty0; ty < ty1; ty++) {
                    for (int tx = tx0; tx < tx1; tx++) {
                        if (!map.is_discovered(tx, ty)) continue;
                        uint8_t t = map.tile_at(tx, ty);
                        if (t == TILE_WALL) found_wall = true;
                        else {
                            found_open = true;
                            open_tile = t;
                        }
                    }
                }

                screen.pen = found_open ? minimap_tile_color(open_tile) : found_wall ? minimap_tile_color(TILE_WALL) : Pen(0, 0, 0);
                screen.rectangle(Rect(mm_x + px, mm_y + py, 1, 1));
            }
        }

        float map_px_w = map.width * TILE_SIZE;
        float map_px_h = map.height * TILE_SIZE;
        float vx = (cam.x / map_px_w) * mm_w;
        float vy = (cam.y / map_px_h) * mm_h;
        float vw = (float)cam.view_w / map_px_w * mm_w;
        float vh = (float)cam.view_h / map_px_h * mm_h;
        screen.pen = Pen(200,200,200);
        screen.rectangle(Rect(mm_x + int(vx), mm_y + int(vy), int(vw), int(vh)));
    }

    void draw_logs() {
        if(CURRENT_MENU != MenuState::Logs) return;

        int panel_w = screen.bounds.w - 20;
        int panel_h = screen.bounds.h - 40;
        int panel_x = 10;
        int panel_y = 20;
        Rect panel(panel_x, panel_y, panel_w, panel_h);

        //background
        screen.pen = Pen(0,0,0, 200);
        screen.rectangle(panel);

        screen.pen = Pen(255,255,255);
        
        //margin
        int margin = 10;
        Point cursor(panel_x + margin, panel_y + margin);

        int line_height = 10;
        int max_lines = get_max_log_lines();

        int total_lines = logs.size();
        int max_scroll = std::max(0, total_lines - max_lines);

        if(log_scroll < 0) log_scroll = 0;
        if(log_scroll > max_scroll) log_scroll = max_scroll;

        int start_index = log_scroll;
        int end_index = std::min(total_lines, start_index + max_lines);


        //draw text
        screen.pen = Pen(255,255,255);

        for(int i = start_index; i < end_index; i++) {
            screen.text(logs[i], minimal_font, cursor);
            cursor.y += line_height;
        }

        //scrollbar
        if(total_lines > max_lines) {
            float ratio = (float)max_lines / total_lines;
            int bar_h = std::max(10, int(panel_h * ratio));
            float scroll_ratio = (float)log_scroll / (total_lines - max_lines);
            int bar_y = panel_y + int((panel_h - bar_h) * scroll_ratio);

            screen.pen = Pen(200,200,200);
            screen.rectangle(Rect(panel_x + panel_w - 6, bar_y, 4, bar_h));
        }
    }

    void draw_ui(const Map &map, const Camera &cam) {
        
        //-MINIMAP-
        if(CURRENT_MENU == MenuState::None)
        draw_minimap(map, cam);
        
        draw_menu();
        draw_logs();
        draw_debug_ui();
    }

}


