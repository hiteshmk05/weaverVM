#include "input.hpp"

#include "raylib.h"

namespace weaver::input {

namespace {

int toRaylibKey(PhysicalKey key) {
    switch (key) {
        case PhysicalKey::One:   return KEY_ONE;
        case PhysicalKey::Two:   return KEY_TWO;
        case PhysicalKey::Three: return KEY_THREE;
        case PhysicalKey::Four:  return KEY_FOUR;
        case PhysicalKey::Q:     return KEY_Q;
        case PhysicalKey::W:     return KEY_W;
        case PhysicalKey::E:     return KEY_E;
        case PhysicalKey::R:     return KEY_R;
        case PhysicalKey::A:     return KEY_A;
        case PhysicalKey::S:     return KEY_S;
        case PhysicalKey::D:     return KEY_D;
        case PhysicalKey::F:     return KEY_F;
        case PhysicalKey::Z:     return KEY_Z;
        case PhysicalKey::X:     return KEY_X;
        case PhysicalKey::C:     return KEY_C;
        case PhysicalKey::V:     return KEY_V;
    }
    return KEY_NULL;
}

} // namespace

bool Input::pressed(PhysicalKey key) const {
    return IsKeyPressed(toRaylibKey(key));
}

bool Input::released(PhysicalKey key) const {
    return IsKeyReleased(toRaylibKey(key));
}

} // namespace weaver::input
