# Walking Robot Simulation II

---

## leetcode 2069

link - <https://leetcode.com/problems/walking-robot-simulation-ii/>

--

## Step 0: Reframing

- The robot moves along the boundary of a rectangular grid.
- Movement is constrained by grid limits and direction changes at boundaries.
- The system behaves as a **finite state machine** defined by position `(x, y)` and direction.
- The robot’s path is cyclic and repeats after completing one full perimeter traversal.
- Each step operation advances the robot along this cycle.
- The problem reduces to simulating transitions over a **cyclic perimeter path**.

> This is a **state machine simulation over a cyclic path with boundary-driven transitions**.

---

## Step 1: Observations

- The robot never enters the inner grid; it only traverses the **perimeter** of the rectangle.
- Movement is deterministic: at any point, the next state depends only on `(x, y, dir)`.
- When hitting a boundary, the robot **turns left (counterclockwise)** and continues movement.
- The total unique path length is finite and equals the **perimeter = 2 * (width + height) - 4**.
- Movement forms a **cycle**, meaning after completing one full loop, the robot returns to `(0,0)` facing South.
- Large step counts can be reduced using modulo with the perimeter.
- Direction changes only occur at **corner points**, not during straight movement.

---

## Step 2: Brute Force Simulation

### Idea

- Simulate the robot step-by-step:
  - For each step:
    - Try moving forward in current direction
    - If out of bounds:
      - Rotate 90° counterclockwise
      - Retry movement
- Maintain state:
  - Position `(x, y)`
  - Direction `dir`

---

### Complexity

- Each `step(num)` takes `O(num)`
- Worst case:
  - `num = 10^5`
  - Up to `10^4` calls

👉 Total: O(total_steps) ≈ 10^9 → Too slow

---

### Why it fails

- Simulates each step individually → no compression
- Does not leverage **cyclic nature of movement**
- Repeats identical cycles for large `num`
- Fails under large constraints due to time limit

---

### Key Limitation

> The brute force approach treats movement as linear,  
> while the actual system is **cyclic and repeatable**

## Step 3: Optimization Insight

- The robot’s movement is cyclic and repeats after completing one full traversal of the perimeter.
- The total number of unique positions visited in one cycle is:
  - `perimeter = 2 * (width + height) - 4`
- Therefore, moving `num` steps is equivalent to moving:
  - `num % perimeter` steps

---

### Cyclic Behavior Insight

> The system exhibits **cyclic behavior**, so large step counts can be reduced using modulo without changing the final state.

---

### Subtle Edge Case

- If `num % perimeter == 0`:
  - The robot completes a full cycle
  - But direction depends on whether it returns to the starting point

---

## Step 4: State Design

The system can be modeled as a **finite state machine**.

### State Variables

- `x` → current x-coordinate  
- `y` → current y-coordinate  
- `dir` → current direction  

---

### Direction Encoding

| Value  | Direction  |
|--------|------------|
| 0      | East       |
| 1      | North      |
| 2      | West       |
| 3      | South      |

---

### State Invariant

- `(x, y)` is always on the **boundary of the grid**
- `dir` represents the direction robot will attempt to move next

---

## Step 5: Transition Logic

- At each step:
  - Move in the current direction until:
    - either steps are exhausted
    - or boundary is reached

---

### Movement Rules

- If moving within bounds:
  - update `(x, y)`
- If boundary is reached:
  - rotate direction counterclockwise
  - continue movement

---

### Efficient Transition

Instead of moving one step at a time:

- Compute maximum possible movement in current direction
- Move in chunks:
  - East → `min(num, w - 1 - x)`
  - North → `min(num, h - 1 - y)`
  - West → `min(num, x)`
  - South → `min(num, y)`

---

#### Movement Insight

> Movement is **piecewise linear along edges**, not step-by-step simulation.

---

## Step 6: Edge Cases

- `num % perimeter == 0`:
  - Special handling required to maintain correct direction
  - If robot returns to `(0,0)`, it should face **South**

- Single row (`height = 1`) or single column (`width = 1`):
  - Degenerates into a straight line (handle carefully)

- Starting position `(0,0)`:
  - Direction behavior differs when completing full cycles

- Large `num`:
  - Must use modulo to avoid TLE

- Corner transitions:
  - Ensure direction changes correctly at boundaries

---

### Critical Edge Case

> Returning to origin after full cycle does NOT reset direction to East.

## Step 7: Complexity

### Time Complexity

- Each `step(num)` operation:
  - Reduces steps using modulo → `num % perimeter`
  - Movement happens in **at most 4 directional segments (edges)**

👉 Therefore:

- Each `step()` call runs in **O(1)** time

---

### Why O(1)?

- Robot moves along rectangle edges
- In one full cycle, it traverses 4 edges:
  - bottom → right → top → left
- After modulo reduction, movement is bounded by perimeter
- Each edge transition is handled in constant time

---

### Overall Complexity

- `step()` → O(1)
- `getPos()` → O(1)
- `getDir()` → O(1)

---

### Space Complexity

- O(1)
- Only storing:
  - position `(x, y)`
  - direction `dir`

---

## Step 8: Implementation (C++)

```cpp
class Robot {
public:
    int w, h;
    int x = 0, y = 0;
    int dir = 0; // 0: East, 1: North, 2: West, 3: South
    int perimeter;

    Robot(int width, int height) : w(width), h(height) {
        perimeter = 2 * (w + h) - 4;
    }

    void step(int num) {
        if (perimeter == 0) return;

        num %= perimeter;

        // Special case: full cycle returns to origin but changes direction
        if (num == 0) {
            if (x == 0 && y == 0) {
                dir = 3; // South
            }
            return;
        }

        while (num > 0) {
            if (dir == 0) { // East
                int move = min(num, w - 1 - x);
                x += move;
                num -= move;
                if (num > 0) dir = 1;
            }
            else if (dir == 1) { // North
                int move = min(num, h - 1 - y);
                y += move;
                num -= move;
                if (num > 0) dir = 2;
            }
            else if (dir == 2) { // West
                int move = min(num, x);
                x -= move;
                num -= move;
                if (num > 0) dir = 3;
            }
            else { // South
                int move = min(num, y);
                y -= move;
                num -= move;
                if (num > 0) dir = 0;
            }
        }
    }

    vector<int> getPos() {
        return {x, y};
    }

    string getDir() {
        if (dir == 0) return "East";
        if (dir == 1) return "North";
        if (dir == 2) return "West";
        return "South";
    }
};

---

# 🔥 Important Correction (don’t ignore this)

You might casually say:
> “loop is there → O(n)”

❌ Wrong.

👉 Because:
- `num` is reduced via modulo
- transitions are **edge-based, not step-based**

---

# 🧠 What interviewer expects from you here

If asked:
> “Why is this O(1)?”

You must say:

> “Because movement is bounded by perimeter and processed in constant directional segments, not per-step simulation.”

---

# ⚠️ Subtle bug check (I’m testing you)

Look at this:

```cpp
if (num == 0) {
    if (x == 0 && y == 0) {
        dir = 3;
    }
}

## Step 9: Engineering Takeaways

- Always check for **cyclic behavior** in simulation problems
  - Repeated patterns can be compressed using modulo

- Avoid naive step-by-step simulation when:
  - movement is repetitive
  - constraints are large

- Model problems as **state machines** when:
  - behavior depends on current state (position + direction)
  - transitions are deterministic

- Convert global movement into **local transitions**
  - handle movement edge-by-edge instead of step-by-step

- Identify **implicit structure**
  - here: robot never explores full grid → only perimeter

- Handle edge cases explicitly:
  - full cycle (`num % perimeter == 0`)
  - direction at origin after cycle

- Optimize based on constraints:
  - large `num` → requires cycle compression

---

## Pattern Mapping

- **Pattern**: Simulation + State Machine  
- **Core Technique**: Cyclic Path Compression  

---

### Signals

- Repeated movement over fixed path  
- Deterministic transitions based on state  
- Large number of steps (`num` up to 1e5)  
- Boundary-driven direction changes  
- Finite number of unique states  

---

### Key Abstractions

- **State** → `(x, y, dir)`  
- **Cycle** → perimeter of rectangle  
- **Transition** → directional movement + boundary rotation  

---

### When to Apply This Pattern

- Robot / movement simulation problems  
- Circular or perimeter traversal problems  
- Problems with:
  - repeated steps
  - fixed path
  - deterministic rules  

---

### Common Mistakes

- Simulating each step individually → leads to TLE  
- Ignoring cyclic nature of system  
- Not modeling direction as part of state  
- Missing special case when completing full cycle  

---

## Post-Mortem

- Initial intuition: simulate movement step-by-step  
- Bottleneck identified: repeated traversal of same path  
- Key breakthrough: recognizing **perimeter cycle**  
- Final optimization: reduce steps using modulo and process in segments  

---

## Generalization

This approach applies to:

- Circular arrays (modulo traversal)  
- Grid boundary traversal problems  
- State machine simulations with finite cycles  
- Any system where:
  - state repeats after fixed number of steps  

> Always ask:
> “Does this system repeat? If yes, what is the cycle length?”
