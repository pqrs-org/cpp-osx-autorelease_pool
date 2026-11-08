#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void pqrs_osx_autorelease_pool_test_create_autoreleased_object(void);
bool pqrs_osx_autorelease_pool_test_object_is_alive(void);

#ifdef __cplusplus
}
#endif
