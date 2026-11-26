# PS3 homebrew proof of concept

This directory is deliberately isolated from the Windows/Rust Leonida runtime. It is a native PlayStation 3
homebrew application built for the Cell PPU with the PS3DEV toolchain and PSL1GHT.

## Build

The supported build environment is the open-source [PS3DEV project](https://github.com/ps3dev/ps3dev).
The GitHub Actions workflow in this repository builds the toolchain and then invokes the PSL1GHT project
Makefile.

Locally, with PS3DEV and PSL1GHT installed:

```sh
export PS3DEV=/usr/local/ps3dev
export PSL1GHT=$PS3DEV
export PATH=$PATH:$PS3DEV/bin:$PS3DEV/ppu/bin:$PS3DEV/spu/bin
make -C ps3/homebrew/hello clean
make -C ps3/homebrew/hello
make -C ps3/homebrew/hello pkg
```

Expected outputs are `leonida_ps3_poc.elf`, `leonida_ps3_poc.self` and, when the packaging target
succeeds, `leonida_ps3_poc.pkg`.

## What this proof of concept does

It is intentionally a small native PPU program. It initializes the system callback and controller input,
keeps the application alive until the XMB Quit Game event, and writes diagnostic messages to stdout
(for example, when launched with a compatible debug/loader setup).

It does **not** implement a renderer, run Xbox code, load GTA VI, or prove game compatibility. The first
milestone is only to establish that the native toolchain can produce a PS3 homebrew executable and package.

## Hardware test

A successful CI build is not a hardware test. Install the PKG on a PS3 running HEN, launch it from the XMB,
verify that it starts and returns cleanly through the PS button > Quit Game flow, then record the console
model, HEN version and observed result. No remote console access is available to this workflow.
