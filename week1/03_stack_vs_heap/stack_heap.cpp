/**
 * EXERCISE 3: Stack vs Heap Memory
 *
 * THE BIGGEST DIFFERENCE FROM JAVA:
 * In Java: ALL objects live on the heap, managed by GC.
 * In C++: YOU choose where objects live.
 *
 * Stack: Fast, automatic lifetime, limited size (~1-8 MB)
 *   - Local variables, function parameters
 *   - Destroyed automatically when scope ends
 *
 * Heap: Slower, manual lifetime, large
 *   - Allocated with new/malloc, freed with delete/free
 *   - Persists until explicitly freed
 *   - Memory leaks if you forget to free!
 *
 * In embedded systems:
 *   - Stack is often very limited (a few KB per task in RTOS)
 *   - Heap may be unavailable or forbidden in safety-critical code
 *   - Prefer stack allocation whenever possible
 *
 * TASKS:
 * 1. Identify stack vs heap in code snippets
 * 2. Fix memory leaks
 * 3. Implement a stack-only buffer (no heap allocation)
 * 4. Understand object lifetime
 */

#include <iostream>

// ---- TASK 1: Read and understand ----
// For each variable below, identify: Stack or Heap?
// Write your answers in the comments

void identify_memory() {
  int x = 10;               // Stack or Heap? ->
  int *p = new int(20);     // p is on ___? The int it points to is on ___?
  int arr[100];             // Stack or Heap? ->
  int *darr = new int[100]; // darr is on ___? The 100 ints are on ___?

  // In Java: int[] arr = new int[100]; is ALWAYS heap.
  // In C++:  int arr[100]; is STACK. int* arr = new int[100]; is HEAP.

  delete p;
  delete[] darr;
}

// ---- TASK 2: Fix the memory leaks ----
// This function has memory leaks. Find and fix them.
// Hint: every 'new' needs a corresponding 'delete'
struct Sensor {
  int id;
  double value;
  Sensor(int i, double v) : id(i), value(v) {}
};

void leaky_function() {
  Sensor *s1 = new Sensor(1, 25.5);
  Sensor *s2 = new Sensor(2, 30.0);

  double *readings = new double[1000];
  for (int i = 0; i < 1000; i++) {
    readings[i] = i * 0.1;
  }

  if (s1->value > 20.0) {
    std::cout << "Sensor " << s1->id << " above threshold\n";
    return; // BUG: leaks s1, s2, and readings!
  }

  std::cout << "Processing...\n";

  // TODO: Fix the leaks. Make sure memory is freed in ALL paths.
  // Better yet: think about whether heap allocation is even needed here.
  delete s1;
  delete s2;
  delete[] readings;
}

// ---- TASK 3: Stack-only ring buffer ----
// In embedded systems, you often need fixed-size buffers with no heap
// allocation. Implement a simple ring buffer that lives entirely on the stack.
//
// A ring buffer is circular: when it reaches the end, it wraps around.
// Used heavily in embedded for: UART buffers, sensor data, log messages.

template <typename T, std::size_t N> struct RingBuffer {
  T data[N];             // Fixed-size array on the stack
  std::size_t head = 0;  // Write position
  std::size_t tail = 0;  // Read position
  std::size_t count = 0; // Number of elements currently stored

  // TODO: Push an element into the buffer.
  // If buffer is full, overwrite the oldest element.
  bool push(const T &value) {
    // TODO
    return true;
  }

  // TODO: Pop an element from the buffer.
  // Return false if buffer is empty.
  bool pop(T &out) {
    // TODO
    return false;
  }

  bool empty() const { return count == 0; }
  bool full() const { return count == N; }
  std::size_t size() const { return count; }
};

// ---- TASK 4: Object Lifetime ----
// Add print statements to constructor/destructor to observe lifetime

struct LifetimeDemo {
  const char *name;

  LifetimeDemo(const char *n) : name(n) {
    // TODO: Print "[name] constructed"
  }

  ~LifetimeDemo() {
    // TODO: Print "[name] destroyed"
  }
};

void demonstrate_lifetime() {
  std::cout << "--- Entering function ---\n";
  LifetimeDemo a("stack_obj");

  {
    std::cout << "--- Entering inner scope ---\n";
    LifetimeDemo b("inner_scope_obj");
    std::cout << "--- Leaving inner scope ---\n";
  } // b is destroyed here

  LifetimeDemo *c = new LifetimeDemo("heap_obj");
  // Notice: heap_obj is NOT destroyed when function returns unless we delete
  // it! In Java, GC would handle this. In C++, this is a leak.

  // TODO: delete c to prevent the leak

  std::cout << "--- Leaving function ---\n";
} // a is destroyed here

// ============ TEST CODE ============

void test_ring_buffer() {
  RingBuffer<int, 4> rb;

  // Push 1, 2, 3, 4
  rb.push(1);
  rb.push(2);
  rb.push(3);
  rb.push(4);
  std::cout << "[RingBuffer] size after 4 pushes: " << rb.size()
            << (rb.size() == 4 ? "  PASS" : "  FAIL") << "\n";

  // Push 5 — should overwrite oldest (1)
  rb.push(5);
  std::cout << "[RingBuffer] size after overflow push: " << rb.size()
            << (rb.size() == 4 ? "  PASS" : "  FAIL") << "\n";

  // Pop should give 2 (oldest surviving element)
  int val;
  rb.pop(val);
  std::cout << "[RingBuffer] first pop: " << val
            << (val == 2 ? "  PASS" : "  FAIL") << "\n";

  // Pop remaining: 3, 4, 5
  rb.pop(val);
  std::cout << "[RingBuffer] pop: " << val << (val == 3 ? "  PASS" : "  FAIL")
            << "\n";
  rb.pop(val);
  std::cout << "[RingBuffer] pop: " << val << (val == 4 ? "  PASS" : "  FAIL")
            << "\n";
  rb.pop(val);
  std::cout << "[RingBuffer] pop: " << val << (val == 5 ? "  PASS" : "  FAIL")
            << "\n";

  std::cout << "[RingBuffer] empty: " << (rb.empty() ? "PASS" : "FAIL") << "\n";
}

int main() {
  std::cout << "=== Exercise 3: Stack vs Heap ===\n\n";

  std::cout << "-- Lifetime Demo --\n";
  demonstrate_lifetime();
  std::cout << "\n";

  std::cout << "-- Ring Buffer --\n";
  test_ring_buffer();

  return 0;
}
