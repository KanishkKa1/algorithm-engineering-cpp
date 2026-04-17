# Minimum Absolute Distance Between Mirror Pairs

---

Leetcode link - <https://leetcode.com/problems/minimum-absolute-distance-between-mirror-pairs/>

---

## Step 0: Reframing

- This is not a digit-manipulation problem; digit reversal is just a deterministic key transform.
- The real task: in a single left-to-right scan, detect when the current value `nums[j]` matches `reverse(nums[i])` for some earlier index `i`, and minimize `j - i`.
- Abstraction: "nearest previous match under a derived key" (a classic `last-seen` / streaming minimum-distance pattern).

---

## Step 1: Observations

- A mirror pair is directional: we need `reverse(nums[i]) == nums[j]` with `i < j`. We do *not* need `reverse(nums[j]) == nums[i]`.
- For a fixed `j`, the best `i` is the **largest** index `< j` that still satisfies the condition (closest on the left), because that minimizes `j - i`.
- Therefore, we only need to remember the **most recent** index that can generate a given `reverse(nums[i])` value.
- Constraints (`n` up to `1e5`) force a near-linear approach; `O(n^2)` pair enumeration is dead on arrival.

---

## Step 2: Brute Force

### Idea

- For every `i < j`, check whether `reverse(nums[i]) == nums[j]`, and track `min(j - i)`.

### Complexity

- Time: `O(n^2 * D)` where `D <= 10` digits per number (still far too large for `n = 1e5`).
- Space: `O(1)`.

### Why it fails

- The constraint size makes quadratic scanning infeasible even with cheap digit reversal.

---

## Step 3: Optimization / Insight

### Key breakthrough

- Flip the work: instead of searching backwards for an `i` for each `j`, pre-index earlier elements by the value they would match in the future.

### What property enables optimization

- `reverse(x)` is deterministic, and the distance objective prefers the most recent valid `i`.
- So we maintain a hash map `lastSeen[target] = latest index i such that reverse(nums[i]) == target`.
- When we are at index `j`, if `lastSeen` contains `nums[j]`, we found the closest mirror pair ending at `j`.

---

## Step 4: Final Approach

1. Initialize `ans = +inf` and an empty hash map `lastSeen`.
2. Scan indices `j = 0..n-1`:
   - If `lastSeen` contains key `nums[j]`, update `ans = min(ans, j - lastSeen[nums[j]])`.
   - Compute `rev = reverse(nums[j])`.
   - Set `lastSeen[rev] = j` (this `j` becomes the best candidate `i` for any future element equal to `rev`).
   - Optional: if `ans == 1`, you can return early (cannot do better than distance `1`).
3. Return `ans` if updated; otherwise return `-1`.

---

## Step 5: Complexity

- Time: `O(n * D)` where `D <= 10` digits, effectively `O(n)`.
  - Each element does one reverse and `O(1)` average hash operations.
- Space: `O(n)` in the worst case for the hash map (all reversals distinct).

---

## Step 6: Edge Cases

- No mirror pair exists: `ans` stays `+inf` -> return `-1`.
- Duplicate candidates for the same key: keeping only the **latest** index is correct because it minimizes `j - i` for future `j`.
- Leading zeros after reversal are naturally removed by integer construction (e.g., `120 -> 21`).
- Best possible answer is `1` (since `i < j`), so early exit is safe.

---

## Step 7: Implementation (C++)

```cpp
class Solution {
private:
    int rev(int x) {
        int r = 0;
        while (x > 0) {
            r = r * 10 + (x % 10);
            x /= 10;
        }
        return r;
    }

public:
    int minMirrorPairDistance(vector<int>& nums) {
        unordered_map<int, int> lastSeen; // key: reverse(nums[i]), value: latest i
        int ans = INT_MAX;

        for (int j = 0; j < (int)nums.size(); ++j) {
            auto it = lastSeen.find(nums[j]);
            if (it != lastSeen.end()) {
                ans = min(ans, j - it->second);
                if (ans == 1) return 1;
            }

            lastSeen[rev(nums[j])] = j;
        }

        return ans == INT_MAX ? -1 : ans;
    }
};
```

---

## Step 8: Engineering Takeaways

- When you need a minimum index distance, ask: "for each position, what information about the past is sufficient?" Often it's just *the most recent occurrence* under the right key.
- If a problem has a transformation (`reverse`, `normalize`, `hash`) and then asks for relationships, convert it into a streaming lookup by indexing the transformed values.
- Directionality matters: confirm whether the definition is symmetric before assuming you can match both ways.

---

## Pattern Mapping

- Pattern name: `range processing / last-seen map`
- Core technique: maintain `lastSeen` for derived keys to compute nearest valid pair
- Signals:
  - "minimum distance" / "closest pair" over indices
  - a deterministic transform that defines a match (`reverse(x)`)
  - large `n` that rules out pair enumeration
- When to apply:
  - when for each `j`, the best `i` is the closest valid index on the left
- Common mistakes:
  - storing the first-seen index (maximizes distance)
  - treating the condition as symmetric (`reverse(nums[j]) == nums[i]`) when it's directional
  - recomputing backwards scans instead of indexing forward targets

---

## Wrong Approaches Considered

- **Sort by value / reverse-value**: destroys index order, and distance is index-based, not value-based.
- **Store all indices per key**: unnecessary; only the latest index can ever be optimal for future `j`.
- **Two-way matching (checking both `reverse(nums[i]) == nums[j]` and `reverse(nums[j]) == nums[i]`)**: adds work and can report pairs that are not valid under the problem's directional definition.
