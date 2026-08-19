---
name: gcode-dialect-translation-contract
description: Architectural contract and semantic invariants for translating Griffin GCode dialect into MiracleGrue JSON Toolpaths (mgjtp).
trigger: glob
glob: "src/gcode/**,src/miracle_jtp/**,include/dulcificum/**"
paths:
  - "src/gcode/**"
  - "src/miracle_jtp/**"
  - "include/dulcificum/**"
---
# Griffin GCode to MiracleGrue JSON Toolpath (`mgjtp`) Translation Contract

This rule codifies the translation semantics between the input Griffin GCode dialect and the output MakerBot / Method MiracleGrue JSON Toolpath representation.

## 1. GCode Parsing & Extraction Invariants

1. **Temperature Commands (`M104`, `M109`, `M140`, `M190`)**:
   - Both `S` (target temperature / heatup) and `R` (target temperature including cooldown wait) parameters MUST be supported.
   - When extracting temperature targets, check `S` first; if absent, fall back to `R`.
   - Never access `std::optional<double>` temperature fields without a prior `.has_value()` check.
2. **Fan Speed Commands (`M106`, `M107`)**:
   - Fan speeds specified with `S` on the `[0, 255]` PWM scale MUST be normalized to the `[0.0, 1.0]` duty cycle float scale ($S / 255.0$) before emission into `botcmd::FanDuty`.
   - Duty cycles already in the `[0.0, 1.0]` range must remain clamped within `[0.0, 1.0]`.
3. **Dwell / Pause Commands (`G4`)**:
   - Parameter `P` is specified in milliseconds and MUST be converted to seconds ($P / 1000.0$) for `botcmd::Delay`.
   - Parameter `S` is specified in seconds and is used directly.
4. **Linear Moves (`G0`, `G1`)**:
   - Relative extruder positioning ($E$) MUST be computed as a delta against the immediately preceding extruder state (`previous_state`).
   - Feedrates in GCode ($mm/\min$) MUST be converted to $mm/s$ ($F / 60.0$) for MiracleGrue output.
5. **Feature Tags**:
   - Slicer semantic comments (`;TYPE:`) MUST map deterministically to `botcmd::Tag` values indexed by the active extruder head (`0` or `1`).

## 2. Stateless Pipeline & Memory Invariants

1. **Stateless Translation**: The translation pipeline (`GCode2Miracle_JTP`) MUST remain re-entrant and stateless across multiple calls.
2. **Extruder Initialization**: The number of extruders (`nb_extruders`) must be explicitly passed into `miracle_jtp::k_key_str::init()` deterministically per translation run.
3. **Bounded Memory**: State tracking throughout `VisitCommand` must keep only the immediately preceding state in `std::optional<state_t> previous_state` rather than accumulating state snapshots in unbounded vectors.
