#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void pqrs_osx_autorelease_pool_test_create_autoreleased_object(void);
size_t pqrs_osx_autorelease_pool_test_object_count(void);

#ifdef __cplusplus
}
#endif
