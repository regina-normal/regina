
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

#import <UIKit/UIKit.h>
#import "Example.h"

// TODO: Viewing and editing a document
// TODO: Creating a new document
// TODO: Selecting a document from a list of documents owned by the application
// TODO: Opening, closing, renaming, and deleting a selected document
// TODO: Putting a selected document in iCloud storage (and removing a selected document from iCloud storage)
// TODO: Indicating error conditions, including document-version conflicts

// TODO: iCloud: all documents, or none. - See https://developer.apple.com/Library/ios/documentation/DataManagement/Conceptual/DocumentBasedAppPGiOS/ManageDocumentLifeCycle/ManageDocumentLifeCycle.html

namespace regina {
    class NPacket;
}

/**
 * Regina's main document class.
 */
@interface ReginaDocument : UIDocument

/**
 * Is this one of the (read-only) example files shipped with Regina?
 */
@property (assign, nonatomic, readonly) BOOL example;

/**
 * Stores the full packet tree whilst the file is open.
 *
 * This is \c nil until the file has been successfully read, and will be
 * \c nil again after the document has been successfully closed.
 *
 * Be warned that opening and closing operations are asynchronous.  So,
 * for instance, the \a tree property may still be \c nil for a little
 * time after openWithCompletionHandler returns, and may still be non-null
 * for a little time after closeWithCompletionHandler returns.
 */
@property (assign, nonatomic, readonly) regina::NPacket* tree;

- (id)initWithExample:(Example*)e;
+ (id)documentWithExample:(Example*)e;

@end
