#import <Foundation/Foundation.h>

#include "TestHelper.h"

#if __has_feature(objc_arc)
#error "TestHelper.m must be compiled with -fno-objc-arc"
#endif

static bool object_is_alive = false;

@interface TestObject : NSObject
@end

@implementation TestObject

- (instancetype)init {
  self = [super init];

  if (self) {
    object_is_alive = true;
  }

  return self;
}

- (void)dealloc {
  object_is_alive = false;

  [super dealloc];
}

@end

void pqrs_osx_autorelease_pool_test_create_autoreleased_object(void) {
  [[[TestObject alloc] init] autorelease];
}

bool pqrs_osx_autorelease_pool_test_object_is_alive(void) {
  return object_is_alive;
}
