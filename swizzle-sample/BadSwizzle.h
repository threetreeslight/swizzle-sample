//
//  BadSwizzled.m
//  swizzle-sample
//
//  Created by ae06710 on 2/10/15.
//  Copyright (c) 2015 ae06710. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <objc/runtime.h>

# pragma - mark
@interface BadBare : NSObject
@end

@implementation BadBare
- (void)foo
{
    NSLog(@"Bare foo method");
}
@end


# pragma mark - general
@interface GeneralSwz : BadBare
@end

@implementation GeneralSwz
+ (void)load
{
    Method originalMethod = class_getInstanceMethod(self, @selector(foo));
    Method swizzledMethod = class_getInstanceMethod(self, @selector(swz_foo));
    method_exchangeImplementations(originalMethod, swizzledMethod);
}

- (void)foo
{
    NSLog(@"General foo method");
}

- (void)swz_foo
{
    NSLog(@"General swizzed foo method");
    [self swz_foo];
}

@end

# pragma mark - Inherited

@interface InheritedSwz : BadBare
@end

@implementation InheritedSwz
+ (void)load
{
    Method originalMethod = class_getInstanceMethod(self, @selector(foo));
    Method swizzledMethod = class_getInstanceMethod(self, @selector(swz_foo));
    method_exchangeImplementations(originalMethod, swizzledMethod);
}

// we got `-[Bare swz_foo]: unrecognized selector sent to instance`
//- (void)swz_foo
//{
//    NSLog(@"Inherited swz_foo method");
//    [self swz_foo];
//}
@end

# pragma mark - Inherited

@interface InheritedWithOverrideSwz : BadBare
@end

@implementation InheritedWithOverrideSwz
- (void)foo
{
    NSLog(@"Inherited with Override foo method");
}
@end


# pragma mark - Category

@interface CategorySwz : NSObject
@end

@implementation CategorySwz

+ (void)load
{
    Method originalMethod = class_getInstanceMethod(self, @selector(foo));
    Method swizzledMethod = class_getInstanceMethod(self, @selector(swz_foo));
    method_exchangeImplementations(originalMethod, swizzledMethod);
}

- (void)foo
{
    NSLog(@"original foo method");
}

- (void)swz_foo
{
    NSLog(@"swizzled foo method");
    [self swz_foo];
}

@end

@implementation CategorySwz (category)

- (void)swz_foo
{
    // Call this only
    NSLog(@"Cateogry's foo method");
}

@end

# pragma mark - assert

@interface AssertSwz : NSObject
@end

@implementation AssertSwz

+ (void)load
{
    Method originalMethod = class_getInstanceMethod(self, @selector(foo));
    Method swizzledMethod = class_getInstanceMethod(self, @selector(swz_foo));
    method_exchangeImplementations(originalMethod, swizzledMethod);
}

- (void)foo
{
    assert([NSStringFromSelector(_cmd) isEqualToString:@"foo"]);
    NSLog(@"original foo method");
}

- (void)swz_foo
{
    NSLog(@"swizzed foo method");
    [self swz_foo];
}

@end

# pragma mark - KVO

@interface MyKlass : NSObject
@property (nonatomic, strong, readwrite) NSString *myString;
@end

@implementation MyKlass
+ (void)load
{
    Method originalMethod = class_getInstanceMethod(self, @selector(setMyString:));
    Method swizzledMethod = class_getInstanceMethod(self, @selector(swz_setMyString:));
    method_exchangeImplementations(originalMethod, swizzledMethod);
}

- (void)setMyString:(NSString *)myString
{
    NSLog(@"override setMyString method");
}

- (void)swz_setMyString:(NSString *)myString
{
    NSLog(@"swzzled setMyString method");
    [self swz_setMyString:myString];
}
@end

@interface KvoSwz : NSObject
@property (nonatomic, strong, readwrite) MyKlass *myKlass;
@end

@implementation KvoSwz

- (instancetype)init
{
    self = [super init];
	if (self != nil) {
        self.myKlass = MyKlass.alloc.init;
        [_myKlass addObserver:self forKeyPath:@"myString" options:NSKeyValueObservingOptionNew context:nil];
    }
    return self;
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
    if ([keyPath isEqual:@"myString"]) {
        NSLog(@"myString are changed");
    }
}
@end

# pragma mark - KVC