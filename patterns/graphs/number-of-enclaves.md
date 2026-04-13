# Number of Enclaves

---

## leetcode 1020

link - <https://leetcode.com/problems/number-of-enclaves/>

---

## Reframing

- Nodes represent grid cells, edges connect adjacent land cells (4-directional).
- Connectivity defines "escapable" components touching boundaries.
- Enclaves are isolated land components with no boundary paths.

> This is a **grid connectivity problem** focusing on boundary isolation.

---

## Observations

- Boundary land cells and connected components can escape.
- Internal land cells in disconnected components are enclaves.
- Grid size up to 500x500 requires O(N*M) solution.
- 4-directional moves limit connectivity to orthogonal neighbors.

---

## Brute Force

### Idea

- For each land cell, DFS/BFS to check boundary reachability.
- Count cells that cannot reach boundary.

### Complexity

- Time: O((N*M)^2) worst case.
- Space: O(N*M) per traversal.

### Why it fails

- Quadratic time exceeds constraints.

---

## Optimization / Insight

- Start DFS from boundary land cells to mark escapable areas.
- Remaining land cells are enclaves.

### Key Property

> Boundary-connected components are escapable; others are not.

---

## Final Approach

1. DFS from all boundary land cells, marking visited (set to 0).
2. Count remaining 1s in the grid.

---

## Edge Cases

- All 0s: 0
- All 1s: 0
- Single internal 1: 1
- 1x1 with 1: 0

---

## Implementation (C++)

```cpp
class Solution {
private:
    void dfs(int row, int col, vector<vector<int>>& grid, int n, int m) {
        grid[row][col] = 0;
        int delrow[] = {-1, 0, 1, 0};
        int delcol[] = {0, 1, 0, -1};
        for (int i = 0; i < 4; i++) {
            int nrow = row + delrow[i];
            int ncol = col + delcol[i];
            if (nrow >= 0 && nrow < n && ncol >= 0 && ncol < m && grid[nrow][ncol] == 1) {
                dfs(nrow, ncol, grid, n, m);
            }
        }
    }

public:
    int numEnclaves(vector<vector<int>>& grid) {
        int n = grid.size();
        int m = grid[0].size();
        for (int j = 0; j < m; j++) {
            if (grid[0][j] == 1) dfs(0, j, grid, n, m);
            if (grid[n-1][j] == 1) dfs(n-1, j, grid, n, m);
        }
        for (int i = 0; i < n; i++) {
            if (grid[i][0] == 1) dfs(i, 0, grid, n, m);
            if (grid[i][m-1] == 1) dfs(i, m-1, grid, n, m);
        }
        int cnt = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (grid[i][j] == 1) cnt++;
            }
        }
        return cnt;
    }
};
```

---

## Decision

- **DFS** → Chosen for simplicity in grid problems.
- **BFS** → Alternative for large grids to avoid recursion depth.

---

## Engineering Takeaways

- Always consider boundary conditions in grid problems.
- In-place algorithms reduce space complexity.
- DFS vs BFS trade-offs depend on grid size.

---

## Pattern Mapping

- Pattern: Grid Flood Fill from Boundaries
- Core: DFS/BFS connectivity
- Signals: Grid, boundary constraints, enclosed regions
- When: Isolating internal components
- Mistakes: Missing boundary cells

---

## Post-Mortem

- Key signal: Boundary defines escapable regions.
- Brute force failed due to quadratic time.
- Insight: Reverse flood fill from boundaries.
- Optimization: Single pass marking.

---

## Generalization

This pattern applies to:

- Finding enclosed areas in grids.
- Island counting with boundary exclusion.
- Connectivity isolation problems.

---

## Wrong Approaches Considered

- Count internal 1s: Ignores connectivity.
- Per-cell check: Too slow.
