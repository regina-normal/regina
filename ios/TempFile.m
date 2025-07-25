
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#import "TempFile.h"

@interface TempFile () {
    BOOL removed;
}
@end

@implementation TempFile

- (id)initWithExtension:(NSString *)ext {
    self = [super init];
    if (self) {
        NSString* guid = [[NSProcessInfo processInfo] globallyUniqueString];
        NSString* basename = [guid stringByAppendingPathExtension:ext];
        _filename = [NSTemporaryDirectory() stringByAppendingPathComponent:basename];
        _url = [NSURL fileURLWithPath:_filename isDirectory:NO];
        removed = NO;
    }
    return self;
}

+ (id)tempFileWithExtension:(NSString *)ext {
    return [[TempFile alloc] initWithExtension:ext];
}

- (BOOL)removeFile {
    BOOL result = [[NSFileManager defaultManager] removeItemAtPath:_filename error:nil];
    if (! result)
        NSLog(@"Could not remove temporary file: %@", _filename);
    else
        removed = YES;
    return result;
}

- (void)dealloc {
    // Note: ARC forbids us from calling [super dealloc].
    if (! removed)
        [self removeFile];
}

@end
