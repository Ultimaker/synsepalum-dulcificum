---
name: python-conventions
description: Python conventions mined from this repository (typing, async, frameworks, formatting, logging).
trigger: glob
glob: "**/*.py"
paths:
  - "**/*.py"
---
# Python Conventions (3 files sampled)

> These rules are DISCOVERED from this repository's own sources by the agentic
> bootstrap — they are not organization-wide defaults. Re-run the bootstrap with
> `--update` after intentional convention changes.

0. **Load the `python-pro` skill** before implementing: it carries the type-safety, async, and packaging practices these conventions assume.
1. **Type Hints on New Code**: annotate all new functions with parameter and return types, even where legacy code is unannotated.
2. **I/O Discipline**: follow the existing synchronous patterns; do not introduce async frameworks without an explicit architecture decision.
3. **Logging & Redaction**: replace stray `print()` calls with `logging` when touching a module. Errors log to `sys.stderr` context and scripts exit non-zero (`sys.exit(1)`) on failure. Never write PII, credentials, or tokens to logs, stdout, or debug output.
4. **String Formatting**: the dominant style here is **f-strings** (f-strings 5). Use it consistently; do not mix formatting styles within a module.
