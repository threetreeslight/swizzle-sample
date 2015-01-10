//
//  ViewController.m
//  sample-single
//
//  Created by ae06710 on 1/10/15.
//  Copyright (c) 2015 ae06710. All rights reserved.
//

#import "ViewController.h"
#import "BadSwizzle.h"
#import "GoodSwizzle.h"

@interface ViewController ()
@property (nonatomic, strong, readwrite) KvoSwz *kvoSwz;
@end

@implementation ViewController

# pragma mark - bad parts

- (IBAction)callBareFoo:(UIButton *)sender {
    BadBare.alloc.init.foo;
}

- (IBAction)callGeneralFoo:(UIButton *)sender {
    GeneralSwz.alloc.init.foo;
}

- (IBAction)callInheritedFoo:(UIButton *)sender {
    InheritedSwz.alloc.init.foo;
}

- (IBAction)callInheritedWithOverrideFoo:(UIButton *)sender {
    InheritedWithOverrideSwz.alloc.init.foo;
}

- (IBAction)callCategoryFoo:(UIButton *)sender {
    CategorySwz.alloc.init.foo;
}

- (IBAction)callAssertFoo:(UIButton *)sender {
    AssertSwz.alloc.init.foo;
}
- (IBAction)callKVOFoo:(UIButton *)sender {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        self.kvoSwz = KvoSwz.alloc.init;
    });
    _kvoSwz.myKlass.myString = @"example";
    [_kvoSwz.myKlass setMyString:@"example"];
}

# pragma mark - good parts

- (IBAction)callGoodBareFoo:(UIButton *)sender {
    GoodBare.alloc.init.foo;
}

- (IBAction)callGoodFoo:(UIButton *)sender {
    GoodSwz.alloc.init.foo;
}

- (IBAction)callGoodSwizzle:(UIButton *)sender {
    [GoodSwz swizzle];
}

- (IBAction)callInheritedGoodFoo:(UIButton *)sender {
    InheritedGoodSwz.alloc.init.foo;
}

@end
