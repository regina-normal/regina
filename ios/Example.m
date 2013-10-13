
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

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
