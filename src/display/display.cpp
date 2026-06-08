#include "display.hpp"

void weaver::display::Display::init(const char* title, int scale) {
    this->scale = scale;
    InitWindow(DISPLAY_WIDTH * scale, DISPLAY_HEIGHT * scale, title);
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);
}

void  weaver::display::Display::render(const weaver::DisplayBuffer& buffer){
    BeginDrawing();
    ClearBackground(BLACK);

    for(int row = 0; row < DISPLAY_HEIGHT; row++){
        for(int col = 0; col < DISPLAY_WIDTH; col++){
            if(buffer[row * DISPLAY_WIDTH + col]){
                DrawRectangle(col * scale, row * scale, scale, scale, WHITE);
            }
        }
    }

    EndDrawing();
}

bool weaver::display::Display::shouldClose() {
    return WindowShouldClose();
}
void weaver::display::Display::close(){
    CloseWindow();
}