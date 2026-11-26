# PS3 port feasibility

## Decision

This repository cannot be converted into a PlayStation 3 build by changing the Rust target or replacing the
Direct3D 12 calls. A real PS3 version would be a separate port/reimplementation with a new platform runtime,
renderer and input/audio/storage layers. The existing Windows implementation remains untouched.

This is a feasibility boundary, not a claim that GTA VI has been ported or that a PS3 package can be produced
from this repository today.

## Why the current program does not map to PS3

| Existing Leonida Legacy | PlayStation 3 |
|---|---|
| Windows x86-64 process | Cell processor: PowerPC-based PPU plus SPUs |
| Windows loader and PE32+ title image | PS3 SELF/ELF loading and GameOS/homebrew interfaces |
| Host Direct3D 12 device and command lists | RSX graphics processor and PS3 graphics APIs |
| DXIL shader output | RSX-compatible shader/program path |
| Windows APIs (`windows`, `windows-sys`) | PS3 system libraries and homebrew SDK interfaces |
| Desktop memory and GPU budgets | 256 MiB system memory plus 256 MiB video memory |

The game code this project expects to load is Xbox Series x86-64 code. The PS3 cannot execute that code natively.
The Xbox runtime shims, PE loader, Direct3D 12 backend and shader translator are not a PS3 runtime.

The workspace is Rust 2024 and pins a Windows MSVC target in `rust-toolchain.toml`. The repository does not
currently define a supported PS3 Rust target, PS3 linker, SELF packaging pipeline or RSX backend. Merely adding
a target name would not supply a working Rust standard library, ABI, system bindings or linker for PS3 GameOS.

## What a genuine PS3 effort would require

1. **Toolchain proof:** select and pin a PS3 homebrew toolchain (for example PSL1GHT plus its PPU/SPU tools),
   and build a minimal native program outside the current Windows workspace.
2. **Bootable package:** turn that program into a PS3-loadable ELF/SELF and a HEN-installable PKG; test it on
   real hardware. This is a separate deliverable from running GTA VI.
3. **Platform layer:** implement controller input, file/USB access, audio, timing, memory allocation, threading,
   and error reporting against PS3 APIs.
4. **Graphics layer:** implement a renderer for RSX. D3D12 objects, command lists, resource layouts and DXIL
   shaders cannot be reused as-is.
5. **Application/game:** provide source code for the game logic or independently reimplement it, plus assets
   that are legally usable on the target. This repository contains neither GTA VI source code nor the game data.

Even after steps 1–4, that would only establish a native PS3 application/runtime foundation. It would not make
GTA VI playable. Recreating a modern open-world game for PS3 would be a separate, extremely large project with
major memory, CPU, GPU and content constraints.

## Safe scope for this branch

- Keep `main` and all Windows functionality unchanged.
- Do not add game files, extracted assets, executable fragments, shaders, screenshots, keys or other derived
  proprietary data.
- Do not add placeholder PS3 source that cannot be built or tested.
- Do not claim a SELF/PKG exists until a reproducible build produces one and it has been tested on hardware.
- Treat a PS3 homebrew proof of concept as a separate project boundary unless a supported toolchain and
  maintainable build path are demonstrated.

## Evidence and references

- The current repository architecture is documented in [architecture.md](architecture.md); its execution path
  is Windows x86-64, Windows APIs and host D3D12.
- The workspace target is pinned in [rust-toolchain.toml](../rust-toolchain.toml).
- The upstream Native32Emu discussion also records the same core Rust/PowerPC64 PS3 target limitation:
  [libretro/libretro-super issue #2048](https://github.com/libretro/libretro-super/issues/2048).
- The PS3 homebrew ecosystem uses a separate platform toolchain; PSL1GHT is one public SDK project:
  [PSL1GHT](https://github.com/ps3dev/PSL1GHT).
