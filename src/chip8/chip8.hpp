#pragma once

#include <cstdint>
#include <string>
#include <array>

#include "src/core/vm_interface.hpp"

namespace mkvm::chip8 {

class Chip8 final : public mkvm::VirtualMachine{
    public: 
        Chip8();

        void loadROM(const std::string& path) override;
        void cycle() override;
        void updateTimers() override;
        void keyDown(uint8_t key) override;
        void keyUp(uint8_t key) override;
        const mkvm::DisplayBuffer& getDisplay() const override;
        bool displayChanged() const override;
        void reset() override;

    private:

        uint16_t fetch();
        void execute(uint16_t opcode);

        // CPU registers
        std::array<uint8_t,4096> memory{};
        std::array<uint8_t,16> V{};
        std::array<uint16_t,16> stack{};
        uint16_t PC = 0x200;
        uint16_t I  = 0;
        uint8_t  SP = 0;

        static constexpr std::array<uint8_t, 80> FONT = {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };


};

} //namespace mkvm::chip8

