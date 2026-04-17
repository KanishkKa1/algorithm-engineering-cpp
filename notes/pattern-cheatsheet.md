# Pattern Cheatsheet

This is a practical index of the patterns used in `patterns/` with the “tell” (how to recognize them) and the minimal state to maintain.

---

## Streaming / last-seen map

- **Tell**: “minimum distance”, “closest pair”, “first time condition becomes true” while scanning indices.
- **State**: hash map from *derived key* -> latest index.
- **Example**: `patterns/range-processing/minimum-absolute-distance-between-mirror-pairs.md`
- **Common failure**: storing first occurrence (maximizes distance) instead of last.

---

## Degree signature (graph identification)

- **Tell**: problem defines a special node by who points to whom, not by reachability.
- **State**: `inDegree[]`, `outDegree[]`.
- **Example**: `patterns/graph-degree/find-the-town-judge.md`
- **Common failure**: building adjacency / running BFS when only counts matter.

---

## Boundary flood fill (grid connectivity)

- **Tell**: “enclosed”, “cannot reach boundary”, “surrounded region”.
- **State**: visited set (or mutate grid in-place); queue/stack.
- **Example**: `patterns/graphs/number-of-enclaves.md`
- **Common failure**: per-cell DFS (quadratic) instead of boundary-first marking.

---

## Union-Find (static connectivity + many queries)

- **Tell**: many connectivity queries, graph edges are static or can be built offline.
- **State**: DSU parent/rank/size.
- **Example**: `patterns/union-find/path-existence-queries.md`
- **Common failure**: BFS/DFS per query (recomputes work).

---

## Finite-state simulation with cycles

- **Tell**: repeated `step(k)` calls, deterministic transitions, bounded state space.
- **State**: explicit state tuple + cycle length; reduce steps by modulo.
- **Example**: `patterns/simulation-state-machine/walking-robot-simulation-ii.md`
- **Common failure**: step-by-step simulation when the system is cyclic.

---

## DP with state normalization

- **Tell**: sequential decisions with long-range effects; greedy feels plausible but breaks.
- **State**: minimal representation (often anchored on “last action/position”).
- **Example**: `patterns/dynamic-programming/minimum-distance-to-type-a-word-using-two-fingers.md`
- **Common failure**: encoding symmetric/duplicate state that doubles work and bugs initialization.
