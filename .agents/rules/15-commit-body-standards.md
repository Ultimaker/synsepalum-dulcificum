---
name: commit-body-standards
description: Standards for commit message structure, bodies, and traceability in synsepalum-dulcificum.
trigger: always_on
---
# Commit Message & Body Standards

In addition to the commit title format `[UC-3697] <Descriptive Title>` required by Rule 01, commit messages MUST maintain thorough structural explanations.

## 1. Commit Message Structure

Every non-trivial commit MUST include:
1. **Title line**: `[<JIRA-KEY>] <Imperative summary under 72 chars>`
2. **Blank line**
3. **Body sections**:
   - **Why**: The problem, bug report, or requirement driving this change.
   - **What**: The core architectural or functional changes introduced.
   - **How**: Key technical details, modified functions, refactored data structures, or safety guarantees added.
   - **Impact**: Any affected consumers (DulcificumJS/WASM, pyDulcificum, CLI translator, Neoprep).

## 2. Commit Integrity & Atomicity

- Commits MUST be self-contained and pass all unit tests independently (`ctest`).
- Never leave WIP or fixup commits in published review branches.
