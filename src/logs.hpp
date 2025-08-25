#pragma once
#include <vector>
#include <string>

namespace game {
    extern std::vector<std::string> logs;
    extern int log_scroll;

    inline bool show_logs = false;


    void add_log(const std::string &msg);
    void draw_logs();
    void handle_log_input();
}