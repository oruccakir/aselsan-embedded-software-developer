/**
 * EXERCISE 1: Pointers & Pointer Arithmetic
 *
 * In Java, all objects are accessed via references (hidden pointers).
 * In C++, you have explicit control over pointers — this is critical
 * for embedded systems where you interact directly with memory addresses.
 *
 * KEY DIFFERENCES FROM JAVA:
 * - You can take the address of any variable (&var)
 * - You can dereference a pointer to access the value (*ptr)
 * - Pointer arithmetic lets you navigate memory directly
 * - There is NO garbage collector — you manage memory yourself
 *
 * TASKS:
 * 1. Complete swap_values() using pointers
 * 2. Complete find_max() using pointer arithmetic to traverse an array
 * 3. Complete reverse_array() in-place using two pointers
 * 4. Complete print_matrix() — pointer arithmetic with 2D arrays
 */

#include <iostream>

// TASK 1: Swap two integers using pointers
// Java equivalent: not possible with primitives — Java passes primitives by
// value
void swap_values(int *a, int *b) {
  // TODO: Swap the values that a and b point to
}

// TASK 2: Find the maximum value in an array using pointer arithmetic
// Do NOT use array indexing (arr[i]). Use pointer arithmetic instead.
// This is how embedded code often traverses buffers.
int find_max(const int *arr, std::size_t size) {
  // TODO: Traverse array using pointer arithmetic (ptr + offset or ptr++)
  // Return the maximum value found
  return 0;
}

// TASK 3: Reverse an array in-place using two pointers
// Use a front pointer and back pointer moving toward each other
void reverse_array(int *arr, std::size_t size) {
  // TODO: Use two pointers (front and back) to reverse the array
}

// TASK 4: Print a "2D array" stored as a flat buffer
// In embedded systems, matrices are often stored as flat 1D arrays.
// Use pointer arithmetic to access element at (row, col) in a rows x cols
// matrix.
void print_matrix(const int *matrix, std::size_t rows, std::size_t cols) {
  // TODO: Print the matrix row by row
  // Access element (r, c) as *(matrix + r * cols + c)
  // Format: print each row on a new line, values separated by spaces
}

// ============ TEST CODE — DO NOT MODIFY BELOW ============

void test_swap() {
  int x = 42, y = 17;
  swap_values(&x, &y);
  std::cout << "[Swap] x=" << x << " y=" << y;
  std::cout << (x == 17 && y == 42 ? "  PASS" : "  FAIL") << "\n";
}

void test_find_max() {
  int data[] = {3, 7, 2, 9, 1, 8, 4};
  int result = find_max(data, 7);
  std::cout << "[FindMax] max=" << result;
  std::cout << (result == 9 ? "  PASS" : "  FAIL") << "\n";
}

void test_reverse() {
  int data[] = {1, 2, 3, 4, 5};
  reverse_array(data, 5);
  bool pass = (data[0] == 5 && data[1] == 4 && data[2] == 3 && data[3] == 2 &&
               data[4] == 1);
  std::cout << "[Reverse] ";
  for (int i = 0; i < 5; i++)
    std::cout << data[i] << " ";
  std::cout << (pass ? " PASS" : " FAIL") << "\n";
}

void test_matrix() {
  // 3x4 matrix stored as flat array
  int mat[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  std::cout << "[Matrix] Expected 3x4 matrix:\n";
  print_matrix(mat, 3, 4);
}

int main() {
  std::cout << "=== Exercise 1: Pointers & Pointer Arithmetic ===\n\n";
  test_swap();
  test_find_max();
  test_reverse();
  test_matrix();
  return 0;
}
