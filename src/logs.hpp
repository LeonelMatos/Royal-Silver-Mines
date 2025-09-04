#pragma once
#include <vector>
#include <string>

namespace game {
    extern std::vector<std::string> logs;
    extern int log_scroll;


    void add_log(const std::string &msg);
    void draw_logs();
    void handle_log_input();
}