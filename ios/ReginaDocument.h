
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

#import <UIKit/UIKit.h>
#import "Example.h"

namespace regina {
    class Packet;
}

/**
 * Regina's main document class.
 */
@interface ReginaDocument : UIDocument

enum DocType {
    /**
     * Indicates a native Regina data file, residing in the usual documents directory.
     */
    DOC_NATIVE,
    /**
     * Indicates a native Regina data file, residing in a read-only location.  Such a
     * file might be an example file, or census data.
     * When modified, a file of this type must be copied into the documents directory
     * before it can be saved.
     */
    DOC_READONLY,
    /**
     * Indicates a file in a foreign data format.
     * When modified, a file of this type must be saved in the documents directory under
     * a different name, using Regina's native file format.
     */
    DOC_FOREIGN
};

@property (assign, nonatomic, readonly) DocType type;

/**
 * Stores the full packet tree whilst the file is open.
 *
 * This is \c nil until either the file has been successfully read or a new
 * file has been created.  It will be \c nil again after the document has been
 * successfully closed.
 *
 * Be warned that creation, opening and closing operations are asynchronous.
 * So, for instance, the \a tree property may still be \c nil for a little
 * time after openWithCompletionHandler returns, and may still be non-null
 * for a little time after closeWithCompletionHandler returns.
 */
- (regina::Packet*)tree;

- (id)initWithExample:(Example*)e;
+ (id)documentWithExample:(Example*)e;

- (id)initWithInboxURL:(NSURL*)u preferredName:(NSURL*)name;
+ (id)documentWithInboxURL:(NSURL*)u preferredName:(NSURL*)name;

- (id)initWithURL:(NSURL*)u;
+ (id)documentWithURL:(NSURL*)u;

// Note: these leave the document in an opened state.
- (id)initNewWithCompletionHandler:(void (^)(ReginaDocument* doc))completionHandler;
+ (id)documentNewWithCompletionHandler:(void (^)(ReginaDocument* doc))completionHandler;

/**
 * Notifies the document that the underlying packet tree has been changed in some way.
 * This could be through changes to the structure of the tree, or to the packets that
 * it contains.
 *
 * If this is a read-only example file, it will be copied into the documents directory.
 *
 * Either way, the file will be marked as dirty so that it will be saved when appropriate.
 */
- (void)setDirty;

+ (NSURL*)docsDir;
+ (NSURL*)uniqueDocURLFor:(NSURL*)url;

@end
