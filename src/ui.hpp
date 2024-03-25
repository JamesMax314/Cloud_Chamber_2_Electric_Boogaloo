#pragma once

#include <emscripten/emscripten.h>
#include <iostream>
#include <thread>

namespace ui
{
    #define GRAPHICS_LOW 0;
    #define GRAPHICS_MEDIUM 1;
    #define GRAPHICS_HIGH 2;

    #define PAUSE 0;
    #define PLAY 1;

    #define FREEMOVE_OFF 0;
    #define FREEMOVE_ON 1;

    class Ui {
        public:
        Ui();

        void poll_all();

        void poll_graphics_setting();
        void poll_free_move();
        void poll_play_pause();

        void set_graphics_setting(int setting);
        int get_graphics_setting();

        int get_free_move_setting();

        int get_play_pause_setting();

        private:
        int graphics_setting;
        int free_move_setting;
        int pause_play_setting;

    };
} // namespace ui
