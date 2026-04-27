# Check if There is a Valid Path in a Grid

---

Leetcode 1391

Link - <https://leetcode.com/problems/check-if-there-is-a-valid-path-in-a-grid/>

---

## Step 0: Reframing

- This is not a free 4-direction grid traversal problem.
- Each cell is a constrained street segment with only two open sides.
- Movement from one cell to another is valid only when both cells agree on the connection:
  - current cell allows movement toward the neighbor
  - neighbor cell allows entry from the opposite direction

So the grid is really an implicit graph where cells are nodes and valid street connections are bidirectional edges.

---

## Step 1: Observations

- Every street type has a fixed set of exactly two allowed directions.
- A move cannot be validated by looking only at the current cell.
- The neighbor must expose the reverse direction; otherwise the streets do not connect.
- The graph is unweighted, so reachability from `(0, 0)` can be checked using BFS or DFS.
- Constraints go up to `300 x 300`, so visiting every cell once is acceptable.
- Each cell has constant degree, so the traversal remains linear in the number of cells.

---

## Step 2: Brute Force

### Idea

For every cell:

1. Try moving in all four directions.
2. Check whether the move is inside the grid.
3. Check if the street types are compatible.
4. Build a full graph of valid edges.
5. Run BFS/DFS from `(0, 0)` to check if `(m - 1, n - 1)` is reachable.

---

### Complexity

- Edge construction: `O(n * m * 4)`
- Traversal: `O(n * m)`
- Space for graph adjacency: `O(n * m)`

---

### Why it is unnecessary

- The graph does not need to be materialized.
- Each cell already tells us its possible outgoing directions.
- Compatibility can be checked lazily during BFS.

The brute force graph-building version is correct, but it stores edges that can be derived on demand.

---

## Step 3: Optimization / Insight

The key insight is:

```text
Valid edge = current allows direction AND neighbor allows reverse direction
```

Instead of constructing the whole graph, run BFS directly on the grid:

- From the current street, enumerate only its allowed directions.
- For each candidate neighbor, verify that the neighbor has the opposite direction.
- If yes, it is a real bidirectional connection and can be visited.

This keeps the graph implicit and avoids extra adjacency storage.

---

## Step 4: Final Approach

1. Define direction mappings for street types `1` to `6`.
2. Start BFS from `(0, 0)`.
3. Maintain a visited matrix to avoid revisiting cells.
4. For the current cell:
   - get its allowed directions
   - compute the neighbor coordinate for each direction
   - skip out-of-bound or already visited neighbors
   - check whether the neighbor contains the reverse direction
5. If the destination `(n - 1, m - 1)` is reached, return `true`.
6. If BFS finishes without reaching the destination, return `false`.

The traversal follows only real street connections, so reaching the destination means a valid path exists.

---

## Step 5: Complexity

- Time complexity: `O(n * m)`
  - Each cell is visited at most once.
  - Each street has exactly two possible directions.
  - Reverse-direction checking is constant work.
- Space complexity: `O(n * m)`
  - `visited` matrix
  - BFS queue in the worst case

---

## Step 6: Edge Cases

- Single cell grid:
  - start and destination are the same cell, so return `true`.
- Start cell has no compatible outgoing neighbor:
  - BFS stops immediately and returns `false`.
- Neighbor is in range but not reverse-compatible:
  - movement must be rejected.
- Path may bend through different street types:
  - valid as long as every adjacent transition is compatible.
- Large grid:
  - BFS remains safe because each cell is processed once.

---

## Step 7: Implementation (C++)

```cpp
class Solution {
public:
    bool hasValidPath(vector<vector<int>>& grid) {
        int n = grid.size();
        int m = grid[0].size();

        vector<vector<vector<int>>> dir = {
            {},
            {{0, -1}, {0, 1}},  // 1: left, right
            {{-1, 0}, {1, 0}},  // 2: up, down
            {{0, -1}, {1, 0}},  // 3: left, down
            {{0, 1}, {1, 0}},   // 4: right, down
            {{0, -1}, {-1, 0}}, // 5: left, up
            {{0, 1}, {-1, 0}}   // 6: right, up
        };

        queue<pair<int, int>> q;
        vector<vector<int>> vis(n, vector<int>(m, 0));

        q.push({0, 0});
        vis[0][0] = 1;

        while (!q.empty()) {
            auto [row, col] = q.front();
            q.pop();

            if (row == n - 1 && col == m - 1) {
                return true;
            }

            for (auto& d : dir[grid[row][col]]) {
                int dr = d[0];
                int dc = d[1];

                int nrow = row + dr;
                int ncol = col + dc;

                if (nrow < 0 || nrow >= n || ncol < 0 || ncol >= m) {
                    continue;
                }

                if (vis[nrow][ncol]) {
                    continue;
                }

                for (auto& back : dir[grid[nrow][ncol]]) {
                    if (back[0] == -dr && back[1] == -dc) {
                        vis[nrow][ncol] = 1;
                        q.push({nrow, ncol});
                        break;
                    }
                }
            }
        }

        return false;
    }
};
```

---

## Step 8: Engineering Takeaways

- Grid problems are often implicit graph problems.
- Do not assume 4-direction movement just because the input is a grid.
- When movement has constraints, validate the edge from both endpoints.
- Build adjacency lists only when they add value; here, the graph can be derived lazily.
- BFS is enough because the problem asks reachability, not shortest path or optimal cost.

---

## Pattern Mapping

- Pattern name: `implicit graph traversal with bidirectional constraints`
- Core technique: BFS/DFS over derived edges
- Signals:
  - grid cells encode movement rules
  - movement between adjacent cells requires compatibility
  - target is reachability, not optimization
  - constraints allow visiting every cell once
- When to apply:
  - street/pipe/tile connection problems
  - maze traversal with directional constraints
  - graph edges are not explicitly given but can be computed from local state
- Common mistakes:
  - moving based only on the current cell
  - ignoring reverse compatibility of the neighbor
  - trying all four directions instead of only allowed street exits
  - building a full adjacency list unnecessarily

---

## Wrong Approaches Considered

- Treating the grid as a normal 4-direction maze:
  - fails because cells do not allow arbitrary movement.
- Checking only whether the current street allows the outgoing direction:
  - fails when the neighbor does not connect back.
- Materializing the full graph before traversal:
  - correct but unnecessary extra storage.
- Using DSU:
  - possible, since the problem is connectivity, but BFS is simpler because there is only one source-target query.
