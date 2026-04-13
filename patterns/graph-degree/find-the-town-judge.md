# Find the Town Judge

## Step 0: Reframing

- This is not about “rumors”; it’s a **graph identification** problem.
- Model people as nodes in a directed graph where an edge `a -> b` means “a trusts b”.
- The judge is the unique node with **out-degree = 0** (trusts nobody) and **in-degree = n-1** (trusted by everyone else).

## Step 1: Observations

- Property checks translate cleanly to degree constraints:
  - “Trusts nobody” ⟶ `outDegree[j] = 0`
  - “Trusted by everyone else” ⟶ `inDegree[j] = n - 1`
- Uniqueness is implied by degrees: two nodes cannot both have `inDegree = n-1` while also having `outDegree = 0` unless `n=1`.
- Constraints (`n <= 1000`, `|trust| <= 1e4`) favor an `O(n + m)` scan; no need for heavier graph algorithms.

## Step 2: Brute Force

- For each person `i`, verify:
  - they trust nobody (no outgoing edges),
  - everyone else trusts them (incoming from all other nodes).
- If implemented via adjacency checks, it becomes `O(n * (n + m))` (or `O(n^2)` with a matrix), which is unnecessary and more error-prone.

## Step 3: Optimization / Insight

- The key is that the judge conditions depend only on **counts**, not on path structure.
- One pass over `trust` can accumulate `inDegree[]` and `outDegree[]`, and a final pass can select the candidate matching the exact degree signature.

## Step 4: Final Approach

- Handle the degenerate case `n == 1` ⟶ the only person is trivially the judge.
- Maintain arrays:
  - `inDegree[i]`: number of people who trust `i`
  - `outDegree[i]`: number of people `i` trusts
- For each trust pair `(a, b)`:
  - increment `outDegree[a]`
  - increment `inDegree[b]`
- Scan `i = 1..n` and return the unique `i` such that:
  - `inDegree[i] == n - 1`
  - `outDegree[i] == 0`
- If none exist, return `-1`.

## Step 5: Complexity

- Time: `O(n + m)` where `m = trust.size()` (single accumulation pass + final scan).
- Space: `O(n)` for the two degree arrays.

## Step 6: Edge Cases

- `n == 1`, `trust` empty ⟶ return `1`.
- `trust` empty with `n > 1` ⟶ no one can have `inDegree = n-1` ⟶ `-1`.
- Cycles (e.g., judge trusts someone) immediately violate `outDegree == 0`.
- Multiple “popular” nodes: only the one with `outDegree == 0` can qualify, and degree checks disambiguate.

## Step 7: Implementation

```cpp
class Solution {
public:
    int findJudge(int n, vector<vector<int>>& trust) {
        if (n == 1) return 1;

        vector<int> indegree(n + 1, 0), outdegree(n + 1, 0);
        for (auto& t : trust) {
            int a = t[0], b = t[1];
            outdegree[a]++;
            indegree[b]++;
        }

        for (int i = 1; i <= n; i++) {
            if (indegree[i] == n - 1 && outdegree[i] == 0) return i;
        }
        return -1;
    }
};
```

## Step 8: Engineering Takeaways

- When the spec describes global social rules, translate them into **invariants on counts** (degrees) before reaching for graph traversal.
- Prefer **signature-based identification** (here: `(in=n-1, out=0)`) over building full adjacency when only aggregate facts are required.

## Pattern Mapping

- Pattern name: Directed graph degree signature
- Core technique: Track `inDegree` / `outDegree` and match a required pair of constraints
- Signals: “everyone points to X”, “X points to nobody”, “identify the special node”
- When to apply: Problems that define a role by *who points to whom*, not by reachability
- Common mistakes:
  - forgetting the `n==1` case
  - using a matrix / nested checks despite small needed state
  - mixing 0/1-indexing when allocating degree arrays

## Wrong Approaches Considered

- “Pick the most trusted person”:
  - Fails because the most trusted node might still trust someone (violates judge condition).
- BFS/DFS from every node:
  - Solves a different problem (reachability). The judge conditions are local degree constraints, not connectivity.
