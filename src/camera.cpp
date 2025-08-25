#include "camera.hpp"

namespace game {

Camera::Camera(int vw, int vh) : x(0), y(0), view_w(vw), view_h(vh), speed(2.0f) {}

void Camera::move(float dx, float dy, int map_px_w, int map_px_h) {
    x += dx;
    y += dy;

    float max_x = map_px_w - view_w;
    float max_y = map_px_h - view_h;
    if(x < 0) x = 0;
    if(y < 0) y = 0;
    if(max_x < 0) max_x = 0;
    if(max_y < 0) max_y = 0;
    if(x > max_x) x = max_x;
    if(y > max_y) y = max_y;
}
}