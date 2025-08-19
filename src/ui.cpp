#include "ui.hpp"
#include "32blit.hpp"
#include "tiles.hpp"

using namespace blit;

namespace game {
    
    void draw_ui(const Map &map, const Camera &cam) {
        //-HUD TEXT-
        screen.pen = Pen(255,255,255);
        screen.text("Silver Mines", minimal_font, Point(4,4));
        screen.text("TILE_SIZE=" + std::to_string(TILE_SIZE), minimal_font, Point(4,16));


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


