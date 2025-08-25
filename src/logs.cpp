#include "logs.hpp"
#include "32blit.hpp"
#include <algorithm>
#include <sstream>

using namespace blit;

namespace game {

    std::vector<std::string> logs;
    int log_scroll = 0;

    void add_log(const std::string &msg) {
        int panel_w = screen.bounds.w - 20;
        int char_w = 6;
        int wrap_width = panel_w / char_w;

        std::istringstream words(msg);
        std::string word, line;

        while(words >> word) {
            if(line.empty())
                line = word;
            else if(line.size() + 1 + word.size() <= (size_t)wrap_width)
                line += " " + word;
            else {
                logs.push_back(line);
                line = word;
            }
        }
        if(!line.empty())
            logs.push_back(line);

        logs.push_back("—");
    }

    void handle_log_input() {
        if (buttons & Button::DPAD_UP) {
            if(log_scroll > 0) log_scroll--;
        }
        if (buttons & Button::DPAD_DOWN) {
            if(log_scroll < (int)logs.size()-1) log_scroll++;
        }
    }
}