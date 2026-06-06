#pragma once

#include <cstdint>
#include <array>

#include "raylib.h"

namespace weaver{
    inline constexpr int DISPLAY_WIDTH = 64;
    inline constexpr int DISPLAY_HEIGHT = 32;

    using DisplayBuffer = std::array<uint8_t, DISPLAY_HEIGHT * DISPLAY_WIDTH>;

    namespace display{
        class Display {

        public:
            void init(const char* title, int scale);
            void render(const DisplayBuffer& buffer);
            bool shouldClose();
            void close();

        private:
            int scale;
        };
    }
}