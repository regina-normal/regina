
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
#import "PacketManagerIOS.h"
#import "packet/npacket.h"
#import "packet/packettype.h"

bool initialised = NO;

@interface DetailViewController () {
    NSString* _menuTitle;
    UIViewController* _sub;
}
@end

@implementation DetailViewController

#pragma mark - Managing the detail item

- (void)renamePortraitPulloverButton:(NSString*)title
{
    _menuTitle = title;
    UIBarButtonItem* left = self.navigationItem.leftBarButtonItem;
    if (left)
        left.title = _menuTitle;
}

- (void)setDoc:(ReginaDocument *)doc
{
    if (doc == _doc && initialised)
        return;
    initialised = YES;
    
    if (_doc) {
        NSLog(@"Closing document...");
        [_doc closeWithCompletionHandler:^(BOOL success) {
            NSLog(@"Closed.");
        }];
    }
    
    _doc = doc;
    _packet = nil;
    
    if (doc) {
        // Display an empty panel.
        [self navigationItem].title = @"";
        [_sub performSegueWithIdentifier:@"embedEmpty" sender:nil];
        
        [self renamePortraitPulloverButton:@"Packets"];
    } else {
        // Display the welcome screen.
        [self navigationItem].title = @"";
        [_sub performSegueWithIdentifier:@"embedWelcome" sender:nil];

        [self renamePortraitPulloverButton:@"Documents"];
    }
}

- (void)setPacket:(regina::NPacket *)p {
    _packet = p;
    
    if (p == nil) {
        // Display an empty panel.
        [self navigationItem].title = @"";
        [_sub performSegueWithIdentifier:@"embedEmpty" sender:nil];
    } else {
        // Display the packet viewer / editor.
        [self navigationItem].title = [NSString stringWithUTF8String:p->getPacketLabel().c_str()];
        [_sub performSegueWithIdentifier:[PacketManagerIOS segueFor:p] sender:self];
    }
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // This is the embed segue.
    _sub = segue.destinationViewController;
}

#pragma mark - Split view

- (void)splitViewController:(UISplitViewController *)splitController willHideViewController:(UIViewController *)viewController withBarButtonItem:(UIBarButtonItem *)barButtonItem forPopoverController:(UIPopoverController *)popoverController
{
    barButtonItem.title = _menuTitle;
    [self.navigationItem setLeftBarButtonItem:barButtonItem animated:YES];
}

- (void)splitViewController:(UISplitViewController *)splitController willShowViewController:(UIViewController *)viewController invalidatingBarButtonItem:(UIBarButtonItem *)barButtonItem
{
    // Called when the view is shown again in the split view, invalidating the button and popover controller.
    [self.navigationItem setLeftBarButtonItem:nil animated:YES];
}

@end
