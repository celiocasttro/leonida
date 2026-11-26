# TODO

The goal: a console-like experience on PC. The game starts straight into play, holds a steady 30 fps (the console's
33 ms frame) or better, looks like it does on the console, and doesn't stutter or crash.

## How to use this list

- **One item, one pull request.** Keep it small, and touch only the files the item needs.
- **Claim an item by opening a draft pull request** with its ID in the title (for example `P-3: ...`) as soon as
  you start. Search the open pull requests for the ID first. If someone already has it, pick another one.
- **The pull request that finishes an item deletes that item's row.** Don't edit other rows in it.
- **New item:** add a row at the end of its section with the next free ID. Never renumber.
- **Needs:** `game` means your own copy of the game, `GPU` a real Direct3D 12 card, `-` nothing beyond Rust.
- **Numbers** come from [docs/fork-notes.md](docs/fork-notes.md) unless a row says otherwise, and most were measured
  on one RTX 4070. Measure on your machine before and after a change, and put both figures in the pull request.
  Post figures, never log files: logs can quote game data (see [docs/asset-policy.md](docs/asset-policy.md)).

## Rendering: looks like the console

| ID | Task | Where | Needs |
|----|------|-------|-------|
| R-1 | Everything out of direct sun is black: the ray-traced bounce light and lamp and sun shadows don't run. The structure-builder programs translate and 59 of 64 trace programs translate, but nothing is lit by them yet. | [docs/fork-ray-tracing.md](docs/fork-ray-tracing.md), `leonida-shader`, `leonida-gpu` | game, GPU |
| R-2 | The first gameplay room is sometimes not drawn after the cutscene hands over to gameplay. | `leonida-gpu` | game, GPU |
| R-3 | Washed-out frames turn up in about 6 of 21 runs with no switch on. Cause not known. | `leonida-gpu` | game, GPU |
| R-4 | An indirect compute pass asks for about 77,500 thread groups in x, where one dispatch runs 65,535. The rest is cut off; split the dispatch. | `leonida-gpu` | game, GPU |
| R-5 | Real queries, depth bounds, HDR output and scaling are missing. | `leonida-gpu` | game, GPU |
| R-6 | Render-target metadata planes are not handled. | `leonida-formats`, `leonida-gpu` | game, GPU |
| R-7 | Shaders that don't translate drop their draws. Count them again on this tree with `leonida translate --list` (the upstream counts in the fork notes are from 2026-11-23) and work down the list. | `leonida-shader` | game |
| R-8 | Mapping pool pages in a row as one view made remaps cheaper but broke texture streaming (black or red faces, garbage in 3 of 3 runs). Find out why before trying it again; see P-4. | `leonida-hle` (`xmem`) | game, GPU |

## Performance: a steady 30 fps

| ID | Task | Where | Needs |
|----|------|-------|-------|
| P-1 | Measure the frame-rate switches that `run-gta6.ps1` sets on more cards (AMD, NVIDIA 30 and 40 series, Intel). Turn on by default the ones that help everywhere without a device loss. Since #24 the run scripts take their switches from `gpu-profiles.json` by graphics card: add a profile for each card measured, with its figures. | `leonida-gpu`, `gpu-profiles.json` | game, GPU |
| P-2 | The per-record clamp before indirect draws is most of the card's time. About 2,400 indirect commands a frame still get a clamp pass, and about 1,000 mesh records sit scattered through a 100 MB argument buffer. | `leonida-gpu` | game, GPU |
| P-3 | Copies between the PC's memory and the card's are most of the submit thread's time. | `leonida-gpu` | game, GPU |
| P-4 | While the camera moves, about half the time goes to frames of about 490 ms in which the card is idle for about 330 ms. The title's page-mapping copies (`CopyPageMappingsBatchX` to `xmem::map_alias`) cost 32 ms of every cutscene frame and 246 ms of a streaming frame. | `leonida-hle` | game |
| P-5 | The title paces itself with `monitorx` / `mwaitx`, which only AMD processors have: about 280,000 emulated exceptions a second on an Intel processor. Find a cheaper way. | `leonida-loader` | game; an Intel CPU to measure |
| P-6 | Translated shaders are too expensive: about 50 ms of a 97 ms frame are the title's own passes as translated. With every pass of the layer's gone, the first rooms would still be near 20 fps. On an RTX 3080, after #23 took the layer's own hash and compare passes from about 21 to 3.6 ms of card time a frame, the title's passes are about 95 ms of it. | `leonida-shader` | game, GPU |
| P-8 | About 24 GB of memory is in use during play. | all | game |
| P-9 | A 12 GB card is full from the first world frame (9.2 of a 10.3 GB budget), and thousands of textures are dropped and made again. Levels on demand (`LEONIDA_LEVELS_ON_DEMAND=1`) saved 3.1 GB in one run; test it more and make it the default if it holds. | `leonida-gpu` | game, GPU |

## Stability

| ID | Task | Where | Needs |
|----|------|-------|-------|
| S-1 | One device loss with indirect draw windows on (`LEONIDA_INDIRECT_WINDOW=1`), 44 s into the world. Cause not found. Don't combine it with `LEONIDA_CARD_FETCH_INFLIGHT=1`. | `leonida-gpu` | game, GPU |
| S-2 | A session past the first rooms lost the device in the open world with no driver entry ("device removed"). | `leonida-gpu` | game, GPU |
| S-3 | Menu frame pacing drifts on one screen in release builds. | `leonida-gpu` | game, GPU |

## Audio, input and system services

| ID | Task | Where | Needs |
|----|------|-------|-------|
| A-1 | Reverb is silent. | `leonida-hle` (audio) | game |
| A-2 | Opus decoding is not checked against what the title expects. | `leonida-formats`, `leonida-hle` | game |
| A-3 | Trigger rumble. | `leonida-hle` (input) | game, an Xbox pad |
| A-4 | Speech is silent. The text entry dialog and the title relaunching itself are untested live. | `leonida-hle` | game |
| A-5 | Series S mode stops at the loading screen. | all | game |

## Console-like experience

| ID | Task | Where | Needs |
|----|------|-------|-------|
| U-1 | One step from download to playing: build, warm the cache (P-7) and run, with no terminal knowledge needed. | `run-gta6.ps1` | - |
| U-2 | Fullscreen and borderless window, and a choice of output resolution. | `leonida-gpu` | GPU |
| U-3 | The frame-rate switches as settings in `leonida.toml` instead of environment variables, with the tested ones on by default. Since #24 the run scripts pick them by graphics card from `gpu-profiles.json`; this item moves that choice into the program itself. | `leonida-core` (config) | - |
| U-4 | Pad first: after launch, the game is playable without touching the keyboard. | `leonida-hle` (input) | game |
| U-5 | Clear messages for the common failures: no AVX2, missing title DLLs, Windows error 1455 (not enough commit). Upstream VIctory has commits for the first two; see UP-1. | `apps/leonida` | - |

## Code health

| ID | Task | Where | Needs |
|----|------|-------|-------|
| C-2 | Failing test `indirect::tests::warp_budget_saturates_without_wraparound_and_preserves_draw_fields`, inherited from victory-gta6. Upstream VIctory `4ab20fc` ("the indirect budget is charged in record order and no longer wraps") may fix it; not checked. | `leonida-gpu` | - |
| C-3 | `cargo fmt --check` reports about 1,420 differences. Run `cargo fmt` once, in a pull request of its own, at a moment when no other pull request is open (it touches almost every file), then add the check to CI. | all | - |
| C-4 | 163 `shortcut:` markers where the code approximates the console (79 in `leonida-gpu`, 55 in `leonida-shader`, 23 in `leonida-hle`, 4 in `leonida-formats`, 2 in `leonida-loader`). Replace them with the console's real behaviour, one per pull request. | all | game |
| C-5 | Some style lints are switched off for this fork in the workspace `Cargo.toml`. Turn them back on and tidy the code they flag. | `Cargo.toml` | - |

## Upstream

| ID | Task | Where | Needs |
|----|------|-------|-------|
| UP-1 | Upstream VIctory has 265 commits this fork lacks (counted on 2026-11-25 with `git cherry`). Go through them and cherry-pick the useful ones: `git remote add victory https://github.com/Marvisak/VIctory.git`, `git fetch victory`, `git cherry-pick <hash>`, then `scripts/rename-from-upstream.ps1` and `cargo check --workspace --all-targets`. Candidates by title, none tried yet: `4ab20fc` (indirect budget, C-2), `534e1dd` (AVX2 check), `e9cb14a` (missing title DLLs message), `42961e8` (warns when GPU hang settings would freeze the PC), `a3acc87` (checks the required startup archive). Many of the GPU commits touch code victory-gta6 changed, so expect conflicts there. | whole tree | - |

## PS3 platform experiment (separate from GTA VI compatibility)

| ID | Task | Where | Needs |
|----|------|-------|-------|
| PS3-1 | Build and boot a minimal native PS3 homebrew ELF/SELF/PKG with a pinned PS3 toolchain. Keep it independent of the Windows/Rust workspace; do not claim GTA VI compatibility. Decide whether a separate repository is the right home after the proof of concept boots on real hardware. | separate PS3 proof of concept | PS3 HEN/CFW hardware |
