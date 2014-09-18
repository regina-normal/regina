
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

#import "AppDelegate.h"
#import "DetailViewController.h"
#import "MasterViewController.h"
#import "PacketTreeController.h"
#import "ReginaHelper.h"

static UISplitViewController* split;
static UINavigationController* masterNav;
static UINavigationController* detailNav;
static MasterViewController* master;
static DetailViewController* detail;

@implementation ReginaHelper

+ (MasterViewController *)master
{
    return master;
}

+ (DetailViewController *)detail
{
    return detail;
}

+ (ReginaDocument *)document
{
    return detail.doc;
}

+ (PacketTreeController *)tree
{
    UIViewController* tree = masterNav.topViewController;
    if ([tree.class isSubclassOfClass:[PacketTreeController class]])
        return static_cast<PacketTreeController*>(tree);
    else
        return nil;
}

+ (void)viewPacket:(regina::NPacket *)packet
{
    detail.packet = packet;
    // Note: the following call is safe even if tree == nil.
    [[ReginaHelper tree] selectPacket:packet];
}

+ (void)initWithApp:(AppDelegate *)app
{
    split = (UISplitViewController *)app.window.rootViewController;

    masterNav = [split.viewControllers firstObject];
    master = (id)masterNav.topViewController;

    detailNav = [split.viewControllers lastObject];
    detail = (id)detailNav.topViewController;

    split.delegate = detail;
}

@end

