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

void Chip8::cycle(){

}

void Chip8::loadROM(const std::string& path){

}

void execute(uint16_t opcode){

}

void Chip8::updateTimers() {
      if (DT > 0) --DT;
      if (ST > 0) --ST;
  }

void Chip8::keyDown(uint8_t key) {

}
void Chip8::keyUp(uint8_t key) {
    
}

const weaver::DisplayBuffer& Chip8::getDisplay() const {
    return display_buffer;
}

bool Chip8::displayChanged() const {
    return display_dirty;
}

uint16_t Chip8::fetch(){
    uint16_t opcode = (static_cast<uint16_t>(memory[PC]) << 8) | memory[PC + 1];
    PC += 2;
    return opcode; 
}

Chip8::DecodedOp Chip8::decode(uint16_t opcode){
    return {};
}

} // namespace weaver::chip8
