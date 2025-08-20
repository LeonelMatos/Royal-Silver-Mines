#include "ui.hpp"
#include "32blit.hpp"
#include "tiles.hpp"

using namespace blit;

namespace game {
    
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

        draw_input_debug();
    }

    void draw_ui(const Map &map, const Camera &cam) {
        draw_debug_ui();

        //-MINIMAP-
        const int mm_w = 80;
        const int mm_h = 40;
        const int mm_x = cam.view_w - mm_w - 4;
        const int mm_y = cam.view_h - mm_h - 4;
        screen.pen = Pen(60,60,60);
        screen.rectangle(Rect(mm_x, mm_y, mm_w, mm_h));

        float map_px_w = map.width * TILE_SIZE;
        float map_px_h = map.height * TILE_SIZE;
        float vx = (cam.x / map_px_w) * mm_w;
        float vy = (cam.y / map_px_h) * mm_h;
        float vw = (float)cam.view_w / map_px_w * mm_w;
        float vh = (float)cam.view_h / map_px_h * mm_h;
        screen.pen = Pen(200,200,200);
        screen.rectangle(Rect(mm_x + int(vx), mm_y + int(vy), int(vw), int(vh)));
    }



}


