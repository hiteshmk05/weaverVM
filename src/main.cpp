#include "chip8/chip8.hpp"
#include "input/input.hpp"
#include <cstdio>
#include <exception>

int main() {
    try {
        weaver::chip8::Chip8 chip8;
        weaver::input::Input input;

        chip8.loadROM("../roms/Pong.ch8");

        while (!chip8.shouldClose()) {
            input.update();

            for (const auto& mapping : weaver::chip8::KEYMAP) {
                if (input.pressed(mapping.physical))  chip8.keyDown(mapping.chip8Key);
                if (input.released(mapping.physical)) chip8.keyUp(mapping.chip8Key);
            }

            chip8.cycle();
            chip8.updateTimers();
        }

        chip8.close();
    }
    catch (const std::exception& e) {
        printf("EXCEPTION: %s\n", e.what()); fflush(stdout);
    }
    catch (...) {
        printf("UNKNOWN EXCEPTION\n"); fflush(stdout);
    }

    printf("program ended\n"); fflush(stdout);
    return 0;
}