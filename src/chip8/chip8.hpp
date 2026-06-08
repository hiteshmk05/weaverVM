#pragma once

#include <cstdint>
#include <string>
#include <array>
#include <functional>
#include <unordered_map>

#include "core/vm_interface.hpp"
#include "input/input.hpp"


namespace weaver::chip8 {

// Maps a physical key to the CHIP-8 hex keypad value (0x0-0xF) it represents,
// following the conventional 1234/QWER/ASDF/ZXCV -> 123C/456D/789E/A0BF layout.
struct KeyMapping {
    input::PhysicalKey physical;
    uint8_t chip8Key;
};

inline constexpr std::array<KeyMapping, 16> KEYMAP = {{
    {input::PhysicalKey::One,   0x1}, {input::PhysicalKey::Two, 0x2}, {input::PhysicalKey::Three, 0x3}, {input::PhysicalKey::Four, 0xC},
    {input::PhysicalKey::Q,     0x4}, {input::PhysicalKey::W,   0x5}, {input::PhysicalKey::E,     0x6}, {input::PhysicalKey::R,    0xD},
    {input::PhysicalKey::A,     0x7}, {input::PhysicalKey::S,   0x8}, {input::PhysicalKey::D,     0x9}, {input::PhysicalKey::F,    0xE},
    {input::PhysicalKey::Z,     0xA}, {input::PhysicalKey::X,   0x0}, {input::PhysicalKey::C,     0xB}, {input::PhysicalKey::V,    0xF},
}};

class Chip8 final : public weaver::VirtualMachine{
    public:
        Chip8();

        void loadROM(const std::string& path) override;
        void cycle() override;
        void updateTimers() override;
        void keyDown(uint8_t key) override;
        void keyUp(uint8_t key) override;
        void reset() override;

    private:

        std::array<uint8_t,4096> memory{};
        std::array<uint8_t,16> V{};             // registers
        std::array<uint16_t,16> stack{};
        uint16_t PC = 0x200;                    // program counter
        uint16_t I  = 0;
        uint8_t  SP = 0;                        // stack pointer
        uint8_t DT = 0;                         // delay timer
        uint8_t ST = 0;                         // sound timer
        std::array<bool,16> keys{};             // hex keypad state (0x0-0xF)
        DisplayBuffer displayBuffer{};          // 64x32 monochrome pixel buffer

        struct DecodedOp {
            uint8_t type;
            uint8_t x;
            uint8_t y;
            uint8_t n;
            uint8_t nn;
            uint16_t nnn;
        };

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

        using opcodeHandler = std::function<void(uint16_t)>;

        std::array<opcodeHandler, 16> table_main;
        std::array<opcodeHandler, 16> table_8;
        std::unordered_map<uint8_t, opcodeHandler> table_E;
        std::unordered_map<uint8_t, opcodeHandler> table_F;

        uint16_t fetch();
        DecodedOp decode(uint16_t opcode);
        void execute(uint16_t opcode);

        void init_tables();
        void dispatch_0(uint16_t op);
        void dispatch_8(uint16_t op);
        void dispatch_E(uint16_t op);
        void dispatch_F(uint16_t op);

        // primary table handlers
        void JP(uint16_t op);
        void CALL(uint16_t op);
        void SE_VX_KK(uint16_t op);
        void SNE_VX_KK(uint16_t op);
        void SE_VX_VY(uint16_t op);
        void LD_VX_KK(uint16_t op);
        void ADD_VX_KK(uint16_t op);
        void SNE_VX_VY(uint16_t op);
        void LD_I(uint16_t op);
        void JP_V0(uint16_t op);
        void RND(uint16_t op);
        void DRW(uint16_t op);

        // group 8
        void LD_VX_VY(uint16_t op);
        void OR(uint16_t op);
        void AND(uint16_t op);
        void XOR(uint16_t op);
        void ADD_VX_VY(uint16_t op);
        void SUB(uint16_t op);
        void SHR(uint16_t op);
        void SUBN(uint16_t op);
        void SHL(uint16_t op);

        // group E
        void SKP(uint16_t op);
        void SKNP(uint16_t op);

        // group F
        void LD_VX_DT(uint16_t op);
        void LD_VX_KEY(uint16_t op);
        void LD_DT_VX(uint16_t op);
        void LD_ST_VX(uint16_t op);
        void ADD_I_VX(uint16_t op);
        void LD_F_VX(uint16_t op);
        void LD_BCD(uint16_t op);
        void STORE_REGS(uint16_t op);
        void LOAD_REGS(uint16_t op);
};

} //namespace weaver::chip8
