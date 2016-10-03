
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
#import "packet/packettype.h"

namespace regina {
    class Packet;
}

/**
 * A picker view that displays all packets of the given type in the
 * current document.
 *
 * You must call \a fill in order to fill the picker with options.
 */
@interface PacketPicker : UIPickerView

/**
 * Fills the picker with options.  This function must be called before
 * the picker can be used.
 *
 * Note that if (for whatever reason) you call \a fill a second time,
 * you will need to call \a reloadAllComponents to update the view.
 *
 * @param tree The root of the packet tree for the current document.
 * @param packetType The type of packets that will be offered.
 * @param allowNone Indicates whether the null packet should be offered
 * at the top of the list of options.  If there are no packets of the given type,
 * then the null packet will be offered regardless of what you pass as \a allowNone.
 * @param noneText The text to display for the null packet.
 */
- (void)fill:(regina::Packet*)tree type:(regina::PacketType)packetType allowNone:(BOOL)allowNone noneText:(NSString*)noneText;

/**
 * Like fill: above, but supports two different packet types.
 * This is useful (for instance) when you wish to allow both NTriangulation and SnapPeaTriangulation.
 */
- (void)fill:(regina::Packet*)tree type1:(regina::PacketType)packetType1 type2:(regina::PacketType)packetType2 allowNone:(BOOL)allowNone noneText:(NSString*)noneText;

/**
 * Returns the packet selected in this picker, or 0 if the null packet is selected.
 */
- (regina::Packet*)selectedPacket;

/**
 * Returns whether this picker contains no valid options at all.
 * This will only be the case if \a fill was called with \a allowNone = NO,
 * and the document contains no packets of the given type.
 */
- (BOOL)empty;

@end
