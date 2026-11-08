#import <Foundation/Foundation.h>

#include "TestHelper.h"

#if __has_feature(objc_arc)
#error "TestHelper.m must be compiled with -fno-objc-arc"
#endif

static size_t object_count = 0;

@interface TestObject : NSObject
@end

@implementation TestObject

- (instancetype)init {
  self = [super init];

  if (self) {
    ++object_count;
  }

  return self;
}

- (void)dealloc {
  --object_count;

  [super dealloc];
}

@end

void pqrs_osx_autorelease_pool_test_create_autoreleased_object(void) {
  [[[TestObject alloc] init] autorelease];
}

size_t pqrs_osx_autorelease_pool_test_object_count(void) {
  return object_count;
}
