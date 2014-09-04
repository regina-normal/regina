
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

#import "ReginaDocument.h"

// TODO: Delete packet when closing the document.

// TODO: Perhaps override the hooks disableEditing / enableEditing, to make the UI read-only?

/**
 * The internal global array of all available examples.
 * This will be created the first time it is required.
 */
static NSArray* _examples = nil;

@interface ReginaDocument () {
    NSString* description;
}
@end

@implementation ReginaDocument

- (id)initWithExampleFile:(NSString *)f desc:(NSString *)d
{
    NSString* path = [[NSBundle mainBundle] pathForResource:f ofType:nil inDirectory:@"examples"];
    if (! path) {
        NSLog(@"Could not find example resource: %@", f);
        return nil;
    }
    
    self = [super initWithFileURL:[NSURL fileURLWithPath:path]];
    if (self) {
        description = d;
        _example = YES;
    }
    return self;
}

- (NSString *)localizedName {
    if (description)
        return description;
    else
        return [super localizedName];
}

+ (id)documentWithExampleFile:(NSString *)f desc:(NSString *)d
{
    return [[ReginaDocument alloc] initWithExampleFile:f desc:d];
}

- (BOOL)loadFromContents:(id)contents ofType:(NSString *)typeName error:(NSError *__autoreleasing *)outError
{
    /*
    _tree = TODO;
    
    // TODO.
    if ([_delegate respondsToSelector:@selector(noteDocumentContentsUpdated:)]) {
        [_delegate noteDocumentContentsUpdated:self];
    }
    */
    return YES;
}

- (id)contentsForType:(NSString *)typeName error:(NSError *__autoreleasing *)outError
{
    // TODO.
    /*
    NSData* data = TODO;
    return data;
     */
    return nil;
}

+ (NSArray*)examples {
    if (! _examples)
        _examples = [NSArray arrayWithObjects:
                [ReginaDocument documentWithExampleFile:@"sample-ios.rga" desc:@"Introductory examples"],
                [ReginaDocument documentWithExampleFile:@"closed-or-census.rga" desc:@"Closed census (orientable)"],
                [ReginaDocument documentWithExampleFile:@"closed-nor-census.rga" desc:@"Closed census (non-orientable)"],
                [ReginaDocument documentWithExampleFile:@"cusped-hyp-or-census.rga" desc:@"Cusped hyperbolic census (orientable)"],
                [ReginaDocument documentWithExampleFile:@"cusped-hyp-nor-census.rga" desc:@"Cusped hyperbolic census (non-orientable)"],
                [ReginaDocument documentWithExampleFile:@"closed-hyp-census.rga" desc:@"Hodgson-Weeks closed hyperbolic census"],
                [ReginaDocument documentWithExampleFile:@"hyp-knot-link-census.rga" desc:@"Hyperbolic knot / link complements"],
                nil];
    return _examples;
}

@end
