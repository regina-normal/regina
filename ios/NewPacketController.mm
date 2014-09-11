
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

#import "DetailViewController.h"
#import "NewPacketController.h"
#import "PacketTreeController.h"
#import "packet/npacket.h"

#pragma mark - New packet specification

@interface NewPacketSpec () {
    /**
     * The parent beneath which the new packet should be created.
     * This will be set by parent or parentWithAlert.
     */
    regina::NPacket* _parent;
    /**
     * The subtree that is currently open in the master packet tree view.
     */
    regina::NPacket* _subtree;
    /**
     * The packet that we are currently viewing in the detail view.
     */
    regina::NPacket* _viewing;
}
@end

@implementation NewPacketSpec

- (id)initWithType:(regina::PacketType)type tree:(PacketTreeController *)t {
    self = [super init];
    if (self) {
        _type = type;
        _tree = t;
        _subtree = t.node;
        _viewing = t.viewingPacket;
    }
    return self;
}

+ (id)specWithType:(regina::PacketType)type tree:(PacketTreeController *)t {
    return [[NewPacketSpec alloc] initWithType:type tree:t];
}

- (regina::NPacket*)parent {
    if (_parent)
        return _parent;
    else
        return [self parentWithAlert:NO];
}

- (regina::NPacket*)parentWithAlert:(BOOL)alert {
    if (_type == regina::PACKET_NORMALSURFACELIST || _type == regina::PACKET_ANGLESTRUCTURELIST) {
        if (_viewing && [NewPacketSpec isTriangulation:_viewing])
            _parent = _viewing;
        else if ([NewPacketSpec isTriangulation:_subtree])
            _parent = _subtree;
        else {
            if (alert) {
                NSString *title, *msg;
                if (_type == regina::PACKET_NORMALSURFACELIST) {
                    title = @"Enumerate in which triangulation?";
                    msg = @"Please select the triangulation in which I should enumerate normal surfaces.";
                } else {
                    title = @"Enumerate in which triangulation?";
                    msg = @"Please select the triangulation in which I should enumerate angle structures.";
                }
                _parent = 0;
                UIAlertView *a = [[UIAlertView alloc] initWithTitle:title
                                                            message:msg
                                                           delegate:nil
                                                  cancelButtonTitle:@"Close"
                                                  otherButtonTitles:nil];
                [a show];
            }
        }
    } else
        _parent = _subtree;
    
    return _parent;
}

+ (BOOL)isTriangulation:(regina::NPacket*)p
{
    return (p->getPacketType() == regina::PACKET_TRIANGULATION || p->getPacketType() == regina::PACKET_SNAPPEATRIANGULATION);
}

@end

#pragma mark - New packet controller

@implementation NewPacketController

- (BOOL)disablesAutomaticKeyboardDismissal
{
    // In text fields we want the keyboard to be automatically dismissed on [return],
    // and for a modal form sheet this does not happen automatically.
    return NO;
}

@end
