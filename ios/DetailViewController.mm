
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
#import "MasterViewController.h"
#import "MBProgressHUD.h"
#import "PacketListenerIOS.h"
#import "PacketManagerIOS.h"
#import "PacketViewController.h"
#import "ReginaHelper.h"
#import "SwitchableSubcontroller.h"
#import "packet/npacket.h"
#import "packet/packettype.h"

@interface DetailViewController () <UIActionSheetDelegate, PacketDelegate> {
    NSString* _menuTitle;
    SwitchableSubcontroller* _sub;
    PacketListenerIOS* _listener;
    UIDocumentInteractionController* _interact;
}
@end

@implementation DetailViewController

- (void)dealloc
{
    [_listener permanentlyUnlisten];
}

- (void)renamePortraitPulloverButton:(NSString*)title
{
    _menuTitle = title;
    UIBarButtonItem* left = self.navigationItem.leftBarButtonItem;
    if (left)
        left.title = _menuTitle;
}

- (void)setDoc:(ReginaDocument *)doc forceRefresh:(BOOL)force
{
    if (doc == _doc && ! force)
        return;

    // Close any packet viewers that might be open,
    // push any outstanding changes to the calculation engine,
    // and segue to whatever the detail panel should display instead.
    [self setPacket:nil forceRefresh:YES emptySegue:(doc ? nil : @"embedWelcome")];

    if (_doc && _doc != doc) {
        NSLog(@"Closing document...");
        ReginaDocument* oldDoc = _doc;
        [_doc closeWithCompletionHandler:^(BOOL success) {
            NSLog(@"Closed.");
            if (oldDoc.type == DOC_NATIVE)
                [[ReginaHelper master] refreshURL:oldDoc.fileURL];
        }];
    }
    
    _doc = doc;
    _interact = nil;
    
    // Adjust items on the navigation bar as necessary.
    if (doc) {
        [self renamePortraitPulloverButton:@"Packets"];
        self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc]
                                                  initWithBarButtonSystemItem:UIBarButtonSystemItemAction
                                                  target:self
                                                  action:@selector(share)];
    } else {
        [self renamePortraitPulloverButton:@"Documents"];
        self.navigationItem.rightBarButtonItem = nil;
    }
}

- (void)setDoc:(ReginaDocument *)doc
{
    [self setDoc:doc forceRefresh:NO];
}

- (void)setPacket:(regina::NPacket *)p forceRefresh:(BOOL)force emptySegue:(NSString*)emptySegue {
    if (_packet == p && ! force)
        return;

    if (_packet) {
        // Push any outstanding changes to the calculation engine.
        PacketViewController* c = static_cast<PacketViewController*>([_sub subview]);
        [c endEditing];
    }

    _packet = p;
    
    if (p == nil) {
        // Display an empty panel.
        [self navigationItem].title = @"";
        [_sub performSegueWithIdentifier:(emptySegue ? emptySegue : @"embedEmpty") sender:nil];

        [_listener permanentlyUnlisten];
        _listener = nil;
    } else {
        // Display the packet viewer / editor.
        [self navigationItem].title = [NSString stringWithUTF8String:p->getPacketLabel().c_str()];
        [_sub performSegueWithIdentifier:[PacketManagerIOS segueFor:p] sender:self];

        _listener = [PacketListenerIOS listenerWithPacket:p delegate:self listenChildren:NO];
    }
}

- (void)setPacket:(regina::NPacket *)packet
{
    [self setPacket:packet forceRefresh:NO emptySegue:nil];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // This is the embed segue.
    if (! _sub) {
        _sub = segue.destinationViewController;
        
        // Now that we have identified _sub, refresh the detail view.
        [self setDoc:_doc forceRefresh:YES];
    }
}

- (void)shareDocument
{
    if (! _interact)
        _interact = [UIDocumentInteractionController interactionControllerWithURL:self.doc.fileURL];

    if (self.doc.hasUnsavedChanges) {
        UIView* rootView = [UIApplication sharedApplication].keyWindow.rootViewController.view;
        MBProgressHUD* hud = [MBProgressHUD showHUDAddedTo:rootView animated:YES];
        [hud setLabelText:@"Saving"];

        [self.doc saveToURL:self.doc.fileURL forSaveOperation:UIDocumentSaveForOverwriting completionHandler:^(BOOL success){
            [MBProgressHUD hideHUDForView:rootView animated:NO];
            if (success)
                [_interact presentOptionsMenuFromBarButtonItem:self.navigationItem.rightBarButtonItem animated:YES];
            else {
                UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Could Not Save"
                                                                message:nil
                                                               delegate:nil
                                                      cancelButtonTitle:@"Close"
                                                      otherButtonTitles:nil];
                [alert show];
            }
        }];
    } else {
        [_interact presentOptionsMenuFromBarButtonItem:self.navigationItem.rightBarButtonItem animated:YES];
    }
}

- (void)share
{
    if (! self.sharer)
        [self shareDocument];
    else {
        UIActionSheet* sheet = [[UIActionSheet alloc] initWithTitle:nil
                                                           delegate:self
                                                  cancelButtonTitle:@"Cancel"
                                             destructiveButtonTitle:nil
                                                  otherButtonTitles:@"Share document", self.sharer.shareText, nil];
        
        [sheet showFromBarButtonItem:self.navigationItem.rightBarButtonItem animated:YES];
    }
}

#pragma mark - Packet listener

- (void)packetWasRenamed:(regina::NPacket *)packet
{
    if (packet == self.packet)
        self.title = [NSString stringWithUTF8String:packet->getPacketLabel().c_str()];
}

- (void)packetToBeDestroyed:(regina::NPacket *)packet
{
    if (packet == self.packet)
        self.packet = nil;
}

#pragma mark - Action sheet

- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (buttonIndex == 0)
        [self shareDocument];
    else if (buttonIndex == 1)
        [self.sharer shareFromButton:self.navigationItem.rightBarButtonItem];
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
