---
name: wasm-embind-contract
description: Emscripten/WebAssembly and embind boundary contracts for JavaScript/TypeScript consumers (Neoprep / web workers).
trigger: glob
glob: "DulcificumJS/**"
paths:
  - "DulcificumJS/**"
---
# WebAssembly & Embind Contract Rules

Dulcificum compiles to WebAssembly (`dulcificum_js.js`) via Emscripten and embind, consumed in browser, web-worker, and Node environments by **Neoprep** and other frontend toolpath consumers.

## 1. Link Options and Module Interface
The Emscripten link options in `DulcificumJS/CMakeLists.txt` form a published contract with downstream bundlers:
- `SHELL:-sENVIRONMENT=web,worker,node` — Enables runtime across web, web-worker, and node environments.
- `SHELL:-s USE_ES6_IMPORT_META=1`, `SHELL:-s EXPORT_ES6=1`, `SHELL:-s MODULARIZE=1`, `SHELL:-s SINGLE_FILE=1` — Dictates ES module import and bundler packaging behavior.
- `SHELL: --embind-emit-tsd dulcificum_js.d.ts` — Emits TypeScript type declarations during compilation.
Do NOT modify these flags without testing compatibility against downstream bundlers (such as Vite/Webpack in Neoprep).

## 2. Embind Export Stability
- The exported functions in `DulcificumJS/DulcificumJS.cpp` (`gcode_2_miracle_jtp`, `dulcificum_info`, `info_t`) are the public API surface.
- Any change to signature or exported types is a breaking API change for frontend consumers.
- Maintain fallback and default values (e.g. `nb_extruders = 2`).

## 3. Boundary Safety & Exceptions
- Never allow C++ exceptions to unwind uncaught across the WebAssembly/embind boundary.
- Any parsing or conversion failure must be caught and converted to structured error information or descriptive logs before crossing to JS.

## 4. Pure Stateless Core
- All WASM-specific glue must stay inside `DulcificumJS/`.
- The core C++ library (`include/`, `src/`) must remain clean, modular C++20 with no direct Emscripten headers or dependencies.
