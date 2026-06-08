#include "chip8.hpp"

#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cstdint>
#include <cstdio>

namespace weaver::chip8 {

Chip8::Chip8(){
    screen.init("CHIP-8", 10);
    reset();
    init_tables();
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
    keys.fill(false);
    displayBuffer.fill(0);

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
    constexpr int instructionsPerFrame = 10;

    // --- TEMPORARY DIAGNOSTIC TRACE: remove once the freeze is found ---
    static int frameCount = 0;
    bool trace = (frameCount++ < 3);
    if (trace) { printf("[cycle %d] start, PC=%04X\n", frameCount, PC); fflush(stdout); }
    // -------------------------------------------------------------------

    for (int i = 0; i < instructionsPerFrame; ++i) {
        uint16_t opcode = fetch();
        if (trace) { printf("[cycle %d]   fetched %04X, PC now %04X\n", frameCount, opcode, PC); fflush(stdout); }
        execute(opcode);
        if (trace) { printf("[cycle %d]   executed %04X ok\n", frameCount, opcode); fflush(stdout); }
    }

    if (trace) { printf("[cycle %d] rendering...\n", frameCount); fflush(stdout); }
    screen.render(displayBuffer);
    if (trace) { printf("[cycle %d] render done\n", frameCount); fflush(stdout); }
}

void Chip8::loadROM(const std::string& path){
    std::ifstream rom(path, std::ios::binary | std::ios::ate);
    if (!rom) throw std::runtime_error("failed to open ROM: " + path);

    std::streamsize size = rom.tellg();
    if (size <= 0 || static_cast<size_t>(size) > memory.size() - 0x200)
        throw std::runtime_error("ROM does not fit in memory: " + path);

    rom.seekg(0, std::ios::beg);
    rom.read(reinterpret_cast<char*>(memory.data() + 0x200), size);
}

void Chip8::execute(uint16_t opcode){
    table_main[(opcode >> 12) & 0xF](opcode);
}

void Chip8::dispatch_0(uint16_t op){
}

void Chip8::dispatch_8(uint16_t op){ table_8[op & 0x000F](op); }
void Chip8::dispatch_E(uint16_t op){ table_E[op & 0x00FF](op); }
void Chip8::dispatch_F(uint16_t op){ table_F[op & 0x00FF](op); }

// ---------------------------------------------------------------------------
// Primary opcodes
// ---------------------------------------------------------------------------

void Chip8::JP(uint16_t op) { 
    PC = op & 0x0FFF; 
}

/**
 * function call 
 * saves the return address and pushes the current PC onto the stack 
 * sets the PC to the address present in the lower bits (NNN)
 */
void Chip8::CALL(uint16_t op) {
    stack[SP] = PC;
    SP++;
    PC = op & 0x0FFF;
}

/**
 * skip if equal
 * compares the register Vx against the value KK;
 * if they are equal it skips the next instruction by adding 2 to PC
 */
void Chip8::SE_VX_KK(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;
    uint8_t kk = op & 0x00FF;

    if(V[x] == kk) PC += 2;
}


void Chip8::SNE_VX_KK(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;
    uint8_t kk = op & 0x00FF;

    if(V[x] != kk) PC += 2;
}


void Chip8::SE_VX_VY(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;
    uint8_t y = (op >> 4) & 0xF;

    if (V[x] == V[y]) PC += 2;
}


void Chip8::LD_VX_KK(uint16_t op) {
    uint8_t x  = (op >> 8) & 0xF;
    uint8_t kk = op & 0xFF;

    V[x] = kk;
}

void Chip8::ADD_VX_KK(uint16_t op) {
    uint8_t x  = (op >> 8) & 0xF;
    uint8_t kk = op & 0xFF;

    V[x] += kk;
}

void Chip8::SNE_VX_VY(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;
    uint8_t y = (op >> 4) & 0xF;

    if (V[x] != V[y]) PC += 2;
}

/**
 * loads the 12 bit address in the Index register (I)
 */
void Chip8::LD_I(uint16_t op) {
    I = op & 0x0FFF;
}


void Chip8::JP_V0(uint16_t op) {
    uint16_t nnn = op & 0x0FFF;

    PC = nnn + V[0];
}

void Chip8::RND(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;
    uint8_t kk = op & 0xFF;

    V[x] = (rand()%256) & kk;
}
void Chip8::DRW(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;
    uint8_t y = (op >> 4) & 0xF;
    uint8_t n = op & 0xF;

    uint8_t startX = V[x] % weaver::DISPLAY_WIDTH;
    uint8_t startY = V[y] % weaver::DISPLAY_HEIGHT;

    V[0xF] = 0;

    for (uint8_t row = 0; row < n; ++row) {
        uint8_t spriteByte = memory[I + row];
        uint8_t py = startY + row;
        if (py >= weaver::DISPLAY_HEIGHT) break;

        for (uint8_t col = 0; col < 8; ++col) {
            if (!(spriteByte & (0x80 >> col))) continue;

            uint8_t px = startX + col;
            if (px >= weaver::DISPLAY_WIDTH) break;

            size_t idx = static_cast<size_t>(py) * weaver::DISPLAY_WIDTH + px;
            if (displayBuffer[idx]) V[0xF] = 1;
            displayBuffer[idx] ^= 1;
        }
    }
}

// ---------------------------------------------------------------------------
// Group 8 — register operations
// ---------------------------------------------------------------------------

void Chip8::LD_VX_VY(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;
    uint8_t y = (op >> 4) & 0xF;

    V[x] = V[y];
}

void Chip8::OR(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;
    uint8_t y = (op >> 4) & 0xF;

    V[x] |= V[y];
}

void Chip8::AND(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;
    uint8_t y = (op >> 4) & 0xF;

    V[x] &= V[y];
}


void Chip8::XOR(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;
    uint8_t y = (op >> 4) & 0xF;

    V[x] ^= V[y];
}

void Chip8::ADD_VX_VY(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;
    uint8_t y = (op >> 4) & 0xF;

    uint16_t result = V[x] + V[y];

    V[0xF] = result > 0xFF ? 1 : 0;  // carry
    V[x]   = result & 0xFF;
}

void Chip8::SUB(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;
    uint8_t y = (op >> 4) & 0xF;

    V[0xF] = V[x] > V[y] ? 1 : 0;  // no borrow
    V[x]  -= V[y];
}

void Chip8::SHR(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;

    V[0xF] = V[x] & 0x1;  // save LSB
    V[x] >>= 1;
}

void Chip8::SUBN(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;
    uint8_t y = (op >> 4) & 0xF;

    V[0xF] = V[y] > V[x] ? 1 : 0;  // no borrow
    V[x]   = V[y] - V[x];
}

void Chip8::SHL(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;

    V[0xF] = (V[x] >> 7) & 0x1;  // save MSB
    V[x] <<= 1;
}

// ---------------------------------------------------------------------------
// Group E — key input
// ---------------------------------------------------------------------------

void Chip8::SKP(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;

    if (keys[V[x]]) PC += 2;
}
void Chip8::SKNP(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;

    if (!keys[V[x]]) PC += 2;
}

// ---------------------------------------------------------------------------
// Group F — misc
// ---------------------------------------------------------------------------

void Chip8::LD_VX_DT(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;

    V[x] = DT;
}

void Chip8::LD_VX_KEY(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;

    for (uint8_t k = 0; k < keys.size(); ++k) {
        if (keys[k]) {
            V[x] = k;
            return;
        }
    }

    PC -= 2;  // no key down yet — re-fetch this instruction next cycle
}

void Chip8::LD_DT_VX(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;

    DT = V[x];
}

void Chip8::LD_ST_VX(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;

    ST = V[x];
}

void Chip8::ADD_I_VX(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;

    I += V[x];
}

void Chip8::LD_F_VX(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;

    I = 0x050 + (V[x] * 5);
}

void Chip8::LD_BCD(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;
    uint8_t value = V[x];

    memory[I]     = value / 100;
    memory[I + 1] = (value / 10) % 10;
    memory[I + 2] = value % 10;
}

// NOTE: leaves I unchanged (Chip-48/SCHIP convention) — the original COSMAC VIP did I += x + 1, which some ROMs may rely on.
void Chip8::STORE_REGS(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;

    for (uint8_t i = 0; i <= x; ++i) {
        memory[I + i] = V[i];
    }
}

void Chip8::LOAD_REGS(uint16_t op) {
    uint8_t x = (op >> 8) & 0xF;

    for (uint8_t i = 0; i <= x; ++i) {
        V[i] = memory[I + i];
    }
}

// ---------------------------------------------------------------------------
// VirtualMachine interface
// ---------------------------------------------------------------------------

void Chip8::updateTimers() {
    if (DT > 0) --DT;
    if (ST > 0) --ST;
}

void Chip8::keyDown(uint8_t key) {
    if (key < keys.size()) keys[key] = true;
}
void Chip8::keyUp(uint8_t key) {
    if (key < keys.size()) keys[key] = false;
}


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
        decodedOp.x = static_cast<uint8_t>((opcode >> 8)  & 0xF),
        decodedOp.y = static_cast<uint8_t>((opcode >> 4)  & 0xF),
        decodedOp.n = static_cast<uint8_t>(opcode & 0xF),
        decodedOp.nn = static_cast<uint8_t>(opcode & 0xFF),
        decodedOp.nnn = static_cast<uint16_t>(opcode & 0xFFF),
    };
}

} // namespace weaver::chip8
