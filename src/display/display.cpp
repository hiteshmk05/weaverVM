#include "display.hpp"

#define SDL_MAIN_HANDLED
#include <SDL.h>

weaver::display::Display::~Display() {
    close();
}

void weaver::display::Display::init(const char* title, int scale) {
    this->scale = scale;

    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        DISPLAY_WIDTH * scale, DISPLAY_HEIGHT * scale,
        SDL_WINDOW_SHOWN
    );

    renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        DISPLAY_WIDTH, DISPLAY_HEIGHT
    );
}

void weaver::display::Display::render(const DisplayBuffer& buffer) {
    uint32_t pixels[DISPLAY_WIDTH * DISPLAY_HEIGHT];

    for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; ++i) {
        pixels[i] = buffer[i] ? 0xFFFFFFFF : 0x000000FF;
    }

    SDL_UpdateTexture(texture, nullptr, pixels, DISPLAY_WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

bool weaver::display::Display::shouldClose() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) quit = true;
    }
    return quit;
}

void weaver::display::Display::close() {
    if (texture) { 
        SDL_DestroyTexture(texture);   
        texture = nullptr; 
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}
