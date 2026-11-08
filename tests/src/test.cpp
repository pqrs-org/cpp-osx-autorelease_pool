#include "TestHelper.h"
#include <atomic>
#include <iostream>
#include <pqrs/osx/autorelease_pool.hpp>
#include <stdexcept>
#include <thread>
#include <type_traits>

namespace {
bool expect(bool condition,
            const char* expression) {
  if (!condition) {
    std::cerr << "Expectation failed: " << expression << std::endl;
  }

  return condition;
}
} // namespace

int main() {
  static_assert(!std::is_copy_constructible_v<pqrs::osx::autorelease_pool::scoped_autorelease_pool>);
  static_assert(!std::is_copy_assignable_v<pqrs::osx::autorelease_pool::scoped_autorelease_pool>);
  static_assert(!std::is_move_constructible_v<pqrs::osx::autorelease_pool::scoped_autorelease_pool>);
  static_assert(!std::is_move_assignable_v<pqrs::osx::autorelease_pool::scoped_autorelease_pool>);

  auto passed = true;

  passed = expect(pqrs_osx_autorelease_pool_test_object_count() == 0,
                  "there are initially no test objects") &&
           passed;

  {
    pqrs::osx::autorelease_pool::scoped_autorelease_pool pool;

    pqrs_osx_autorelease_pool_test_create_autoreleased_object();

    passed = expect(pqrs_osx_autorelease_pool_test_object_count() == 1,
                    "autoreleased object remains alive inside the pool") &&
             passed;
  }

  passed = expect(pqrs_osx_autorelease_pool_test_object_count() == 0,
                  "autoreleased object is released when the pool is destroyed") &&
           passed;

  {
    pqrs::osx::autorelease_pool::scoped_autorelease_pool outer_pool;

    pqrs_osx_autorelease_pool_test_create_autoreleased_object();

    passed = expect(pqrs_osx_autorelease_pool_test_object_count() == 1,
                    "outer pool keeps its autoreleased object alive") &&
             passed;

    {
      pqrs::osx::autorelease_pool::scoped_autorelease_pool inner_pool;

      pqrs_osx_autorelease_pool_test_create_autoreleased_object();

      passed = expect(pqrs_osx_autorelease_pool_test_object_count() == 2,
                      "outer and inner pool objects remain alive inside the inner pool") &&
               passed;
    }

    passed = expect(pqrs_osx_autorelease_pool_test_object_count() == 1,
                    "inner pool releases only its autoreleased object") &&
             passed;
  }

  passed = expect(pqrs_osx_autorelease_pool_test_object_count() == 0,
                  "outer pool releases its autoreleased object") &&
           passed;

  std::atomic<bool> object_was_alive_in_worker_pool{false};

  std::thread worker([&object_was_alive_in_worker_pool] {
    pqrs::osx::autorelease_pool::scoped_autorelease_pool pool;

    pqrs_osx_autorelease_pool_test_create_autoreleased_object();
    object_was_alive_in_worker_pool = pqrs_osx_autorelease_pool_test_object_count() == 1;
  });

  worker.join();

  passed = expect(object_was_alive_in_worker_pool.load(),
                  "autoreleased object remains alive inside a worker thread pool") &&
           passed;
  passed = expect(pqrs_osx_autorelease_pool_test_object_count() == 0,
                  "worker thread pool releases its autoreleased object") &&
           passed;

  auto object_was_alive_in_execute = false;

  pqrs::osx::autorelease_pool::execute_with_autorelease_pool([&object_was_alive_in_execute] {
    pqrs_osx_autorelease_pool_test_create_autoreleased_object();
    object_was_alive_in_execute = pqrs_osx_autorelease_pool_test_object_count() == 1;
  });

  passed = expect(object_was_alive_in_execute,
                  "autoreleased object remains alive inside execute_with_autorelease_pool") &&
           passed;
  passed = expect(pqrs_osx_autorelease_pool_test_object_count() == 0,
                  "execute_with_autorelease_pool releases its autoreleased object") &&
           passed;

  auto exception_was_caught = false;

  try {
    pqrs::osx::autorelease_pool::execute_with_autorelease_pool([] {
      pqrs_osx_autorelease_pool_test_create_autoreleased_object();
      throw std::runtime_error("test");
    });
  } catch (const std::runtime_error&) {
    exception_was_caught = true;
  }

  passed = expect(exception_was_caught,
                  "execute_with_autorelease_pool propagates exceptions") &&
           passed;
  passed = expect(pqrs_osx_autorelease_pool_test_object_count() == 0,
                  "execute_with_autorelease_pool releases its autoreleased object when an exception is thrown") &&
           passed;

  return passed ? 0 : 1;
}
