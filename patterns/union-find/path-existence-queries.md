# Path Existence Queries in a Graph

---

Leetcode Link - <https://leetcode.com/problems/find-if-path-exists-in-graph/description/>

---

## Step 0: Reframing

- We have `q` connectivity queries over a *static* graph implicitly defined by a sorted array.
- Nodes are indices `0..n-1`.
- Edge rule: connect indices `i` and `j` if `|nums[i] - nums[j]| <= maxDiff`.
- The key is the sorted order: value differences are monotonic with index movement, so connectivity collapses to **adjacent feasibility**.

This is “build components once, answer many queries”.

---

## Step 1: Observations

- If the array is sorted, then for any `i < j`:
  - If every adjacent gap on the path is small (`nums[t] - nums[t-1] <= maxDiff`), then `i` and `j` are connected (via the chain).
  - If there exists a gap `> maxDiff` between them, no path can cross it (graph splits).
- Therefore, connected components are just **contiguous segments** separated by “large gaps”.

---

## Step 2: Brute Force

### Idea

For each query `(u, v)`:

1. Construct graph:
   - Add edge between every pair `(i, j)` where `|nums[i] - nums[j]| ≤ maxDiff`
2. Run BFS/DFS from `u`
3. Check if `v` is reachable

---

### Complexity

- Edge construction: `O(n²)`
- BFS per query: `O(n + e)`
- Total: `O(q * n²)`

---

### Why it fails

- Recomputes the same graph structure for every query
- Quadratic edge construction is too expensive
- Does not leverage **static nature of connectivity**

---

## Step 3: Optimization / Insight

- Build connectivity once using only adjacent pairs:
  - For each `i = 1..n-1`, if `nums[i] - nums[i-1] <= maxDiff`, connect `i-1` and `i`.
- Then each query is a “same component?” check.

Two valid implementations:

- DSU (Union-Find): general connectivity machinery.
- Component labeling: linear scan to assign component IDs (simpler here).

---

## Step 4: Final Approach

### Option A (DSU)

1. Initialize DSU for `n` indices.
2. Union adjacent indices where the gap is `<= maxDiff`.
3. Answer each query by checking if `find(u) == find(v)`.

### Option B (Component labeling)

1. Scan left to right and assign a component ID.
2. Start a new component whenever `nums[i] - nums[i-1] > maxDiff`.
3. Answer each query by checking if `comp[u] == comp[v]`.

Option B is optimal for this structure; Option A is more reusable as a pattern.

---

## Step 5: Complexity

- Build:
  - DSU: `O(n α(n))`
  - Labeling: `O(n)`
- Query:
  - DSU: `O(α(n))` per query
  - Labeling: `O(1)` per query
- Space: `O(n)`

---

## Step 6: Edge Cases

- `n == 1`: every query is trivially true.
- All adjacent gaps `> maxDiff`: every node is isolated.
- All adjacent gaps `<= maxDiff`: one component.
- `u == v`: always true (same node).

---

## Step 7: Implementation (DSU, C++)

```cpp
class Solution {
public:
    vector<int> parent, rankv;

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int a, int b) {
        int pa = find(a), pb = find(b);
        if (pa == pb) return;
        if (rankv[pa] < rankv[pb]) swap(pa, pb);
        parent[pb] = pa;
        if (rankv[pa] == rankv[pb]) rankv[pa]++;
    }

    vector<bool> pathExistenceQueries(int n, vector<int>& nums, int maxDiff,
                                      vector<vector<int>>& queries) {
        parent.resize(n);
        rankv.assign(n, 0);
        for (int i = 0; i < n; ++i) parent[i] = i;

        for (int i = 1; i < n; ++i) {
            if (nums[i] - nums[i - 1] <= maxDiff) unite(i, i - 1);
        }

        vector<bool> ans;
        ans.reserve(queries.size());
        for (const auto& q : queries) {
            ans.push_back(find(q[0]) == find(q[1]));
        }
        return ans;
    }
};
```

---

## Step 8: Engineering Takeaways

- Sorted order can convert a global constraint into a local adjacency rule.
- For “many queries on a static structure”, precompute once and answer fast.
- DSU is powerful, but always ask whether the graph is actually a simple 1D segmentation problem.

---

## Pattern Mapping

- Pattern name: `static connectivity + many queries`
- Core technique: build components once (DSU or labeling), answer membership queries
- Signals:
  - multiple connectivity queries
  - no edge updates between queries
  - sorted/monotonic structure enabling local unions
- Common mistakes:
  - BFS/DFS per query
  - trying to connect all pairs (`O(n^2)`) instead of adjacent only

---

## Wrong Approaches Considered

- Building all edges `(i, j)` where `|nums[i] - nums[j]| <= maxDiff`:
  - unnecessary `O(n^2)` work; sorted order makes adjacent unions sufficient.
