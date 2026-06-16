#pragma once

#include <cstdint>
#include <array>

namespace weaver::input {

enum class PhysicalKey : uint8_t {
    One, Two, Three, Four,
    Q, W, E, R,
    A, S, D, F,
    Z, X, C, V,
};

class Input {
public:
    void update();
    bool pressed(PhysicalKey key) const;
    bool released(PhysicalKey key) const;

private:
    std::array<bool, 16> current{};
    std::array<bool, 16> previous{};
};

} // namespace weaver::input
