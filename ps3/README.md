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

## Install and test on PS3 HEN

1. Wait for the **PS3 homebrew proof of concept** GitHub Actions job to finish successfully.
2. Download its `leonida-ps3-homebrew-<commit>` artifact from the Actions run and extract the ZIP.
3. Copy `leonida_ps3_poc.pkg` to a FAT32 USB drive.
4. On the PS3, open **Package Manager → Install Package Files → Standard** and install the PKG.
5. Launch **Leonida PS3 Homebrew PoC** from the Game column.
6. Use the PS button and choose **Quit Game** to verify the registered exit callback.

**Display limitation:** this first test has no RSX framebuffer or text renderer yet. The TV may remain black while
the process is running; that is expected for this non-visual bring-up test, not proof of a crash. Diagnostic text
is sent to stdout and is visible only through a compatible debug/loader setup. A later milestone must add a
simple on-screen status display before this is a user-friendly test app.

A successful CI build is not a hardware test. No remote console access is available to this workflow, so the
actual HEN launch/exit test still has to be performed on your console. Record the console model, HEN version
and observed result before treating the hardware milestone as complete.
