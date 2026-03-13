/**
 * EXERCISE 2: References vs Pointers
 *
 * C++ references are similar to Java references but with key differences:
 * - Must be initialized when declared (cannot be null)
 * - Cannot be reseated (always refer to the same object)
 * - No need to dereference — used like the original variable
 * - Preferred over pointers when nullptr is not a valid case
 *
 * In embedded C++, references are preferred for:
 * - Function parameters (avoids copy, guarantees non-null)
 * - Returning from functions (when lifetime is guaranteed)
 *
 * TASKS:
 * 1. Implement clamp() using references
 * 2. Implement find_first_above() returning a reference
 * 3. Implement Stats struct accumulator using references
 * 4. Understand dangling reference danger
 */

#include <iostream>
#include <limits>

// TASK 1: Clamp a value between min and max (modify in-place via reference)
// In Java: you'd return the clamped value since you can't modify primitives
// In C++: you can modify the original variable through a reference
void clamp(int &value, int min_val, int max_val) {
  // TODO: If value < min_val, set it to min_val
  //       If value > max_val, set it to max_val
}

// TASK 2: Find and return a reference to the first element above threshold
// This allows the caller to MODIFY the found element directly.
// Throws std::runtime_error if no element found.
//
// Think about it: in Java, you'd return the index or the object.
// In C++, returning a reference lets the caller modify the original array
// element.
int &find_first_above(int *arr, std::size_t size, int threshold) {
  // TODO: Find and return reference to first element > threshold
  // If none found, throw std::runtime_error("No element above threshold")
  throw std::runtime_error("No element above threshold");
}

// TASK 3: A simple statistics accumulator
// Complete the methods using references to avoid unnecessary copies
struct Stats {
  double sum = 0.0;
  double min_val = std::numeric_limits<double>::max();
  double max_val = std::numeric_limits<double>::lowest();
  std::size_t count = 0;

  // TODO: Take value by const reference (const double&)
  // Update sum, min_val, max_val, and count
  void add(/* fix parameter */) {}

  // TODO: Return average. Return by value (double), not reference. Why?
  // (Because the average is computed, not stored — no object to reference)
  double average() const { return 0.0; }

  // TODO: Print stats. Take ostream by reference.
  // "const" after method name means this method doesn't modify the object.
  // Java equivalent: this is like a method that only reads fields.
  void print(std::ostream &os) const {
    // TODO: Print count, sum, min, max, average
  }
};

// TASK 4: DANGER — Dangling References
// Uncomment and understand why this is dangerous:
//
// int& create_value() {
//     int local = 42;
//     return local;  // WARNING: returning reference to local variable!
//     // 'local' is destroyed when function returns — reference is dangling
//     // In Java, the GC would keep the object alive. In C++, it's YOUR
//     problem.
// }

// ============ TEST CODE — DO NOT MODIFY BELOW ============

void test_clamp() {
  int a = 150, b = -5, c = 50;
  clamp(a, 0, 100);
  clamp(b, 0, 100);
  clamp(c, 0, 100);
  bool pass = (a == 100 && b == 0 && c == 50);
  std::cout << "[Clamp] a=" << a << " b=" << b << " c=" << c;
  std::cout << (pass ? "  PASS" : "  FAIL") << "\n";
}

void test_find_first_above() {
  int data[] = {1, 5, 3, 8, 2, 9};
  int &found = find_first_above(data, 6, 4);
  std::cout << "[FindAbove] found=" << found;
  bool pass1 = (found == 5);

  // Modify through the reference — this should change the original array!
  found = 99;
  bool pass2 = (data[1] == 99);
  std::cout << " modified_array[1]=" << data[1];
  std::cout << (pass1 && pass2 ? "  PASS" : "  FAIL") << "\n";
}

void test_stats() {
  Stats s;
  double values[] = {3.0, 7.0, 1.0, 9.0, 5.0};
  for (int i = 0; i < 5; i++) {
    // s.add(values[i]);  // Uncomment after fixing add() parameter
  }
  std::cout << "[Stats] ";
  s.print(std::cout);
  std::cout << "\n";
}

int main() {
  std::cout << "=== Exercise 2: References ===\n\n";
  test_clamp();
  test_find_first_above();
  test_stats();
  return 0;
}
