#include "TestHelper.h"
#include <atomic>
#include <iostream>
#include <pqrs/osx/autorelease_pool.hpp>
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

  passed = expect(!pqrs_osx_autorelease_pool_test_object_is_alive(),
                  "test object is initially released") &&
           passed;

  {
    pqrs::osx::autorelease_pool::scoped_autorelease_pool pool;

    pqrs_osx_autorelease_pool_test_create_autoreleased_object();

    passed = expect(pqrs_osx_autorelease_pool_test_object_is_alive(),
                    "autoreleased object remains alive inside the pool") &&
             passed;
  }

  passed = expect(!pqrs_osx_autorelease_pool_test_object_is_alive(),
                  "autoreleased object is released when the pool is destroyed") &&
           passed;

  {
    pqrs::osx::autorelease_pool::scoped_autorelease_pool outer_pool;

    {
      pqrs::osx::autorelease_pool::scoped_autorelease_pool inner_pool;

      pqrs_osx_autorelease_pool_test_create_autoreleased_object();

      passed = expect(pqrs_osx_autorelease_pool_test_object_is_alive(),
                      "autoreleased object remains alive inside the inner pool") &&
               passed;
    }

    passed = expect(!pqrs_osx_autorelease_pool_test_object_is_alive(),
                    "inner pool releases its autoreleased object") &&
             passed;
  }

  std::atomic<bool> object_was_alive_in_worker_pool{false};

  std::thread worker([&object_was_alive_in_worker_pool] {
    pqrs::osx::autorelease_pool::scoped_autorelease_pool pool;

    pqrs_osx_autorelease_pool_test_create_autoreleased_object();
    object_was_alive_in_worker_pool = pqrs_osx_autorelease_pool_test_object_is_alive();
  });

  worker.join();

  passed = expect(object_was_alive_in_worker_pool.load(),
                  "autoreleased object remains alive inside a worker thread pool") &&
           passed;
  passed = expect(!pqrs_osx_autorelease_pool_test_object_is_alive(),
                  "worker thread pool releases its autoreleased object") &&
           passed;

  return passed ? 0 : 1;
}
