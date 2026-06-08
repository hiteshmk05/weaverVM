#pragma once

#include <cstdint>

namespace weaver::input {

// Engine-neutral physical key identifiers. Only the keys actually needed
// by a supported VM's keypad belong here — extend as new systems require.
enum class PhysicalKey : uint8_t {
    One, Two, Three, Four,
    Q, W, E, R,
    A, S, D, F,
    Z, X, C, V,
};

// Thin bridge over the windowing/input backend (currently raylib). Anything
// that needs to read physical keyboard state goes through this — VMs never
// talk to raylib directly, so swapping backends only touches this class.
class Input {
public:
    bool pressed(PhysicalKey key) const;
    bool released(PhysicalKey key) const;
};

} // namespace weaver::input
