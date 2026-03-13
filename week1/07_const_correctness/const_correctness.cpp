/**
 * EXERCISE 7: const Correctness
 *
 * In Java: 'final' only prevents reassignment of the variable.
 * In C++: 'const' is much more powerful and pervasive:
 *   - const variable: value cannot change
 *   - const pointer: pointer or pointee (or both) can't change
 *   - const reference: can't modify through the reference
 *   - const method: method promises not to modify the object
 *   - const parameter: function won't modify the argument
 *
 * Rule of thumb: "Use const everywhere you can."
 * This helps the compiler optimize and catches bugs at compile time.
 *
 * In embedded:
 *   - const data can be placed in ROM/Flash (saves RAM!)
 *   - const methods are safe to call from interrupt handlers
 *   - constexpr enables compile-time computation
 *
 * TASKS:
 * 1. Fix const-related compilation errors
 * 2. Make a class const-correct
 * 3. Understand const with pointers
 * 4. Use constexpr for compile-time computation
 */

#include <iostream>
#include <cstdint>
#include <cstring>

// ---- TASK 1: Fix the const errors ----
// Each function below has a const-correctness bug. Fix them.

// Bug 1: This function should not modify the array. Add const.
int sum_array(int* arr, std::size_t size) {
    int total = 0;
    for (std::size_t i = 0; i < size; i++) {
        total += arr[i];
    }
    return total;
}

// Bug 2: This function should not modify the string. Fix the parameter.
std::size_t string_length(char* str) {
    std::size_t len = 0;
    while (str[len] != '\0') len++;
    return len;
}

// Bug 3: This function accidentally modifies the input. Fix it.
void print_buffer(uint8_t* buf, std::size_t size) {
    for (std::size_t i = 0; i < size; i++) {
        // buf[i] = 0;  // Oops! This was supposed to be a read-only operation
        std::cout << static_cast<int>(buf[i]) << " ";
    }
    std::cout << "\n";
}

// ---- TASK 2: Make this class const-correct ----
// Add 'const' to methods that don't modify the object.
// Add 'const' to parameters where appropriate.

struct SensorConfig {
    int id;
    char name[32];
    double min_range;
    double max_range;
    double resolution;

    // TODO: Which of these methods should be const?

    int get_id() { return id; }

    char* get_name() { return name; }  // Think carefully about this one!
    // Hint: should there be a const AND non-const version?

    double get_range() { return max_range - min_range; }

    bool is_in_range(double value) {
        return value >= min_range && value <= max_range;
    }

    void set_range(double min_val, double max_val) {
        min_range = min_val;
        max_range = max_val;
    }

    // TODO: Add a print method that takes a const ostream reference
    // void print(std::ostream& os) { ... }
};

// ---- TASK 3: Const and Pointers — understand the differences ----
void pointer_const_demo() {
    int value = 42;
    int other = 99;

    // Read each declaration carefully and understand what is const:

    const int* p1 = &value;      // Pointer to const int
    // *p1 = 10;                  // ERROR: can't modify the int
    // p1 = &other;               // OK: can change what p1 points to

    int* const p2 = &value;      // Const pointer to int
    // *p2 = 10;                  // OK: can modify the int
    // p2 = &other;               // ERROR: can't change what p2 points to

    const int* const p3 = &value; // Const pointer to const int
    // *p3 = 10;                  // ERROR
    // p3 = &other;               // ERROR

    // TODO: Write a comment explaining when you'd use each form in embedded:
    // p1 (const int*):
    // p2 (int* const):
    // p3 (const int* const):

    // Trick to remember: read RIGHT to LEFT
    // const int* → "pointer to int that is const" → can't change the int
    // int* const → "const pointer to int" → can't change the pointer
}

// ---- TASK 4: constexpr — Compile-Time Computation ----
// constexpr means the computation happens at COMPILE TIME.
// The result is baked into the binary — no runtime cost!
// Perfect for embedded: lookup tables, configuration values.

// TODO: Make these constexpr
int square(int x) {
    return x * x;
}

double celsius_to_fahrenheit(double c) {
    return c * 9.0 / 5.0 + 32.0;
}

// TODO: Create a constexpr function to calculate a bitmask
// For example: bitmask(3) should return 0b00001000 (1 << 3)
// This is used constantly in embedded for register manipulation

// TODO: Create a constexpr lookup table for sine values (approximate)
// Use a constexpr array with pre-computed values for 0°, 30°, 45°, 60°, 90°
// In embedded, lookup tables in Flash save CPU cycles vs computing sin() at runtime

// ============ TEST CODE ============

int main() {
    std::cout << "=== Exercise 7: const Correctness ===\n\n";

    // Test Task 1
    int data[] = {1, 2, 3, 4, 5};
    std::cout << "Sum: " << sum_array(data, 5) << "\n";

    char msg[] = "Hello";
    std::cout << "Length: " << string_length(msg) << "\n";

    uint8_t buf[] = {0xDE, 0xAD, 0xBE, 0xEF};
    print_buffer(buf, 4);

    // Test Task 2
    SensorConfig cfg{1, "Temp", -40.0, 125.0, 0.1};
    std::cout << "Range: " << cfg.get_range() << "\n";
    std::cout << "50.0 in range: " << cfg.is_in_range(50.0) << "\n";

    // Test Task 4
    // After making square constexpr:
    // constexpr int sq = square(5);  // Computed at compile time!
    // static_assert(sq == 25, "square(5) should be 25");

    std::cout << "\nAll tasks in this exercise require you to ADD const.\n";
    std::cout << "The code compiles without const, but it's not CORRECT.\n";
    std::cout << "Const correctness catches bugs at compile time!\n";

    return 0;
}
