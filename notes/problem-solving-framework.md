# Algorithmic Problem Solving Framework (Engineering-Oriented)

This framework is used to solve algorithmic problems with a focus on reasoning, constraint analysis, and reusable patterns.  
The goal is not just to solve problems, but to understand *why* a solution works and when it applies.

---

## Step 0: Problem Reframing

- What is the problem fundamentally asking?
- Is it:
  - Connectivity?
  - Optimization?
  - Counting?
  - Validation under constraints?

> Weak signal: “This is a graph problem”  
> Strong signal: “This is connectivity under constraints”

---

## Step 1: Problem Analysis

- Clearly define:
  - Inputs
  - Outputs
  - Constraints
- Extract keywords:
  - contiguous → sliding window
  - sorted → binary search
  - limit/threshold → constraint filtering

---

## Step 2: Constraint Filtering

- Use input size to eliminate infeasible approaches

| Input Size | Expected Complexity |
|-----------|-------------------|
| n ≤ 20 | O(2^n) |
| n ≤ 1e3 | O(n²) |
| n ≤ 1e5 | O(n log n) / O(n) |

---

## Step 3: Brute Force (Correctness First)

- Build the simplest correct solution
- Explicitly compute complexity
- Identify where it fails

---

## Step 4: Recomputations (Optimization Lever)

- What work is repeated?
- Is the same computation done multiple times?
- Can results be reused?

> This step bridges brute force → optimal

---

## Step 5: Monotonicity Detection

- Does increasing input relax constraints?
- Can the problem be processed incrementally?

Common signals:

- Increasing limit
- Expanding valid set
- No rollback required

---

## Step 6: Pattern Mapping

Only after previous steps:

- Sliding Window → local constraints on subarray
- Binary Search → monotonic answer space
- DSU → dynamic connectivity
- DP → overlapping subproblems
- Greedy → locally optimal leads to global optimum

---

## Step 7: State / Invariant Definition

- Define the minimal state required
- Define invariant clearly

Example:

- Sliding window → no duplicates in window
- DSU → nodes in same component are connected

> If invariant breaks → solution fails

---

## Step 8: Data Structure Justification

- Why this data structure?
- Why not alternatives?

Example:

- DSU vs BFS per query → avoids recomputation
- Array vs map → cache-friendly, constant time

---

## Step 9: Transition Logic

- How does state evolve?
- What triggers state updates?

---

## Step 10: Edge Case Validation

Actively test:

- Empty input
- Single element
- Maximum constraints
- Uniform values

---

## Step 11: Implementation

- Clean, readable, minimal
- Avoid unnecessary abstractions

---

## Post-Mortem (Critical)

After solving:

- What signal did I miss?
- What incorrect assumption did I make?
- Can this pattern generalize?
