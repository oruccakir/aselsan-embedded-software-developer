/**
 * EXERCISE 5: Smart Pointers
 *
 * Smart pointers apply RAII to heap memory management.
 * They are the modern C++ answer to "who deletes this?"
 *
 * Three types:
 * - unique_ptr: Sole owner. Cannot be copied, only moved.
 *               Like having exactly one key to a locker.
 * - shared_ptr: Shared ownership via reference counting.
 *               Like a shared document — deleted when last person leaves.
 * - weak_ptr:   Non-owning observer of shared_ptr.
 *               Can check if the object still exists.
 *
 * Java analogy:
 * - Java references are like shared_ptr (with GC instead of ref counting)
 * - Java has no equivalent to unique_ptr (sole ownership)
 * - Java's WeakReference ≈ weak_ptr
 *
 * Embedded note:
 * - unique_ptr has ZERO overhead vs raw pointer (use freely!)
 * - shared_ptr has overhead (ref count, atomic ops) — use sparingly
 * - Many embedded codebases avoid shared_ptr entirely
 *
 * TASKS:
 * 1. Use unique_ptr for exclusive ownership
 * 2. Transfer ownership with std::move
 * 3. Use unique_ptr with custom deleters (for hardware resources)
 * 4. Use shared_ptr for shared ownership
 * 5. Understand weak_ptr to break circular references
 */

#include <iostream>
#include <memory>
#include <string>

// A simple sensor device for our exercises
struct SensorDevice {
  int id;
  std::string name;
  double last_reading;

  SensorDevice(int id, const std::string &name)
      : id(id), name(name), last_reading(0.0) {
    std::cout << "  [Sensor " << id << " '" << name << "'] Created\n";
  }

  ~SensorDevice() {
    std::cout << "  [Sensor " << id << " '" << name << "'] Destroyed\n";
  }

  double read() {
    last_reading = id * 10.0 + 0.5; // fake reading
    return last_reading;
  }
};

// ---- TASK 1: unique_ptr basics ----
// Create and use unique_ptr. Observe automatic cleanup.
void task1_unique_basics() {
  std::cout << "\n-- Task 1: unique_ptr basics --\n";

  // TODO: Create a unique_ptr<SensorDevice> using std::make_unique
  // auto sensor = ???

  // TODO: Access members using -> (just like raw pointer)
  // sensor->read();
  // std::cout << "Reading: " << sensor->last_reading << "\n";

  // TODO: Get raw pointer (non-owning) using .get()
  // SensorDevice* raw = sensor.get();

  // When this function returns, unique_ptr's destructor runs
  // and the sensor is automatically deleted. No memory leak possible!
}

// ---- TASK 2: Ownership transfer with std::move ----
// unique_ptr cannot be copied (who would own it?), only MOVED.
// Moving transfers ownership from one unique_ptr to another.

// This function takes ownership of the sensor
void configure_sensor(std::unique_ptr<SensorDevice> sensor) {
  std::cout << "  Configuring sensor: " << sensor->name << "\n";
  // sensor is destroyed at end of this function
}

void task2_ownership_transfer() {
  std::cout << "\n-- Task 2: Ownership Transfer --\n";

  auto sensor = std::make_unique<SensorDevice>(1, "Temperature");

  // TODO: This won't compile — why?
  // configure_sensor(sensor);  // ERROR: can't copy unique_ptr

  // TODO: Use std::move to transfer ownership
  // configure_sensor(std::move(sensor));

  // TODO: After move, sensor is nullptr. Check it:
  // std::cout << "  After move, sensor is "
  //           << (sensor == nullptr ? "null" : "valid") << "\n";
}

// ---- TASK 3: Custom Deleters ----
// In embedded, you might need custom cleanup (not just 'delete').
// Example: a hardware handle that needs a specific deinitialization call.

namespace HardwareAPI {
struct Handle {
  int id;
};

Handle *open_device(int id) {
  std::cout << "  [HW] Device " << id << " opened\n";
  return new Handle{id};
}

void close_device(Handle *h) {
  std::cout << "  [HW] Device " << h->id << " closed\n";
  delete h;
}
} // namespace HardwareAPI

void task3_custom_deleter() {
  std::cout << "\n-- Task 3: Custom Deleter --\n";

  // TODO: Create a unique_ptr with a custom deleter that calls close_device
  // Syntax: std::unique_ptr<Type, decltype(&deleter_function)>
  //
  // auto device = std::unique_ptr<HardwareAPI::Handle,
  // decltype(&HardwareAPI::close_device)>(
  //     HardwareAPI::open_device(42),
  //     HardwareAPI::close_device
  // );

  // When device goes out of scope, close_device is called instead of delete!
}

// ---- TASK 4: shared_ptr ----
// Multiple owners. Object is deleted when the last shared_ptr is destroyed.

struct DataLogger {
  std::string name;
  DataLogger(const std::string &n) : name(n) {
    std::cout << "  [Logger '" << name << "'] Created\n";
  }
  ~DataLogger() { std::cout << "  [Logger '" << name << "'] Destroyed\n"; }
  void log(const std::string &msg) {
    std::cout << "  [" << name << "] " << msg << "\n";
  }
};

void task4_shared_ptr() {
  std::cout << "\n-- Task 4: shared_ptr --\n";

  // TODO: Create a shared_ptr<DataLogger>
  // auto logger = std::make_shared<DataLogger>("MainLog");

  // TODO: Create another shared_ptr pointing to the same logger (copy)
  // auto logger2 = logger;

  // TODO: Check reference count with use_count()
  // std::cout << "  ref count: " << logger.use_count() << "\n";  // should be 2

  // TODO: Put shared_ptrs in a vector (multiple owners)
  // std::vector<std::shared_ptr<DataLogger>> subscribers;
  // subscribers.push_back(logger);
  // std::cout << "  ref count after vector: " << logger.use_count() << "\n";

  // Object is destroyed only when ALL shared_ptrs are gone
}

// ---- TASK 5: Preventing circular references with weak_ptr ----
// This is an advanced topic. Study and understand the problem.

struct Node {
  std::string name;
  std::shared_ptr<Node> next; // strong reference
  // std::shared_ptr<Node> prev;    // THIS WOULD CAUSE A LEAK (circular!)
  std::weak_ptr<Node> prev; // weak reference — doesn't prevent deletion

  Node(const std::string &n) : name(n) {
    std::cout << "  [Node '" << name << "'] Created\n";
  }
  ~Node() { std::cout << "  [Node '" << name << "'] Destroyed\n"; }
};

void task5_weak_ptr() {
  std::cout << "\n-- Task 5: weak_ptr --\n";

  auto node1 = std::make_shared<Node>("A");
  auto node2 = std::make_shared<Node>("B");

  node1->next = node2;
  node2->prev = node1; // weak_ptr — doesn't increase ref count

  std::cout << "  node1 ref count: " << node1.use_count() << "\n"; // 1 (not 2!)
  std::cout << "  node2 ref count: " << node2.use_count()
            << "\n"; // 2 (node1->next)

  // To use a weak_ptr, you must "lock" it to get a temporary shared_ptr
  if (auto prev = node2->prev.lock()) {
    std::cout << "  node2's prev is: " << prev->name << "\n";
  } else {
    std::cout << "  node2's prev has been deleted!\n";
  }
}

// ============ TEST CODE ============

int main() {
  std::cout << "=== Exercise 5: Smart Pointers ===\n";

  task1_unique_basics();
  task2_ownership_transfer();
  task3_custom_deleter();
  task4_shared_ptr();
  task5_weak_ptr();

  return 0;
}
