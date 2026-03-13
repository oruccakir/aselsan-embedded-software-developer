/**
 * EXERCISE 4: RAII (Resource Acquisition Is Initialization)
 *
 * RAII is THE most important C++ idiom. No equivalent in Java.
 *
 * The idea: tie resource lifetime to object lifetime.
 * - Acquire resource in constructor
 * - Release resource in destructor
 * - The compiler guarantees the destructor runs when scope ends
 *
 * Java uses try-with-resources / finally. C++ uses RAII — it's automatic
 * and applies to ALL resources, not just "closeable" ones.
 *
 * In embedded systems, RAII manages:
 * - Mutex locks (lock in ctor, unlock in dtor)
 * - Hardware peripherals (init in ctor, deinit in dtor)
 * - Interrupt enable/disable
 * - DMA channels, GPIO pins, file handles
 *
 * TASKS:
 * 1. Implement a RAII mutex lock guard
 * 2. Implement a RAII GPIO pin manager
 * 3. Implement a RAII interrupt disabler
 * 4. Fix exception-unsafe code using RAII
 */

#include <iostream>
#include <stdexcept>

// ---- Simulated Hardware (pretend these are real hardware calls) ----

struct FakeMutex {
  bool locked = false;
  void lock() {
    locked = true;
    std::cout << "  [HW] Mutex locked\n";
  }
  void unlock() {
    locked = false;
    std::cout << "  [HW] Mutex unlocked\n";
  }
};

namespace GPIO {
inline void init(int pin) {
  std::cout << "  [HW] GPIO pin " << pin << " initialized\n";
}
inline void deinit(int pin) {
  std::cout << "  [HW] GPIO pin " << pin << " deinitialized\n";
}
inline void set_high(int pin) {
  std::cout << "  [HW] GPIO pin " << pin << " HIGH\n";
}
inline void set_low(int pin) {
  std::cout << "  [HW] GPIO pin " << pin << " LOW\n";
}
} // namespace GPIO

namespace Interrupt {
inline void disable() { std::cout << "  [HW] Interrupts DISABLED\n"; }
inline void enable() { std::cout << "  [HW] Interrupts ENABLED\n"; }
} // namespace Interrupt

// ---- TASK 1: RAII Lock Guard ----
// When created: locks the mutex
// When destroyed: unlocks the mutex
// This guarantees the mutex is ALWAYS unlocked, even if an exception is thrown.
//
// Java equivalent: synchronized block — but RAII is more general.
// std::lock_guard in the standard library does exactly this.

class LockGuard {
public:
  // TODO: Constructor — take a FakeMutex reference and lock it
  // TODO: Destructor — unlock the mutex

  // Prevent copying (a lock should not be duplicated!)
  LockGuard(const LockGuard &) = delete;
  LockGuard &operator=(const LockGuard &) = delete;

private:
  // TODO: Store a reference to the mutex
};

// ---- TASK 2: RAII GPIO Pin ----
// Manages a GPIO pin's lifecycle.
// Constructor: initializes the pin
// Destructor: deinitializes the pin
// Also provides set_high() and set_low() methods.

class GpioPin {
public:
  // TODO: Constructor — take pin number, call GPIO::init()
  // TODO: Destructor — call GPIO::deinit()

  // TODO: set_high() and set_low() methods

  GpioPin(const GpioPin &) = delete;
  GpioPin &operator=(const GpioPin &) = delete;

private:
  // TODO: Store pin number
};

// ---- TASK 3: RAII Interrupt Guard ----
// Disables interrupts on construction, re-enables on destruction.
// Critical for embedded: accessing shared hardware registers.

class InterruptGuard {
public:
  // TODO: Constructor — disable interrupts
  // TODO: Destructor — re-enable interrupts

  InterruptGuard(const InterruptGuard &) = delete;
  InterruptGuard &operator=(const InterruptGuard &) = delete;
};

// ---- TASK 4: Fix this code using RAII ----
// This function has a resource leak when an exception is thrown.
// Rewrite it using your RAII classes above.

void process_sensor_data_BROKEN(FakeMutex &mtx) {
  // BROKEN VERSION — DO NOT MODIFY, just study it
  mtx.lock();
  GPIO::init(7);

  // Simulate some processing that might throw
  int sensor_value = 42; // pretend we read from hardware
  if (sensor_value > 40) {
    // BUG: mutex stays locked, GPIO stays initialized!
    throw std::runtime_error("Sensor value too high!");
  }

  GPIO::deinit(7);
  mtx.unlock();
}

// TODO: Rewrite the function above using RAII
void process_sensor_data_FIXED(FakeMutex &mtx) {
  // TODO: Use LockGuard and GpioPin to manage resources
  // Even if an exception is thrown, everything gets cleaned up!

  int sensor_value = 42;
  if (sensor_value > 40) {
    throw std::runtime_error("Sensor value too high!");
    // Resources are automatically released by RAII destructors
  }
}

// ============ TEST CODE ============

void test_lock_guard() {
  std::cout << "-- LockGuard Test --\n";
  FakeMutex mtx;
  {
    // LockGuard guard(mtx);  // Uncomment after implementing
    std::cout << "  Inside guarded scope\n";
  } // guard destroyed here — mutex should be unlocked
  std::cout << "  After scope, mutex locked=" << mtx.locked << "\n\n";
}

void test_gpio() {
  std::cout << "-- GpioPin Test --\n";
  {
    // GpioPin led(13);  // Uncomment after implementing
    // led.set_high();
    // led.set_low();
  } // pin deinitialized here
  std::cout << "  After scope, pin should be deinitialized\n\n";
}

void test_raii_exception_safety() {
  std::cout << "-- RAII Exception Safety Test --\n";
  FakeMutex mtx;

  std::cout << "  Testing BROKEN version:\n";
  try {
    process_sensor_data_BROKEN(mtx);
  } catch (const std::exception &e) {
    std::cout << "  Caught: " << e.what() << "\n";
    std::cout << "  Mutex still locked=" << mtx.locked << " (BAD!)\n\n";
  }
  mtx.unlock(); // manual cleanup needed

  std::cout << "  Testing FIXED version:\n";
  try {
    process_sensor_data_FIXED(mtx);
  } catch (const std::exception &e) {
    std::cout << "  Caught: " << e.what() << "\n";
    std::cout << "  Mutex locked=" << mtx.locked << " (should be 0)\n";
  }
}

int main() {
  std::cout << "=== Exercise 4: RAII ===\n\n";
  test_lock_guard();
  test_gpio();
  test_raii_exception_safety();
  return 0;
}
