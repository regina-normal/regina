
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

#import <Foundation/Foundation.h>

/**
 * Represents a temporary file.  The filename is guaranteed to be unique
 * for the life of this process (but no longer).
 *
 * When this TempFile object is initialised, the filename and URL will be
 * initialised but the file itself will not be created.
 *
 * When this TempFile object is destroyed, the temporary file (if it exists
 * on the filesystem) will be deleted.
 */
@interface TempFile : NSObject

@property (strong, nonatomic, readonly) NSString *filename;
@property (strong, nonatomic, readonly) NSURL *url;

/**
 * Initialises this to a new temporary filename, which is unique for the life of this process.
 *
 * @param ext The extension of the new filename, which should not begin with a period.
 * An example might be @"pdf".
 */
- (id)initWithExtension:(NSString*)ext;
/**
 * Creates a new temporary filename, which is unique for the life of this process.
 *
 * @param ext The extension of the new filename, which should not begin with a period.
 * An example might be @"pdf".
 */
+ (id)tempFileWithExtension:(NSString*)ext;

/**
 * Deletes this file from the filesystem, if it exists.
 *
 * There is typically no need to call removeFile, unless you urgently need the space.
 * This is because removeFile will be called automatically when this TempFile is
 * deallocated.  As an exception, it will \e not be called upon deallocation if it
 * has been called before and it returned \c true.
 *
 * You may call this removeFile multiple times (though of course this only makes sense if you
 * have created the file multiple times).
 *
 * @return \c true if and only if the file was removed from the filesystem, or
 * \c false if an error occured (e.g., if the file was never created in the first place).
 */
- (BOOL)removeFile;

- (void)dealloc;

@end
