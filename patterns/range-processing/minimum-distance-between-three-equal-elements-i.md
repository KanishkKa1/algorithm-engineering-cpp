# Minimum Distance Between Three Equal Elements I

---

link - <https://leetcode.com/problems/minimum-distance-between-three-equal-elements-i/>

---

## Step 0: Reframing

- The input is effectively a sequence of values, and the task is to identify three equal values whose positions are as close together as possible.
- The core abstraction is not about ordering the values, but about minimizing the distance span of a triple of identical elements.
- This fits the `range-processing` mindset because the answer depends only on the span between the first and last chosen indices for a fixed value.

---

## Step 1: Observations

- For any triple of indices `(i, j, k)` with `i <= j <= k`, the distance formula simplifies:
  - `|i - j| + |j - k| + |k - i| = (j - i) + (k - j) + (k - i) = 2 * (k - i)`.
- The only part that matters is the outermost indices among the three equal values.
- If a value appears at positions `p0 < p1 < ... < pm`, the best triple for that value is the one with the smallest span `p[t+2] - p[t]`.
- There is no need to consider every ordering of the indices because the sorted order gives the minimum distance for a fixed triple.

---

## Step 2: Brute Force

### Idea

- Enumerate all triples `(i, j, k)` with `i < j < k`.
- Check whether `nums[i] == nums[j] == nums[k]`.
- Compute the distance and track the minimum.

### Complexity

- `O(n^3)` time for `n <= 100` gives at most one million checks.
- This is workable under the problem constraints, but it ignores structure and misses the simpler insight.

### Why it fails as a polished solution

- It is correct, but not elegant.
- It ignores the key structure of the distance formula and the value-grouping opportunity.
- It would be harder to scale if the constraints were larger.

---

## Step 3: Optimization / Insight

### Key breakthrough

- The distance only depends on the first and last positions of the chosen triple, not on the middle position explicitly.
- For a fixed value, the smallest distance is achieved by the smallest span between any two positions that are two steps apart in the sorted list of occurrences.

### What enables optimization

- Group indices by value.
- Use the sorted list of positions for each value.
- Only inspect windows of size three rather than all triples.

---

## Step 4: Final Approach

1. Build a map from value to the list of indices where it occurs.
2. For each value that occurs at least three times:
   - sort its index list (indices are naturally increasing if collected in order).
   - scan consecutive triples of indices: `(pos[i], pos[i+1], pos[i+2])`.
   - compute the distance as `2 * (pos[i+2] - pos[i])`.
   - keep the minimum distance across all values.
3. If no value has three occurrences, return `-1`.

This approach is both direct and optimal for the problem structure.

---

## Step 5: Complexity

- Time complexity: `O(n + u)` where `u` is the total number of distinct values processed.
  - Building index groups is `O(n)`.
  - Scanning each group is linear in the group size.
- Space complexity: `O(n)` for the index groups.

---

## Step 6: Edge Cases

- `nums` has fewer than three elements: return `-1`.
- No value appears three times: return `-1`.
- A value appears exactly three times: the answer is `2 * (last - first)` for that value.
- Positions are already monotonic in the input, so no extra sorting is needed when collecting indices in order.

---

## Step 7: Implementation (C++)

```cpp
class Solution {
public:
    int minimumDistance(vector<int>& nums) {
        int n = nums.size();
        unordered_map<int, vector<int>> positions;

        for (int i = 0; i < n; ++i) {
            positions[nums[i]].push_back(i);
        }

        int best = INT_MAX;
        for (auto& [value, pos] : positions) {
            if (pos.size() < 3) {
                continue;
            }
            for (int i = 0; i + 2 < pos.size(); ++i) {
                int span = pos[i + 2] - pos[i];
                best = min(best, 2 * span);
            }
        }

        return best == INT_MAX ? -1 : best;
    }
};
```

---

## Step 8: Engineering Takeaways

- When a distance formula simplifies, use the simplified form to reduce the search space.
- Grouping by value is a powerful way to turn a triple search into a linear scan over occurrences.
- Always look for monotonic structure in index-based problems.

---

## Pattern Mapping

- Pattern name: `range processing`
- Core technique: grouping by value and scanning fixed-size windows of positions
- Signals:
  - repeated values with index-based distance costs
  - a distance formula that collapses to a span
  - small constraint sizes where structural simplification is enough
- When to apply:
  - when identical values are the key combinatorial element
  - when distance depends on the earliest and latest occurrence in a group
- Common mistakes:
  - trying to compute the metric using only adjacent duplicates
  - using full triple enumeration without exploiting the span property

---

## Wrong Approaches Considered

- Enumerating all triples directly: correct but unnecessarily expensive and not insight-driven.
- Using only the minimum pairwise distance for each value: wrong, because three indices require the outer span, not just the closest pair.
- Recording only the last two occurrences for each value: wrong when a smaller span exists earlier in the list.
