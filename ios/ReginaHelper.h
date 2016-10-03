
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

@class AppDelegate;
@class DetailViewController;
@class MasterViewController;
@class PacketTreeController;
@class ReginaDocument;

namespace regina {
    class Packet;
}

/**
 * Gives access to some of the core objects in the user interface.
 */
@interface ReginaHelper : NSObject

/**
 * Returns the root view controller for the entire app.
 * This is useful for displaying popups (which might otherwise
 * be hidden beneath the master view controller in portrait mode).
 */
+ (UIViewController*)root;

/**
 * Returns the root-level master view controller.  This contains the
 * list of documents within the local documents directory.
 */
+ (MasterViewController*)master;

/**
 * Returns the detail view controller.  This is the view into which
 * packet viewers will be placed.
 */
+ (DetailViewController*)detail;

/**
 * Returns the current working document, or \c nil if no document is open.
 */
+ (ReginaDocument*)document;

/**
 * Returns the topmost packet tree controller in the navigation stack,
 * or \c nil if no document is open.
 */
+ (PacketTreeController*)tree;

/**
 * Returns the packet tree controller corresponding to the root of the
 * packet tree, or \c nil if no document is open.
 */
+ (PacketTreeController*)treeRoot;

/**
 * Opens the given packet for viewing and/or editing.
 *
 * This routine will also attempt to navigate to and select the packet in the
 * master view if this has not already been done.  This will always be safe,
 * even if the packet is already selected, or even if the packet does not appear
 * in the master view at all.
 */
+ (void)viewPacket:(regina::Packet*)packet;

/**
 * Navigates to the given packet in the master view so that its list of children
 * is visible.  In addition, if there is only one child, that child will be
 * opened for viewing in the detail view.
 */
+ (void)viewChildren:(regina::Packet*)packet;

/**
 * Shows a notification banner for a brief period of time.
 * The message is mandatory; the detail is optional.
 */
+ (void)notify:(NSString*)message detail:(NSString*)detail;

/**
 * Displays a HUD while the given code block runs in a background thread.
 *
 * The cleanup block is optional; if non-nil, it will run on the main thread
 * after the code block has finished.  This can be used (for instance) to
 * update elements of the user interface.
 */
+ (void)runWithHUD:(NSString*)message code:(void(^)())code cleanup:(void(^)())cleanup;

/**
 * Is this app running on iOS 9 or above?
 */
+ (BOOL)ios9;

/**
 * Initialises this helper class.  This should be called once at startup.
 */
+ (void)initWithApp:(AppDelegate*)app;

@end

