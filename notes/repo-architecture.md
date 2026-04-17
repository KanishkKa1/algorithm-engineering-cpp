# Repo Architecture

This repo is organized for *pattern reuse* rather than for platform tags.

---

## Top-level folders

- `patterns/`
  - The primary content: reasoning-first notes grouped by the technique that solves them.
  - Each note is intended to be a reusable “playbook” for the pattern.
- `implementations/`
  - Reusable C++ building blocks (e.g., DSU) that show an interview-ready implementation style.
- `templates/`
  - Boilerplate snippets for quick starts (prefer adapting templates over retyping).
- `notes/`
  - Meta: how to think, how to navigate, and how to write solution notes consistently.

---

## Naming conventions

- Pattern folders are nouns or noun-phrases: `range-processing/`, `union-find/`, `simulation-state-machine/`.
- Problem files use kebab-case and should be stable (renames are okay, but fix links if you do).

---

## Authoring checklist for a new note

- Start with **reframing**: name the abstraction and the invariant you plan to maintain.
- Write the brute force and explicitly state the bottleneck.
- Capture at least one *rejected approach* and the reason it fails.
- Keep code minimal:
  - small state
  - no unnecessary templates/macros
  - clear variable names

---

## Why this structure works

- Searching by pattern helps you solve *new* problems faster than searching by problem title.
- Notes stay useful even when the original platform/problem changes, because the technique does not.
