---
name: library-consumer-contract
description: This repository's public surface is mounted into other repositories — every exported symbol, model field and schema is a cross-repository contract.
trigger: glob
glob: "include/**"
paths:
  - "include/**"
---
# Library Consumer Contract

This repository is consumed by other repositories rather than run on its own.
The evidence, from this tree:

- no entry point starts a process here, yet the repository publishes a surface (`include/`)


A service is bounded by its own process: rename an internal function and
nothing outside notices. This repository has no such boundary. Its surface is
mounted into someone else's build, so a change here lands in trees that this
checkout cannot see and that no test in this repository runs.

## What that makes a breaking change

Within `include/`, all of the following are contract changes,
not refactors:

1. **Renaming or removing any exported symbol** — class, function, constant,
   type, or module path. A consumer imports it by name.
2. **Changing a model or schema field** — removing it, renaming it, narrowing
   its type, or making an optional field required. Adding an optional field
   with a default is the only safe shape change.
3. **Changing a default value or an enum member**, including its wire value.
4. **Moving a file between packages**, even with the symbol re-exported: a
   consumer may import the module path directly.

## How to make one anyway

1. **Name the consumers in the pull request.**
   Enumerate them before you start — a consumer you did not check is a
   consumer you broke. Search the organisation for pins of this repository:

   ```bash
   gh search code --owner Ultimaker --filename .gitmodules "$(basename "$PWD")"
   ```

2. **Land this repository first, then move each consumer's pointer.** A
   submodule pointer bump is its own commit and names the revision it moves to.
   Never commit inside a consumer's mounted copy of this tree.
3. **Additive first.** Where a breaking shape is unavoidable, ship the new
   surface alongside the old one, migrate the consumers, and remove the old
   surface in a later ticket — not in the same one.
4. **Say so in the commit message.** The consumers' agents read this
   repository's history to work out what moved under them.

## What this rule does not cover

The *meaning* of the contract — which peer owns which definition, what happens
operationally when a field changes — is the ecosystem-contract investigator's
subject (`.agents/agents/ecosystem_contract_investigator/agent.md`). This rule
covers only what is provable from the tree: that the surface is shared, and
that a change to it is never local.
