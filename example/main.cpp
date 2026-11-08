#include <iostream>
#include <pqrs/osx/autorelease_pool.hpp>

int main() {
  pqrs::osx::autorelease_pool::execute_with_autorelease_pool([] {
    std::cout << "scoped_autorelease_pool is active" << std::endl;
  });

  return 0;
}
