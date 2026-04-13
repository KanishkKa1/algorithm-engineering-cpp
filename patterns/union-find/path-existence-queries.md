# Path Existence Queries in a Graph

---

## Step 0: Reframing

- Nodes represent indices of the array.
- An edge exists between two nodes if `|nums[i] - nums[j]| ≤ maxDiff`.
- Since the array is sorted, value differences increase monotonically with index distance.
- Connectivity is therefore **local and transitive via adjacent elements**.
- The graph reduces to **connected components formed by contiguous valid segments**.
- Each query asks whether two indices belong to the same component.

> This is a **connectivity validation problem under a local, monotonic constraint**.

---

## Step 3: Brute Force

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

## Step 4: Recomputations

- Connectivity between nodes depends only on `nums` and `maxDiff`
- Queries do NOT change the graph
- We repeatedly:
  - rebuild edges
  - recompute connectivity

### Key Observation

> The graph is **static**, but queries are **multiple**

👉 Precompute once, reuse many times

---

## Step 5: Key Insight

- Since the array is sorted:
  - If `nums[i]` and `nums[j]` satisfy constraint,
    then all intermediate adjacent pairs must also satisfy it
- Therefore:
  - Connectivity can be built using **only adjacent elements**

### Critical Property

> If a long-range connection is valid, it must be achievable through a chain of adjacent valid differences.

---

## Step 6: Optimal Approach

1. Traverse array once:
   - If `nums[i] - nums[i-1] ≤ maxDiff`, connect `i` and `i-1`
2. This forms connected components (segments)
3. For each query:
   - Check if both nodes belong to same component

---

## Step 7: Data Structure Choice

### Option 1: DSU (Chosen)

- Efficiently maintains connected components
- Amortized near O(1) operations

---

### Option 2: Component Labeling (Alternative 🔥)

Instead of DSU:

- Traverse once and assign component IDs

```cpp
vector<int> comp(n);
comp[0] = 0;
int id = 0;

for (int i = 1; i < n; i++) {
    if (nums[i] - nums[i - 1] <= maxDiff)
        comp[i] = id;
    else
        comp[i] = ++id;
}
```

### Query

```cpp
comp[u] == comp[v];
```

## Insight

- DSU is not necessary here
- A simple linear scan + component labeling is sufficient

---

## Step 8: Trade-offs

| Approach              | Pros                         | Cons                |
|---------------------|------------------------------|---------------------|
| DSU                 | General, reusable            | Slight overhead     |
| Component labeling  | Simpler, faster in practice  | Problem-specific    |
| BFS per query       | Easy to reason               | Too slow            |

This trade-off analysis follows the "Data Structure Justification" step in the framework, weighing alternatives against problem requirements.

---

## Decision

- **DSU** → Good for general connectivity problems
- **Component labeling** → Optimal for this linear structure

---

## Step 9: Complexity

### DSU Approach

- Build: `O(n α(n))`
- Query: `O(α(n))`

### Labeling Approach

- Build: `O(n)`
- Query: `O(1)`

---

## Step 10: Edge Cases

- Single element → trivially connected
- All differences > maxDiff → all nodes isolated
- Entire array valid → single component
- Queries where `u == v` → always true

---

## Step 11: Implementation (DSU)

```cpp
class Solution {
public:
    vector<int> parent, rankv;

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int a, int b) {
        int pa = find(a);
        int pb = find(b);
        if (pa == pb) return;

        if (rankv[pa] < rankv[pb]) {
            parent[pa] = pb;
        } else if (rankv[pb] < rankv[pa]) {
            parent[pb] = pa;
        } else {
            parent[pb] = pa;
            rankv[pa]++;
        }
    }

    vector<bool> pathExistenceQueries(int n, vector<int>& nums, int maxDiff,
                                      vector<vector<int>>& queries) {
        parent.resize(n);
        rankv.resize(n, 0);

        for (int i = 0; i < n; i++)
            parent[i] = i;

        for (int i = 1; i < n; i++) {
            if (nums[i] - nums[i - 1] <= maxDiff) {
                unite(i, i - 1);
            }
        }

        vector<bool> ans;
        for (auto& q : queries) {
            ans.push_back(find(q[0]) == find(q[1]));
        }
        return ans;
    }
};
```

---

## Step 12: Engineering Takeaways

- Always check if the graph is **implicit and structured**
- Sorted order can convert **global constraints → local constraints**
- Separate concerns:
  - **Static computation** → build once
  - **Dynamic queries** → answer efficiently
- DSU is powerful, but sometimes **overkill**
- Prefer simpler solutions when the problem structure allows

---

## Pattern Mapping

- **Pattern**: Connected Components
- **Technique**: DSU / Linear Component Labeling

### Signals

- Repeated connectivity queries
- Static graph (no updates between queries)
- Local constraints leading to global connectivity

---

## Post-Mortem

- Key signal identified: **connectivity does not change across queries**
- Initial brute force failed due to **repeated graph reconstruction**
- Critical insight: **sorted order enables adjacency-based connectivity**
- Optimization came from:
  - eliminating recomputation
  - exploiting monotonic structure
- Alternative solution (labeling) revealed that:
  - DSU was **not required**, only convenient

This post-mortem follows the framework's emphasis on reflection to improve pattern recognition.

---

## Generalization

This pattern applies to:

- Grouping problems based on threshold differences
- Clustering contiguous valid segments
- Connectivity queries on **static datasets**
- Problems where:
  - constraints are local
  - but effects are global
