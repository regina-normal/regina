
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

#import "DetailViewController.h"
#import "NewPacketController.h"
#import "ReginaHelper.h"
#import "PacketTreeController.h"
#import "packet/packet.h"

#pragma mark - New packet specification

@implementation NewPacketSpec

- (id)initWithType:(regina::PacketType)type tree:(PacketTreeController *)tree
{
    self = [super init];
    if (self) {
        _type = type;
        _viewOnCreation = YES;

        regina::Packet* viewing = [ReginaHelper detail].packet;
        if (type == regina::PACKET_NORMALSURFACES || type == regina::PACKET_ANGLESTRUCTURES) {
            if (viewing && [NewPacketSpec isTriangulation:viewing])
                _parent = viewing;
            else if ([NewPacketSpec isTriangulation:tree.node])
                _parent = tree.node;
            else
                _parent = 0;
        } else if (type == regina::PACKET_NORMALHYPERSURFACES) {
            if (viewing && viewing->type() == regina::PACKET_TRIANGULATION4)
                _parent = viewing;
            else if (tree.node->type() == regina::PACKET_TRIANGULATION4)
                _parent = tree.node;
            else
                _parent = 0;
        } else
            _parent = tree.node;
    }
    return self;
}

+ (id)specWithType:(regina::PacketType)type tree:(PacketTreeController *)tree
{
    return [[NewPacketSpec alloc] initWithType:type tree:tree];
}

- (id)initWithType:(regina::PacketType)type parent:(regina::Packet *)parent
{
    self = [super init];
    if (self) {
        _type = type;
        _parent = parent;
        _viewOnCreation = YES;
    }
    return self;
}

+ (id)specWithType:(regina::PacketType)type parent:(regina::Packet *)parent
{
    return [[NewPacketSpec alloc] initWithType:type parent:parent];
}

- (BOOL)hasParentWithAlert
{
    if (self.parent)
        return YES;

    NSString* title = @"Enumerate in which triangulation?";
    NSString* msg;
    switch (self.type) {
        case regina::PACKET_NORMALSURFACES:
            msg = @"Please select the 3-D triangulation in which I should enumerate normal surfaces.";
            break;
        case regina::PACKET_NORMALHYPERSURFACES:
            msg = @"Please select the 4-D triangulation in which I should enumerate normal hypersurfaces.";
            break;
        case regina::PACKET_ANGLESTRUCTURES:
            msg = @"Please select the 3-D triangulation in which I should enumerate angle structures.";
            break;
        default:
            msg = nil;
            break;
    }

    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title
                                                    message:msg
                                                   delegate:nil
                                          cancelButtonTitle:@"Close"
                                          otherButtonTitles:nil];
    [alert show];
    return NO;
}

- (void)created:(regina::Packet *)result
{
    if (result) {
        if (self.viewOnCreation && result->type() != regina::PACKET_CONTAINER)
            [ReginaHelper viewPacket:result];
        else
            [[ReginaHelper tree] selectPacket:result];
    }
}

+ (BOOL)isTriangulation:(regina::Packet*)p
{
    return (p->type() == regina::PACKET_TRIANGULATION3 || p->type() == regina::PACKET_SNAPPEATRIANGULATION);
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
