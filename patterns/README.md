# Patterns

This folder groups algorithmic problems by the underlying problem-solving patterns used to solve them, rather than by contest platform or problem source.

The goal is to help you:

- identify the right pattern for a new problem
- reuse existing solutions and techniques
- think in terms of structures and strategies instead of ad hoc casework

## How to use this folder

1. Read the problem statement and look for the core behavior:
   - is it about connectivity under changing constraints?
   - does it simulate a state machine or repeat transitions?
   - does it require maintaining ranges, counts, degrees, or parity?
2. Open the matching pattern subfolder.
3. Study the example problems, notes, and templates.
4. Reuse the pattern names and approaches when solving similar problems.

## Available categories

- `dynamic-programming/` -
- `graph-degree/` — role identification via in/out-degree signatures in directed graphs
- `graph/` -
- `range-processing/` — range updates, queries, and aggregate transforms
- `simulation-state-machine/` — state-driven transitions, cyclic behavior, and repeated simulation
- `union-find/` — connectivity under constraints, component merging, and dynamic connectivity

## Notes

- Each category contains problem notes, explanations, and implementation examples.
- This structure is designed to support learning by pattern, not by specific contest names.
- New categories should be added when a new recurring technique emerges.

## Contributing

To add a new pattern:

1. Create a new folder with a descriptive name.
2. Add a `README.md` or problem notes file explaining the pattern.
3. Add example problems and code templates if available.
4. Keep the explanation focused on when the pattern applies and how it is recognized.
