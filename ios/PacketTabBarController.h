
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

#import "PacketListenerIOS.h"
#import "PacketViewer.h"

namespace regina {
    class Packet;
}

@interface PacketTabBarController : UITabBarController

/**
 * Manually set the selected images for tab bar icons.
 * This is necessary because Xcode 6.0.1 seems to have a bug:
 * setting the selected image for a tab icon in the storyboard
 * results in an "Invalid asset name" error at runtime (and a null image).
 *
 * Once the bug in Xcode is fixed, this method can be safely removed.
 */
- (void)setSelectedImages:(NSArray*)imageNames;

/**
 * This must be called from viewDidLoad.
 */
- (void)registerDefaultKey:(NSString*)key;

/**
 * Calls reloadPacket on the current tab.
 */
- (void)reloadPacket;

/**
 * Calls endEditing on the current tab if it is a packet editor.
 */
- (void)endEditing;

@end
