#pragma once

#include <cstdint>
#include <string>
#include <array>

namespace mkvm{
    inline constexpr int DISPLAY_WIDTH = 64;
    inline constexpr int DISPLAY_HEIGHT = 32;

    using DisplayBuffer = std::array<uint8_t, DISPLAY_HEIGHT*DISPLAY_WIDTH>;

    class VirtualMachine{
        public:
            virtual void loadROM(const std::string& path) = 0;
            virtual void cycle() = 0;
            virtual void updateTimers() = 0;
            virtual void keyDown(uint8_t key) = 0;
            virtual void keyUp(uint8_t key) = 0;
            virtual const DisplayBuffer& getDisplay() const = 0; 
            virtual bool displayChanged() const = 0 ;
            virtual void reset() = 0;
            virtual ~VirtualMachine() = default;
        
        protected:
            DisplayBuffer display{};
            bool display_dirty = false; 
    };
} // namespace mkvm 