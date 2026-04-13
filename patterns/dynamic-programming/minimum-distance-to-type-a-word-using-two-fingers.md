# Minimum Distance to Type a Word Using Two Fingers

---

## leetcode 1320

link - <https://leetcode.com/problems/minimum-distance-to-type-a-word-using-two-fingers/>

---

## Step 0: Reframing

- Model the keyboard as a fixed grid graph; each letter is a node with a coordinate.
- Typing the word is a sequence of required targets; for each target we choose which of two cursors (fingers) moves.
- Cost is only the distance traveled by the chosen cursor. The other cursor is a cached position we may want to preserve.
- Initial positions are free: each finger can be "placed" on the first letter it ever presses at zero cost (spawn-on-first-use).

---

## Step 1: Observations

- Coordinates are deterministic with 6 columns: `idx -> (idx/6, idx%6)` where `idx = letter - 'A'`.
  - Manhattan distance is then O(1) without any precomputation.
- Exactly one finger moves per character; this is an assignment problem over time where state is (positions of both fingers).
- Straight DP state `(i, f1, f2)` is already feasible: `n <= 300`, `26^2 = 676` => ~200k states.
  - But it duplicates symmetric states (`(f1,f2)` vs `(f2,f1)`) and forces a more complex initialization (unused finger / free placement).
- State reduction is available:
  - After typing `word[i]`, some finger must be at `word[i]`.
  - Therefore the full configuration can be represented as: last typed position = `word[i]` plus the other finger position `j`.
  - This removes finger identity and collapses symmetry naturally.

---

## Step 2: Brute Force

- Idea: for each character decide which finger presses it (2 choices each step).
- Complexity: `O(2^n)` sequences; even with pruning it is exponential.
- Why it fails: decisions have long-range effects because moving one finger now changes the future baseline for many steps.

---

## Step 3: Optimization / Insight

- Key breakthrough: normalize the state by anchoring one finger to the previously typed character.
- After finishing index `i-1`, the set of finger positions must be `{ word[i-1], someOtherPos }`.
  - We do not need to remember which physical finger is where; only the set matters for future costs.
- When typing `word[i]`, there are exactly two exhaustive moves:
  1) move the finger currently on `word[i-1]` to `word[i]`
  2) move the other finger from `j` to `word[i]` (and then the "other" becomes `word[i-1]`)
- Correctness intuition:
  - The DP enumerates all possible `someOtherPos = j` reachable after `i-1`.
  - Every real execution corresponds to exactly one DP state, and every DP transition corresponds to a legal move.

---

## Step 4: Final Approach

Define DP on the reduced state.

- Let `dp[i][j]` be the minimum cost after typing `word[0..i]` (inclusive) such that:
  - one finger is on `word[i]` (the last typed character), and
  - the other finger is on letter index `j` (`0..25`).

Initialization (handles free start cleanly):

- `dp[0][j] = 0` for all `j`.
  - Interpretation: after typing the first character with one finger, the other finger could have been placed anywhere for free.

Transition for `i >= 1` (let `prev = word[i-1]`, `curr = word[i]` as letter indices):

- Keep the other finger at `j`, move the last-typed finger:  
  `dp[i][j] = min(dp[i][j], dp[i-1][j] + dist(prev, curr))`

- Move the other finger from `j` to `curr`; the previous last-typed position `prev` becomes the new "other":  
  `dp[i][prev] = min(dp[i][prev], dp[i-1][j] + dist(j, curr))`

Answer:

- `min_j dp[n-1][j]`.

Trade-off note:

- This reduced DP is `O(n*26)` and is also easier to reason about because every step is a two-choice transition.
- You can further compress memory to `O(26)` with a rolling array, but `n*26` is small enough to keep the table for clarity.

---

## Step 5: Complexity

- Time: `O(n * 26)`
  - For each `i` we iterate all `j` and do two O(1) relaxations.
- Space: `O(n * 26)` as written.
  - Can be reduced to `O(26)` by keeping only previous row.

---

## Step 6: Edge Cases

- Free initial placement:
  - Common bug is accidentally charging distance for the first use of a finger.
  - Here it is handled by `dp[0][j]=0` for all `j`.
- Repeated letters:
  - `dist(x,x)=0` so transitions naturally allow zero-cost repeats (including pressing the same letter with either finger).
- `n=2`:
  - Single transition; both moves are considered and minimum is returned.

---

## Step 7: Implementation (C++)

```cpp
class Solution {
  public:
    int dist(int a, int b) {
      if (a == -1) {
        return 0;
        }
        int x1 = a / 6, y1 = a % 6;
        int x2 = b / 6, y2 = b % 6;
        return abs(x1 - x2) + abs(y1 - y2);
    }

    int minimumDistance(string word) {
      int n = (int)word.size();

        vector<vector<int>> dp(n, vector<int>(26, INT_MAX));
        for (int j = 0; j < 26; j++) {
          dp[0][j] = 0;
        }

        for (int i = 1; i < n; i++) {
          int curr = word[i] - 'A';
            int prev = word[i - 1] - 'A';

            for (int j = 0; j < 26; j++) {
              if (dp[i - 1][j] == INT_MAX) {
                continue;
                }
                // Use the finger currently at prev.
                dp[i][j] = min(dp[i][j], dp[i - 1][j] + dist(prev, curr));

                // Use the other finger currently at j; then prev becomes the other.
                dp[i][prev] = min(dp[i][prev], dp[i - 1][j] + dist(j, curr));
            }
        }

        int ans = INT_MAX;
        for (int j = 0; j < 26; j++) {
          ans = min(ans, dp[n - 1][j]);
        }
        return ans;
    }
};
```

---

## Step 8: Engineering Takeaways

- For multi-agent DP, spend time on the state: the right state makes transitions trivial and proofs short.
- If agents are interchangeable, do not encode identities unless needed; symmetry usually buys you a factor.
- "Free initial placement" often means you want an explicit "unconstrained" initialization instead of special-casing the first few moves.

---

## Pattern Mapping

- Pattern name: Dynamic Programming (two cursors / assignment DP)
- Core technique: state normalization + two-way relaxation per step
- Signals:
  - two movable resources (fingers/robots/cursors)
  - sequential stream of targets (string/waypoints)
  - cost is a function of last positions (metric distance)
- When to apply:
  - two robots visiting points in order with minimal travel
  - two pointers serving requests with movement cost
- Common mistakes:
  - charging cost for initial placement
  - keeping `(f1,f2)` without removing symmetry (doubles work, complicates initialization)
  - greedy "use the closer finger" decisions

---

## Wrong Approaches Considered

- Greedy: always move the closer finger
  - Fails because the locally cheaper move can destroy a valuable parked position needed for a far future character.
- One-finger baseline (ignore the second finger)
  - Always valid but misses the optimization; it is only an upper bound.
- Full DP `(i, f1, f2)` without careful modeling of "unused" finger
  - Works, but many implementations accidentally charge the first move or mishandle symmetry; more code, more bug surface.
