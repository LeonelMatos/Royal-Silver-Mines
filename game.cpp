#include "game.hpp"

using namespace blit;

//Basic procedural generated map + camera
//tile size 12x12

//MAP
const int TILE_SIZE = 12;
const int MAP_W = 128;
const int MAP_H = 64;

enum Tile : uint8_t {
    TILE_EMPTY,
    TILE_FLOOR,
    TILE_WALL,
    TILE_WOOD
};

static std::vector<uint8_t> map_data;

//CAMERA
static float cam_x = 0.0f;
static float cam_y = 0.0f;
static int view_w = 240;
static int view_h = 240;

//generation params
static const int FILL_PROB = 60; //def:45 for wide
static const int SMOOTH_ITERS = 5;

void generate_cave_map() {
    map_data.assign(MAP_W * MAP_H, TILE_WALL);

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, 99);

    // random fill
    for(int y = 1; y < MAP_H - 1; y++) {
        for(int x = 1; x < MAP_W - 1; x++) {
        int i = y * MAP_W + x;
        if(dist(rng) < FILL_PROB) map_data[i] = TILE_WALL;
        else map_data[i] = TILE_FLOOR;
        }
    }

    //carve a vertical entrance shaft
    int entrance_x = MAP_W / 2;
    for(int y = 0; y < 8; y++) map_data[y * MAP_W + entrance_x] = TILE_FLOOR;
    for(int y = 0; y < 4; y++) map_data[(y+8) * MAP_W + entrance_x] = TILE_WOOD;

    // smooth with cellular automata
    for(int iter = 0; iter < SMOOTH_ITERS; iter++) {
        std::vector<uint8_t> next = map_data;
        for(int y = 1; y < MAP_H - 1; y++) {
        for(int x = 1; x < MAP_W - 1; x++) {
            int walls = 0;
            for(int oy = -1; oy <= 1; oy++) {
            for(int ox = -1; ox <= 1; ox++) {
                if(ox == 0 && oy == 0) continue;
                int nx = x + ox;
                int ny = y + oy;
                if(map_data[ny * MAP_W + nx] == TILE_WALL) walls++;
            }
            }
            int idx = y * MAP_W + x;
            if(walls >= 5) next[idx] = TILE_WALL;
            else next[idx] = TILE_FLOOR;
        }
        }
        map_data.swap(next);
    }
    // Simple post-pass: make sure edges are walls
    for(int x = 0; x < MAP_W; x++) {
        map_data[x] = TILE_WALL;
        map_data[(MAP_H-1)*MAP_W + x] = TILE_WALL;
    }
    for(int y = 0; y < MAP_H; y++) {
        map_data[y*MAP_W + 0] = TILE_WALL;
        map_data[y*MAP_W + (MAP_W-1)] = TILE_WALL;
    }
}

inline uint8_t tile_at(int tx, int ty) {
    if(tx < 0 || ty < 0 || tx >= MAP_W || ty >= MAP_H) return TILE_WALL;
    return map_data[ty * MAP_W + tx];
}

void draw_tile(int sx, int sy, uint8_t tile) {
    switch(tile) {
        case TILE_FLOOR:
        screen.pen = Pen(90, 90, 90); // grey stone floor
        screen.rectangle(Rect(sx, sy, TILE_SIZE, TILE_SIZE));
        break;
        case TILE_WALL:
        screen.pen = Pen(40, 40, 40); // darker stone
        screen.rectangle(Rect(sx, sy, TILE_SIZE, TILE_SIZE));
        break;
        case TILE_WOOD:
        screen.pen = Pen(120, 70, 30); // wood / support beams
        screen.rectangle(Rect(sx, sy, TILE_SIZE, TILE_SIZE));
        break;
        default:
        // empty/void
        screen.pen = Pen(0, 0, 0);
        screen.rectangle(Rect(sx, sy, TILE_SIZE, TILE_SIZE));
        break;
    }
}

void init() {
    set_screen_mode(ScreenMode::hires);
    view_w = screen.bounds.w;
    view_h = screen.bounds.h;

    generate_cave_map();

    cam_x = (MAP_W/2) * TILE_SIZE - view_w / 2;
    cam_y = 0;
}

void render(uint32_t time) {
    screen.pen = Pen(0,0,0);
    screen.clear();
    
    // compute visible tile bounds
    int tx0 = std::max(0, int(cam_x) / TILE_SIZE);
    int ty0 = std::max(0, int(cam_y) / TILE_SIZE);
    int tx1 = std::min(MAP_W - 1, int((cam_x + view_w) / TILE_SIZE) + 1);
    int ty1 = std::min(MAP_H - 1, int((cam_y + view_h) / TILE_SIZE) + 1);

    for(int ty = ty0; ty <= ty1; ty++) {
    for(int tx = tx0; tx <= tx1; tx++) {
        uint8_t t = tile_at(tx, ty);
        int sx = int(tx * TILE_SIZE - cam_x);
        int sy = int(ty * TILE_SIZE - cam_y);

        draw_tile(sx, sy, t);
    }
    }

    //debug HUD
    screen.pen = Pen(255,255,255);
    screen.text("Mine map example", minimal_font, Point(4,4));
    screen.text("TILE_SIZE=" + std::to_string(TILE_SIZE), minimal_font, Point(4,16));

    //minimap
    const int mm_w = 80;
    const int mm_h = 40;
    const int mm_x = view_w - mm_w - 4;
    const int mm_y = view_h - mm_h - 4;
    screen.pen = Pen(60,60,60);
    screen.rectangle(Rect(mm_x, mm_y, mm_w, mm_h));

    float map_px_w = MAP_W * TILE_SIZE;
    float map_px_h = MAP_H * TILE_SIZE;
    float vx = (cam_x / map_px_w) * mm_w;
    float vy = (cam_y / map_px_h) * mm_h;
    float vw = (float)view_w / map_px_w * mm_w;
    float vh = (float)view_h / map_px_h * mm_h;
    screen.pen = Pen(200,200,200);
    screen.rectangle(Rect(mm_x + int(vx), mm_y + int(vy), int(vw), int(vh)));
}

void update(uint32_t time) {
    //camera pan with D-pad
    const float speed = 2.0f;
    if(pressed(Button::DPAD_LEFT)) cam_x -= speed;
    if(pressed(Button::DPAD_RIGHT)) cam_x += speed;
    if(pressed(Button::DPAD_UP)) cam_y -= speed;
    if(pressed(Button::DPAD_DOWN)) cam_y += speed;

    //clamp camera to map bounds
    float max_cam_x = MAP_W * TILE_SIZE - view_w;
    float max_cam_y = MAP_H * TILE_SIZE - view_h;
    if(cam_x < 0) cam_x = 0;
    if(cam_y < 0) cam_y = 0;
    if(cam_x > max_cam_x) cam_x = max_cam_x;
    if(cam_y > max_cam_y) cam_y = max_cam_y;
}