#pragma once

#include <cstdint>
#include <string>
#include "../display/display.hpp"

namespace weaver {
    class VirtualMachine {
    public:
        virtual void loadROM(const std::string& path) = 0;
        virtual void cycle() = 0;
        virtual void updateTimers() = 0;
        virtual void keyDown(uint8_t key) = 0;
        virtual void keyUp(uint8_t key) = 0;
        virtual void reset() = 0;
        bool shouldClose() { return screen.shouldClose(); }
        void close() { screen.close(); }
        virtual ~VirtualMachine() = default;

    protected:
        display::Display screen;
    };
}
