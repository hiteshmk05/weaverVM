#pragma once

#include <cstdint>
#include <array>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace weaver{
    inline constexpr int DISPLAY_WIDTH = 64;
    inline constexpr int DISPLAY_HEIGHT = 32;

    using DisplayBuffer = std::array<uint8_t, DISPLAY_HEIGHT * DISPLAY_WIDTH>;

    namespace display{
        class Display {

        public:
            ~Display();

            void init(const char* title, int scale);
            void render(const DisplayBuffer& buffer);
            bool shouldClose();
            void close();

        private:
            int scale = 1;
            bool quit = false;
            SDL_Window* window = nullptr;
            SDL_Renderer* renderer = nullptr;
            SDL_Texture* texture = nullptr;
        };
    }
}
