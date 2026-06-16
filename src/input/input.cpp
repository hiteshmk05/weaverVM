#include "input.hpp"

#include <SDL.h>

namespace weaver::input {

namespace {

int toScancode(PhysicalKey key) {
    switch (key) {
        case PhysicalKey::One: return SDL_SCANCODE_1;
        case PhysicalKey::Two: return SDL_SCANCODE_2;
        case PhysicalKey::Three: return SDL_SCANCODE_3;
        case PhysicalKey::Four: return SDL_SCANCODE_4;
        case PhysicalKey::Q: return SDL_SCANCODE_Q;
        case PhysicalKey::W: return SDL_SCANCODE_W;
        case PhysicalKey::E: return SDL_SCANCODE_E;
        case PhysicalKey::R: return SDL_SCANCODE_R;
        case PhysicalKey::A: return SDL_SCANCODE_A;
        case PhysicalKey::S: return SDL_SCANCODE_S;
        case PhysicalKey::D: return SDL_SCANCODE_D;
        case PhysicalKey::F: return SDL_SCANCODE_F;
        case PhysicalKey::Z: return SDL_SCANCODE_Z;
        case PhysicalKey::X: return SDL_SCANCODE_X;
        case PhysicalKey::C: return SDL_SCANCODE_C;
        case PhysicalKey::V: return SDL_SCANCODE_V;
    }
    return 0;
}

} // namespace

void Input::update() {
    previous = current;

    const Uint8* state = SDL_GetKeyboardState(nullptr);
    for (int i = 0; i < 16; ++i) {
        current[i] = state[toScancode(static_cast<PhysicalKey>(i))];
    }
}

bool Input::pressed(PhysicalKey key) const {
    auto i = static_cast<int>(key);
    return current[i] && !previous[i];
}

bool Input::released(PhysicalKey key) const {
    auto i = static_cast<int>(key);
    return !current[i] && previous[i];
}

} // namespace weaver::input
