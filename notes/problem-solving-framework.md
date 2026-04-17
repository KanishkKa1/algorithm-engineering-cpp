# Algorithmic Problem Solving Framework (Engineering-Oriented)

This framework is optimized for *repeatable decision making*: quickly eliminate infeasible approaches, isolate the true abstraction, and justify the final design with invariants.

---

## Step 0: Reframe (name the abstraction)

Ask: what is the problem *really* about?

- Connectivity under constraints (DSU / BFS-once)
- Streaming + last-seen (min distance, first collision)
- Range aggregation / offline updates
- Finite-state simulation (position + direction, cycles)
- DP over minimal state (overlapping subproblems)

Weak: “this is a graph problem”  
Strong: “this is role identification by in/out-degree signature”

---

## Step 1: Extract constraints into a budget

Constraint sizes should directly kill approaches.

| Scale | Usually feasible |
|---:|---|
| `n <= 20` | `O(2^n)` |
| `n <= 300` | `O(n * 26^2)` |
| `n <= 2e3` | `O(n^2)` |
| `n <= 1e5` | `O(n)` / `O(n log n)` |
| Many queries | precompute once; answer fast |

Rule: don’t “optimize later”. Pick an approach that fits the budget from the start.

---

## Step 2: Brute force (correctness first)

- Write the simplest correct idea.
- Compute complexity (including nested loops, digit-cost, graph edges).
- Identify *why* it fails under the constraints (time or memory).

The goal is to make the failure mode obvious: *what work is being repeated?*

---

## Step 3: Identify repeated work (the optimization lever)

Typical sources of wasted work:

- Recomputing connectivity per query (should be build-once)
- Re-scanning ranges for each endpoint (should be prefix / last-seen / offline)
- Simulating each step in a cyclic system (should be modulo + chunking)
- DP with symmetric state (should normalize/remove identity)

---

## Step 4: Define minimal state + invariant

You want the smallest state that makes transitions local and proofs short.

Examples:

- Sliding window: invariant “no duplicates in window”
- DSU: invariant “same parent ⇒ connected”
- Simulation: state `(x, y, dir)` and deterministic transition
- DP: state must uniquely represent all future-relevant information

If you can’t state the invariant in one sentence, the solution is not stable yet.

---

## Step 5: Data structure justification

For the final design, explicitly answer:

- Why this DS / technique is sufficient (and what it tracks)
- Why simpler alternatives fail
- Why heavier alternatives are unnecessary

This is what turns a “working solution” into an “interview-ready solution”.

---

## Step 6: Edge cases (validate the model)

Target the edges that break invariants:

- Empty / size-1 inputs
- “all equal”, “all distinct”
- Degenerate geometry (width=1 / height=1)
- Full cycles (`k % cycle == 0`) in simulations
- `u == v` / repeated queries

---

## Step 7: Implementation discipline

- Keep code surface area small (fewer moving parts = fewer bugs).
- Prefer explicit names over cleverness.
- Avoid special casing by choosing the right initialization (common DP bug source).

---

## Post-mortem (how you actually get better)

After solving:

- What signal did you miss early?
- Which wrong approach was most tempting and why?
- What is the reusable pattern name for this problem?
