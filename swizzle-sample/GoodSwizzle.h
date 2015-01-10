//
//  GoodSwizzle.h
//  swizzle-sample
//
//  Created by ae06710 on 2/11/15.
//  Copyright (c) 2015 ae06710. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <objc/runtime.h>
#import <libkern/OSAtomic.h>

# pragma mark - bare klass

@interface GoodBare : NSObject
@end

@implementation GoodBare
// It effected by GoodSwz class
- (void)foo
{
    NSLog(@"Good Bare foo method");
}
@end

# pragma mark - swz

@interface GoodSwz : GoodBare
+ (void)swizzle;
@end

static IMP __original_Method_Imp;

void _replacement_Method(id self, SEL _cmd)
{
    // it will pass
    assert([NSStringFromSelector(_cmd) isEqualToString:@"foo"]);

    NSLog(@"swizzed good foo method");

    ((void(*)(id,SEL))__original_Method_Imp)(self, _cmd);
}

@implementation GoodSwz
+ (void)swizzle
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        OSSpinLock lock = OS_SPINLOCK_INIT;

        OSSpinLockLock(&lock);
        Method m = class_getInstanceMethod( [self class], @selector(foo));
        __original_Method_Imp = method_setImplementation( m, (IMP)_replacement_Method );
        OSSpinLockUnlock(&lock);
    });
}
@end

# pragma mark - inherited

@interface InheritedGoodSwz : GoodSwz
@end

@implementation InheritedGoodSwz
// thare are not any effect!

- (void)foo
{
    NSLog(@"inherited good foo method");
}
@end

//# pragma mark - Category Extended
//
//@implementation GoodSwz (category)
//- (void)foo
//{
//    NSLog(@"Category's good foo method");
//}
//@end




