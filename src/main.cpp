#include "chip8/chip8.hpp"
#include <cstdio>
#include <exception>

int main() {
    try {
        weaver::chip8::Chip8 chip8;
        printf("chip8 constructed ok\n"); fflush(stdout);

        while (!chip8.shouldClose()) {
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
