# weaver-vm

A multi-system retro emulator written in modern C++20. The goal is to accurately emulate classic hardware architectures and run real ROM files — starting with CHIP-8 and expanding to Intel 8080.

Built around a clean abstract VM interface so each emulated system is a self-contained module. Adding a new system does not require changing any existing code.

---

## Systems

| System | Status | Games |
|---|---|---|
| CHIP-8 | In progress | Pong, Tetris, Space Invaders |
| Intel 8080 | Planned | Space Invaders (arcade) |
| SUPER-CHIP | Planned | Extended CHIP-8 games |

---

## Tech Stack

- **Language** — C++20
- **Rendering** — SDL2
- **Build system** — CMake 3.20+ with Ninja and CMakePresets

---

## Prerequisites

- CMake 3.20 or newer
- Ninja build system
- A C++20 compiler (GCC 12+, Clang 14+, or MSVC 2022+)
- SDL2 development libraries

### Installing SDL2

**Windows (MSYS2):**

```
pacman -S mingw-w64-ucrt-x86_64-SDL2
```

**Linux (Debian/Ubuntu):**

```
sudo apt install libsdl2-dev
```

**Linux (Arch):**

```
sudo pacman -S sdl2
```

**macOS:**

```
brew install sdl2
```

---

## Building

Configure and build using the bundled CMake preset:

```
cmake --preset default
cmake --build build
```

The binary is placed at `build/weaver-vm.exe` (Windows) or `build/weaver-vm` (Linux/macOS).

To do a clean rebuild:

```
cmake --build build --clean-first
```

---

## Running

Place ROM files in the `roms/` directory, then run from the build folder:

```
cd build
./weaver-vm
```

---

## Project Structure

```
src/
  main.cpp              — entry point
  core/
    vm_interface.hpp    — abstract VirtualMachine base class
  chip8/
    chip8.hpp           — CHIP-8 emulator declaration
    chip8.cpp           — CHIP-8 emulator implementation
  display/
    display.hpp         — SDL2 display wrapper
    display.cpp         — window creation, texture rendering
  input/
    input.hpp           — backend-neutral input abstraction
    input.cpp           — SDL2 keyboard state mapping
```
