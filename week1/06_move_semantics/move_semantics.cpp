/**
 * EXERCISE 6: Move Semantics & Ownership
 *
 * Move semantics is unique to C++. No equivalent in Java.
 *
 * The problem: Copying large objects is expensive.
 * The solution: Instead of copying, MOVE resources from one object to another.
 *               The source becomes empty/invalid after the move.
 *
 * Think of it like moving houses:
 * - Copy: Buy new furniture identical to the old one (expensive)
 * - Move: Load furniture onto a truck and bring it to the new house (cheap)
 *         The old house is now empty.
 *
 * Key concepts:
 * - lvalue: Has a name, persists. (int x = 5; x is an lvalue)
 * - rvalue: Temporary, about to die. (x + 1 is an rvalue)
 * - && : rvalue reference — binds to temporaries
 * - std::move: Casts an lvalue to an rvalue reference (says "I'm done with this")
 *
 * Why it matters for embedded:
 * - Avoids unnecessary memory allocations
 * - Enables efficient transfer of buffers/resources
 * - Zero-cost abstraction — the compiler optimizes moves
 *
 * TASKS:
 * 1. Implement a move-aware DataBuffer class
 * 2. Understand copy vs move behavior
 * 3. Implement the Rule of Five
 */

#include <iostream>
#include <cstdint>
#include <cstring>
#include <utility>

// ---- TASK 1 & 3: Move-aware DataBuffer ----
// This class manages a heap-allocated buffer.
// Implement copy constructor, move constructor, copy assignment, move assignment, destructor.
// This is the "Rule of Five" — if you define any of these, define all five.

class DataBuffer {
public:
    // Constructor
    DataBuffer(std::size_t size, const char* label = "unnamed")
        : size_(size), label_(label) {
        data_ = new uint8_t[size];
        std::memset(data_, 0, size);
        std::cout << "  [" << label_ << "] Constructed (size=" << size_ << ")\n";
    }

    // Destructor
    ~DataBuffer() {
        // TODO: Free the data_ buffer
        // Print: "[label] Destroyed"
        // Handle case where data_ might be nullptr (after move)
    }

    // Copy Constructor — deep copy
    // Java equivalent: clone() method
    DataBuffer(const DataBuffer& other) {
        // TODO: Allocate new buffer and copy data from 'other'
        // This is expensive! We allocate new memory and copy every byte.
        std::cout << "  [" << label_ << "] COPY constructed from [" << other.label_ << "]\n";
    }

    // Move Constructor — steal resources
    // No Java equivalent! This is unique to C++.
    DataBuffer(DataBuffer&& other) noexcept {
        // TODO: Steal data_, size_ from 'other'
        // Set other's data_ to nullptr and size_ to 0
        // This is cheap! Just pointer assignments, no memory allocation.
        std::cout << "  [" << label_ << "] MOVE constructed from [" << other.label_ << "]\n";
    }

    // Copy Assignment Operator
    DataBuffer& operator=(const DataBuffer& other) {
        // TODO: Handle self-assignment (if &other == this)
        // Free existing data, allocate new, copy from other
        std::cout << "  [" << label_ << "] COPY assigned from [" << other.label_ << "]\n";
        return *this;
    }

    // Move Assignment Operator
    DataBuffer& operator=(DataBuffer&& other) noexcept {
        // TODO: Handle self-assignment
        // Free existing data, steal from other
        std::cout << "  [" << label_ << "] MOVE assigned from [" << other.label_ << "]\n";
        return *this;
    }

    // Fill buffer with a value
    void fill(uint8_t value) {
        if (data_) std::memset(data_, value, size_);
    }

    // Access
    uint8_t* data() { return data_; }
    std::size_t size() const { return size_; }
    const char* label() const { return label_; }

private:
    uint8_t* data_ = nullptr;
    std::size_t size_ = 0;
    const char* label_ = "";
};

// ---- TASK 2: Understanding when copy vs move happens ----

DataBuffer create_buffer() {
    DataBuffer buf(1024, "factory_buf");
    buf.fill(0xAA);
    return buf;  // Move (or copy elision — even better!)
}

void consume_buffer(DataBuffer buf) {
    std::cout << "  Consumed buffer: " << buf.label()
              << " size=" << buf.size() << "\n";
}

void task2_copy_vs_move() {
    std::cout << "\n-- Copy vs Move --\n";

    // Case 1: Copy
    DataBuffer a(512, "A");
    DataBuffer b = a;  // Copy constructor — a is an lvalue, we might use it later

    // Case 2: Move
    DataBuffer c = std::move(a);  // Move constructor — we told compiler "I'm done with a"
    // WARNING: 'a' is now in a "moved-from" state. Don't use it!
    std::cout << "  After move, a.size()=" << a.size()
              << " a.data()=" << (a.data() == nullptr ? "null" : "valid") << "\n";

    // Case 3: Return value — compiler may use move or elide copy entirely
    DataBuffer d = create_buffer();

    // Case 4: Passing to function
    DataBuffer e(256, "E");
    consume_buffer(std::move(e));  // Move into function parameter
}

// ============ TEST CODE ============

int main() {
    std::cout << "=== Exercise 6: Move Semantics ===\n";

    std::cout << "\n-- Construction & Destruction --\n";
    {
        DataBuffer buf(128, "test");
        buf.fill(0xFF);
    }  // Should print "Destroyed"

    task2_copy_vs_move();

    std::cout << "\n-- Move Assignment --\n";
    {
        DataBuffer x(64, "X");
        DataBuffer y(32, "Y");
        y = std::move(x);  // Move assignment: Y frees its data, steals from X
        std::cout << "  After move assign: x.size()=" << x.size()
                  << " y.size()=" << y.size() << "\n";
    }

    return 0;
}
