#include "chip8.hpp"

#include <fstream>
#include <stdexcept>
#include <algorithm>

namespace weaver::chip8 {

Chip8::Chip8(){
    reset();
}

void Chip8::reset(){
    memory.fill(0);
    V.fill(0);
    stack.fill(0);
    PC = 0x200;
    I  = 0;
    SP = 0;
    DT = 0;
    ST = 0;

    display_buffer.fill(0);
    display_dirty = false;

    std::copy(FONT.begin(), FONT.end(), memory.begin() + 0x050);
}

void Chip8::init_tables() {
    // ----------------------------------------------------------
    // table_main — top nibble (op >> 12)
    // ----------------------------------------------------------
    table_main[0x0] = [this](uint16_t op){ dispatch_0(op); };
    table_main[0x1] = [this](uint16_t op){ JP(op); };
    table_main[0x2] = [this](uint16_t op){ CALL(op); };
    table_main[0x3] = [this](uint16_t op){ SE_VX_KK(op); };
    table_main[0x4] = [this](uint16_t op){ SNE_VX_KK(op); };
    table_main[0x5] = [this](uint16_t op){ SE_VX_VY(op); };
    table_main[0x6] = [this](uint16_t op){ LD_VX_KK(op); };
    table_main[0x7] = [this](uint16_t op){ ADD_VX_KK(op); };
    table_main[0x8] = [this](uint16_t op){ dispatch_8(op); };
    table_main[0x9] = [this](uint16_t op){ SNE_VX_VY(op); };
    table_main[0xA] = [this](uint16_t op){ LD_I(op); };
    table_main[0xB] = [this](uint16_t op){ JP_V0(op); };
    table_main[0xC] = [this](uint16_t op){ RND(op); };
    table_main[0xD] = [this](uint16_t op){ DRW(op); };
    table_main[0xE] = [this](uint16_t op){ dispatch_E(op); };
    table_main[0xF] = [this](uint16_t op){ dispatch_F(op); };

    // ----------------------------------------------------------
    // table_8 — last nibble (op & 0x000F)
    // ----------------------------------------------------------
    auto noop = [](uint16_t){};

    table_8[0x0] = [this](uint16_t op){ LD_VX_VY(op); };
    table_8[0x1] = [this](uint16_t op){ OR(op); };
    table_8[0x2] = [this](uint16_t op){ AND(op); };
    table_8[0x3] = [this](uint16_t op){ XOR(op); };
    table_8[0x4] = [this](uint16_t op){ ADD_VX_VY(op); };
    table_8[0x5] = [this](uint16_t op){ SUB(op); };
    table_8[0x6] = [this](uint16_t op){ SHR(op); };
    table_8[0x7] = [this](uint16_t op){ SUBN(op); };
    table_8[0x8] = noop;
    table_8[0x9] = noop;
    table_8[0xA] = noop;
    table_8[0xB] = noop;
    table_8[0xC] = noop;
    table_8[0xD] = noop;
    table_8[0xE] = [this](uint16_t op){ SHL(op); };
    table_8[0xF] = noop;

    // ----------------------------------------------------------
    // table_E — lower byte (op & 0x00FF)
    // ----------------------------------------------------------
    table_E[0x9E] = [this](uint16_t op){ SKP(op); };
    table_E[0xA1] = [this](uint16_t op){ SKNP(op); };

    // ----------------------------------------------------------
    // table_F — lower byte (op & 0x00FF)
    // ----------------------------------------------------------
    table_F[0x07] = [this](uint16_t op){ LD_VX_DT(op); };
    table_F[0x0A] = [this](uint16_t op){ LD_VX_KEY(op); };
    table_F[0x15] = [this](uint16_t op){ LD_DT_VX(op); };
    table_F[0x18] = [this](uint16_t op){ LD_ST_VX(op); };
    table_F[0x1E] = [this](uint16_t op){ ADD_I_VX(op); };
    table_F[0x29] = [this](uint16_t op){ LD_F_VX(op); };
    table_F[0x33] = [this](uint16_t op){ LD_BCD(op); };
    table_F[0x55] = [this](uint16_t op){ STORE_REGS(op); };
    table_F[0x65] = [this](uint16_t op){ LOAD_REGS(op); };
}

void Chip8::cycle(){

}

void Chip8::loadROM(const std::string& path){

}

void Chip8::execute(uint16_t opcode){
    table_main[(opcode >> 12) & 0xF](opcode);
}

void Chip8::dispatch_0(uint16_t op){
    if      (op == 0x00E0) { display_buffer.fill(0); display_dirty = true; }
    else if (op == 0x00EE) { PC = stack[--SP]; }
}

void Chip8::dispatch_8(uint16_t op){ table_8[op & 0x000F](op); }
void Chip8::dispatch_E(uint16_t op){ table_E[op & 0x00FF](op); }
void Chip8::dispatch_F(uint16_t op){ table_F[op & 0x00FF](op); }

// ---------------------------------------------------------------------------
// Primary opcodes
// ---------------------------------------------------------------------------

void Chip8::JP(uint16_t op)        { PC = op & 0x0FFF; }
void Chip8::CALL(uint16_t op)      {}
void Chip8::SE_VX_KK(uint16_t op)  {}
void Chip8::SNE_VX_KK(uint16_t op) {}
void Chip8::SE_VX_VY(uint16_t op)  {}
void Chip8::LD_VX_KK(uint16_t op)  {}
void Chip8::ADD_VX_KK(uint16_t op) {}
void Chip8::SNE_VX_VY(uint16_t op) {}
void Chip8::LD_I(uint16_t op)      {}
void Chip8::JP_V0(uint16_t op)     {}
void Chip8::RND(uint16_t op)       {}
void Chip8::DRW(uint16_t op)       {}

// ---------------------------------------------------------------------------
// Group 8 — register operations
// ---------------------------------------------------------------------------

void Chip8::LD_VX_VY(uint16_t op)  {}
void Chip8::OR(uint16_t op)        {}
void Chip8::AND(uint16_t op)       {}
void Chip8::XOR(uint16_t op)       {}
void Chip8::ADD_VX_VY(uint16_t op) {}
void Chip8::SUB(uint16_t op)       {}
void Chip8::SHR(uint16_t op)       {}
void Chip8::SUBN(uint16_t op)      {}
void Chip8::SHL(uint16_t op)       {}

// ---------------------------------------------------------------------------
// Group E — key input
// ---------------------------------------------------------------------------

void Chip8::SKP(uint16_t op)  {}
void Chip8::SKNP(uint16_t op) {}

// ---------------------------------------------------------------------------
// Group F — misc
// ---------------------------------------------------------------------------

void Chip8::LD_VX_DT(uint16_t op)   {}
void Chip8::LD_VX_KEY(uint16_t op)  {}
void Chip8::LD_DT_VX(uint16_t op)   {}
void Chip8::LD_ST_VX(uint16_t op)   {}
void Chip8::ADD_I_VX(uint16_t op)   {}
void Chip8::LD_F_VX(uint16_t op)    {}
void Chip8::LD_BCD(uint16_t op)     {}
void Chip8::STORE_REGS(uint16_t op) {}
void Chip8::LOAD_REGS(uint16_t op)  {}

// ---------------------------------------------------------------------------
// VirtualMachine interface
// ---------------------------------------------------------------------------

void Chip8::updateTimers() {
    if (DT > 0) --DT;
    if (ST > 0) --ST;
}

void Chip8::keyDown(uint8_t key) {}
void Chip8::keyUp(uint8_t key)   {}

const weaver::DisplayBuffer& Chip8::getDisplay() const { return display_buffer; }
bool Chip8::displayChanged() const                     { return display_dirty;  }

// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------

uint16_t Chip8::fetch(){
    uint16_t opcode = (static_cast<uint16_t>(memory[PC]) << 8) | memory[PC + 1];
    PC += 2;
    return opcode;
}

Chip8::DecodedOp Chip8::decode(uint16_t opcode){
    DecodedOp decodedOp = {};
    return{
        decodedOp.type = static_cast<uint8_t>((opcode >> 12) & 0xF),
        decodedOp.x    = static_cast<uint8_t>((opcode >> 8)  & 0xF),
        decodedOp.y    = static_cast<uint8_t>((opcode >> 4)  & 0xF),
        decodedOp.n    = static_cast<uint8_t>(opcode & 0xF),
        decodedOp.nn   = static_cast<uint8_t>(opcode & 0xFF),
        decodedOp.nnn  = static_cast<uint16_t>(opcode & 0xFFF),
    };
}

} // namespace weaver::chip8
