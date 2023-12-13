#pragma once

#include <emscripten/emscripten.h>
#include <iostream>

namespace ui
{
    #define GRAPHICS_LOW 0;
    #define GRAPHICS_MEDIUM 1;
    #define GRAPHICS_HIGH 2;

    class Ui {
        public:
        Ui();

        private:
        int graphics_setting;
    };
} // namespace ui
