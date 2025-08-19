#pragma once

namespace game {
struct Camera {
    float x,y;
    int view_w, view_h;

    Camera(int vw, int vh);
    void move(float dx, float dy, int map_px_w, int map_px_h);
};

}