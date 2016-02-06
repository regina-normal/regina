
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

static Example* _intro = [Example exampleWithFile:@"sample.rga" desc:@"Introductory examples"];

static NSArray* _census = [NSArray arrayWithObjects:
                           [Example exampleWithFile:@"closed-or-census.rga" desc:@"Closed census (orientable)"],
                           [Example exampleWithFile:@"closed-nor-census.rga" desc:@"Closed census (non-orientable)"],
                           [Example exampleWithFile:@"cusped-hyp-or-census.rga" desc:@"Cusped hyperbolic census (orientable)"],
                           [Example exampleWithFile:@"cusped-hyp-nor-census.rga" desc:@"Cusped hyperbolic census (non-orientable)"],
                           [Example exampleWithFile:@"closed-hyp-census.rga" desc:@"Hodgson-Weeks closed hyperbolic census"],
                           [Example exampleWithFile:@"hyp-knot-link-census.rga" desc:@"Hyperbolic knot / link complements"],
                           nil];

@implementation Example

- (id)initWithFile:(NSString*)f desc:(NSString*)d
{
    self = [super init];
    if (self) {
        _file = f;
        _desc = d;
    }
    return self;
}

+ (id)exampleWithFile:(NSString *)f desc:(NSString *)d
{
    return [[Example alloc] initWithFile:f desc:d];
}

+ (Example*)intro
{
    return _intro;
}

+ (NSArray*)census
{
    return _census;
}

@end
