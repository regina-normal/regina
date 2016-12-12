
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

#import "PacketViewer.h"
#import <string>

/**
 * The text field that displays the value of a script variable.
 */
@interface ScriptVariableValueField : UITextField
@end

/**
 * A cell in a table of script variables.
 */
@interface ScriptVariableCell : UITableViewCell
@end

/**
 * The controller for viewing script packets.
 */
@interface ScriptViewController : UIViewController <PacketViewer>
- (bool)hasVariable:(const std::string&)name;
// The following methods must only be called from ScriptVariableCell.
// They assume the cell has already been edited; their job is to push the
// updates to the packet itself.
- (void)editedValue:(const std::string&)name value:(regina::Packet*)value;
- (void)editedName:(const std::string&)name rename:(const std::string&)newName;
@end
