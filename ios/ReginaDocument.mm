
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
#import "packet/npacket.h"
#import <boost/iostreams/device/array.hpp>
#import <boost/iostreams/stream.hpp>
#import <sstream>

// TODO: Delete packet when closing the document.

// TODO: Perhaps override the hooks disableEditing / enableEditing, to make the UI read-only?

@interface ReginaDocument () {
    NSString* description;
}
@end

@implementation ReginaDocument

- (id)initWithExample:(Example *)e
{
    NSString* path = [[NSBundle mainBundle] pathForResource:e.file ofType:nil inDirectory:@"examples"];
    if (! path) {
        NSLog(@"Could not find example resource: %@", e.file);
        return nil;
    }
    
    self = [super initWithFileURL:[NSURL fileURLWithPath:path]];
    if (self) {
        description = e.desc;
        _example = YES;
        _tree = 0;
    }
    return self;
}

+ (id)documentWithExample:(Example *)e
{
    return [[ReginaDocument alloc] initWithExample:e];
}

- (NSString *)localizedName {
    if (description)
        return description;
    else
        return [super localizedName];
}

- (BOOL)loadFromContents:(id)contents ofType:(NSString *)typeName error:(NSError *__autoreleasing *)outError
{
    boost::iostreams::stream<boost::iostreams::array_source> s(static_cast<const char*>([contents bytes]), [contents length]);
    _tree = regina::open(s);
    
    if (_tree)
        return YES;
    else {
        // TODO: outError = [NSError errorWithDomain:<#(NSString *)#> code:<#(NSInteger)#> userInfo:<#(NSDictionary *)#>]
        return NO;
    }
}

- (id)contentsForType:(NSString *)typeName error:(NSError *__autoreleasing *)outError
{
    if (! _tree) {
        // TODO
        return nil;
    }
    
    std::ostringstream s;
    if (_tree->save(s)) {
        const std::string& str = s.str();
        return [NSData dataWithBytes:str.c_str() length:str.length()];
    } else {
        // TODO
        return nil;
    }
}

@end
