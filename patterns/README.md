# Patterns

This folder groups problems by the *reasoning pattern* that solves them (the invariant and state), not by platform tags.

The goal is reuse: when you see a new problem, you should be able to map it to a known structure quickly.

## How to navigate

1. Read the statement and extract the *constraint that dominates* (time, memory, number of queries, update vs query).
2. Name the abstraction ("connectivity under threshold", "streaming last-seen", "two-cursor DP", "boundary flood fill").
3. Pick the closest pattern directory and study an example note.

## Pattern index

| Pattern folder | What it’s really about | Typical signals |
|---|---|---|
| `dynamic-programming/` | Optimal decisions with overlapping subproblems | “minimum/maximum cost”, sequential choices, state reuse |
| `graph-degree/` | Role identification via in/out-degree signature | “trusted by everyone”, “points to nobody”, special node by counts |
| `graphs/` | Connectivity / traversal on explicit graphs or grids | reachability, components, flood fill, BFS/DFS |
| `range-processing/` | Index-based reasoning (streams, last-seen, group-by, diff tricks) | “minimum distance”, many updates, aggregation at the end |
| `simulation-state-machine/` | Deterministic transitions over finite state | repeated steps, direction/position state, cycles/modulo |
| `union-find/` | Connectivity under constraints, many queries | “are u and v connected?”, static edges, offline construction |

## Examples in this repo

- Dynamic programming
  - `dynamic-programming/minimum-distance-to-type-a-word-using-two-fingers.md`
- Graph degree signature
  - `graph-degree/find-the-town-judge.md`
- Graph / grid traversal
  - `graphs/number-of-enclaves.md`
- Range processing (streams + updates)
  - `range-processing/minimum-absolute-distance-between-mirror-pairs.md`
  - `range-processing/minimum-distance-between-three-equal-elements-i.md`
  - `range-processing/xor-after-range-multipplication-queries.md`
  - `range-processing/xor-after-range-multiplication-queries-ii.md`
- Simulation / state machine
  - `simulation-state-machine/walking-robot-simulation-ii.md`
- Union-find / connectivity queries
  - `union-find/path-existence-queries.md`

## Authoring conventions

- Notes are reasoning-first: decisions, invariants, and trade-offs matter more than code.
- Prefer clean stepwise structure (reframing → observations → brute force → insight → final approach).
- Keep code minimal and interview-ready: small state, explicit invariants, avoid over-engineering.
