//
//  RExamples.m
//  Regina
//
//  Created by Ben Burton on 12/10/13.
//  Copyright (c) 2013 Regina Development Team. All rights reserved.
//

#import "Example.h"

// The internal global array of all available examples.
// This will be created the first time it is required.
static NSArray* _all = nil;

@implementation Example

- (id)initWithFile:(NSString*)f desc:(NSString*)d {
    self = [super init];
    if (self) {
        _file = f;
        _desc = d;
    }
    return self;
}

+ (id)exampleWithFile:(NSString *)f desc:(NSString *)d {
    return [[Example alloc] initWithFile:f desc:d];
}

+ (NSArray*)all {
    if (! _all)
        _all = [NSArray arrayWithObjects:
            [Example exampleWithFile:@"sample-misc.rga" desc:@"Introductory examples"],
            [Example exampleWithFile:@"closed-or-census.rga" desc:@"Closed orientable census"],
            [Example exampleWithFile:@"snappea-census.rga" desc:@"Cusped hyperbolic census"],
            [Example exampleWithFile:@"knot-link-census.rga" desc:@"Hyperbolic knot/link complements"],
            nil];
    return _all;
}

@end
