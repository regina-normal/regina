
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
#import "MBProgressHUD.h"
#import "NewPacketController.h"
#import "NewPacketMenu.h"
#import "PacketListenerIOS.h"
#import "PacketManagerIOS.h"
#import "PacketTreeController.h"

#import "file/nxmlfile.h"
#import "packet/npacket.h"
#import "packet/ncontainer.h"
#import "packet/ntext.h"

// TODO: Move packets around the tree.

@interface PacketTreeController () <UIAlertViewDelegate, UIActionSheetDelegate, PacketDelegate> {
    NSPointerArray *_packets;
    NSMutableDictionary *_packetIndices;
    NSInteger _subtreeRow;
    PacketListenerIOS* _listener;
    __weak UIPopoverController* _newPacketPopover;
}

@end

@implementation PacketTreeController

- (void)newDocumentWithDetail:(DetailViewController *)d
{
    NSLog(@"Creating new document...");
    
    _detail = d;
    _detail.doc = [ReginaDocument documentNewWithCompletionHandler:^(ReginaDocument* doc) {
        if (! doc) {
            NSLog(@"Could not create.");
            return;
        }
        NSLog(@"Created.");
        _node = doc.tree;
        _listener = [PacketListenerIOS listenerWithPacket:_node delegate:self listenChildren:YES];
        self.title = doc.localizedName;
        [self refreshPackets];
    }];
}

- (void)openDocument:(ReginaDocument*)doc detail:(DetailViewController *)d
{
    NSLog(@"Opening document...");
    
    _detail = d;
    
    // We use an activity indicator since files could take some time to load.
    UIView* rootView = [UIApplication sharedApplication].keyWindow.rootViewController.view;
    MBProgressHUD* hud = [MBProgressHUD showHUDAddedTo:rootView animated:YES];
    [hud setLabelText:@"Loading"];
    
    [doc openWithCompletionHandler:^(BOOL success) {
        [MBProgressHUD hideHUDForView:rootView animated:YES];
        if (! success) {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Could Not Open Document"
                                                            message:@"I can read Regina's own data files, as well as SnapPea/SnapPy triangulation files.  Please ensure that your document uses one of these file formats."
                                                           delegate:self
                                                  cancelButtonTitle:@"Close"
                                                  otherButtonTitles:nil];
            
            // Ensure that this tree controller is popped when the alert is dismissed.
            alert.delegate = self;
            
            [alert show];
            return;
        }
        NSLog(@"Opened.");
        _node = doc.tree;
        _listener = [PacketListenerIOS listenerWithPacket:_node delegate:self listenChildren:YES];
        self.title = doc.localizedName;
        [self refreshPackets];
    }];
    
    _detail.doc = doc;
}

- (void)openSubtree:(regina::NPacket *)p detail:(DetailViewController *)d {
    _detail = d;
    _node = p;
    _listener = [PacketListenerIOS listenerWithPacket:_node delegate:self listenChildren:YES];
    self.title = [NSString stringWithUTF8String:p->getPacketLabel().c_str()];
    [self refreshPackets];
}

- (void)refreshPackets {
    if (! _node)
        return;

    _packets = [NSPointerArray pointerArrayWithOptions:NSPointerFunctionsOpaqueMemory | NSPointerFunctionsOpaquePersonality];
    _packetIndices = [NSMutableDictionary dictionary];

    regina::NPacket* p;
    int index = 0;
    for (p = _node->getFirstTreeChild(); p; p = p->getNextTreeSibling(), ++index) {
        [_packets addPointer:p];
        [_packetIndices setObject:[NSNumber numberWithInt:index] forKey:[NSValue valueWithPointer:p]];
    }

    [self.tableView reloadData];
}

- (int)packetIndexForPath:(NSIndexPath*)indexPath {
    if (_subtreeRow <= indexPath.row && _subtreeRow > 0)
        return indexPath.row - 1;
    else
        return indexPath.row;
}

// Note: if indexPath points to the subtree row, this returns the associated packet.
- (regina::NPacket*)packetForPath:(NSIndexPath*)indexPath {
    if (_subtreeRow <= indexPath.row && _subtreeRow > 0)
        return static_cast<regina::NPacket*>([_packets pointerAtIndex:(indexPath.row - 1)]);
    else
        return static_cast<regina::NPacket*>([_packets pointerAtIndex:indexPath.row]);
}

- (int)packetIndexForPacket:(regina::NPacket*)packet {
    NSNumber *packetIndex = [_packetIndices objectForKey:[NSValue valueWithPointer:packet]];
    return (packetIndex == nil ? -1 : packetIndex.intValue);
}

- (NSIndexPath*)pathForPacket:(regina::NPacket*)packet {
    NSNumber *packetIndex = [_packetIndices objectForKey:[NSValue valueWithPointer:packet]];
    if (packetIndex == nil)
        return nil;
    int i = packetIndex.intValue;
    if (_subtreeRow == 0 || _subtreeRow > i)
        return [NSIndexPath indexPathForRow:i inSection:0];
    else
        return [NSIndexPath indexPathForRow:(i + 1) inSection:0];
}

- (NSIndexPath*)pathForPacketIndex:(int)index {
    if (_subtreeRow == 0 || _subtreeRow > index)
        return [NSIndexPath indexPathForRow:index inSection:0];
    else
        return [NSIndexPath indexPathForRow:(index + 1) inSection:0];
}

- (void)viewPacket:(regina::NPacket *)p {
    if (p->getPacketType() != regina::PACKET_CONTAINER) {
        // View this packet in detail.
        self.detail.packet = p;
    }

    if (p->getTreeParent() == self.node) {
        // This is one of the packets in the master list.
        // Select it (if this is safe), and make sure it is visible.
        NSIndexPath* path = [self pathForPacket:p];
        if (p->getPacketType() != regina::PACKET_CONTAINER && ! [self.tableView.indexPathForSelectedRow isEqual:path])
            [self.tableView selectRowAtIndexPath:path animated:NO scrollPosition:UITableViewScrollPositionNone];
        [self.tableView scrollToRowAtIndexPath:[self pathForPacket:p] atScrollPosition:UITableViewScrollPositionNone animated:YES];
    }
}

- (void)newPacket:(regina::PacketType)type {
    if (_newPacketPopover)
        [_newPacketPopover dismissPopoverAnimated:NO];

    NewPacketSpec* spec = [NewPacketSpec specWithType:type tree:self];
    if (! [spec parentWithAlert:YES])
        return;

    [PacketManagerIOS newPacket:spec];
}

- (void)dealloc
{
    [_listener permanentlyUnlisten];
}

- (ReginaDocument *)document
{
    return self.detail.doc;
}

- (regina::NPacket *)viewingPacket
{
    return self.detail.packet;
}

#pragma mark - Segues

- (BOOL)shouldPerformSegueWithIdentifier:(NSString *)identifier sender:(id)sender {
    // Note that _newPacketPopover is a zeroing weak reference, and will
    // automagically return to nil when the popover is dismissed.
    if ([identifier isEqualToString:@"newPacket"] && _newPacketPopover) {
        // The popover is already visible.
        // Instead of showing it, we should dismiss it (since the button
        // was pressed a second time).
        [_newPacketPopover dismissPopoverAnimated:NO];
        return NO;
    }
    return YES;
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    if ([[segue identifier] isEqualToString:@"openSubtree"]) {
        NSIndexPath *indexPath = [self.tableView indexPathForSelectedRow];
        [[segue destinationViewController] openSubtree:[self packetForPath:indexPath] detail:self.detail];
    } else if ([[segue identifier] isEqualToString:@"newPacket"]) {
        _newPacketPopover = [(UIStoryboardPopoverSegue*)segue popoverController];
        ((NewPacketMenu*)segue.destinationViewController).packetTree = self;
    } else {
        // This must be one of the new packet segues.
        // Pass through the parent packet.
        ((NewPacketController*)segue.destinationViewController).spec = sender;
    }
}

#pragma mark - Packet listener

- (void)packetWasChanged:(regina::NPacket *)packet {
    [self.document setDirty];
}

- (void)packetWasRenamed:(regina::NPacket *)packet {
    [self.document setDirty];
    if (packet == self.node) {
        // Refresh the title, but only if this is not the root of the packet tree.
        if (packet->getTreeParent())
            self.title = [NSString stringWithUTF8String:packet->getPacketLabel().c_str()];
    } else {
        NSIndexPath* path = [self pathForPacket:packet];
        if (path)
            [self.tableView reloadRowsAtIndexPaths:@[path] withRowAnimation:UITableViewRowAnimationAutomatic];
    }
}

- (void)childWasAddedTo:(regina::NPacket*)packet child:(regina::NPacket*)child {
    [self.document setDirty];

    NSIndexPath* path;
    if (packet == self.node) {
        // We have a new row for our table of children.
        int childIndex;
        if (! child->getNextTreeSibling()) {
            // The new row goes on the end of the list.
            childIndex = _packets.count;
            [_packets addPointer:child];
        } else {
            childIndex = [self packetIndexForPacket:child->getNextTreeSibling()] - 1;
            NSAssert(childIndex >= 0, @"childWasAddedTo: childIndex should be non-negative.");
            [_packets insertPointer:child atIndex:childIndex];
        }
        [_packetIndices setObject:[NSNumber numberWithInt:childIndex] forKey:[NSValue valueWithPointer:child]];

        path = [self pathForPacketIndex:childIndex];
        [self.tableView insertRowsAtIndexPaths:@[path] withRowAnimation:UITableViewRowAnimationAutomatic];
    } else {
        // One of our children needs a new subtitle (which counts *its* children).
        path = [self pathForPacket:child->getTreeParent()];
        if (path)
            [self.tableView reloadRowsAtIndexPaths:@[path] withRowAnimation:UITableViewRowAnimationAutomatic];
    }

    // Disable the following code, since the new packet actions will call viewPacket:,
    // which scrolls the table anyway.
    /*
    if (path)
        [self.tableView scrollToRowAtIndexPath:path atScrollPosition:UITableViewScrollPositionNone animated:YES];
     */
}

- (void)childWasRemovedFrom:(regina::NPacket *)packet child:(regina::NPacket *)child inParentDestructor:(bool)d {
    // No need to update the table, since this action can only have happened as a result
    // of user interaction with the table.
    [self.document setDirty];
}

- (void)childrenWereReordered:(regina::NPacket *)packet {
    // No need to update the table, since this action can only have happened as a result
    // of user interaction with the table.
    [self.document setDirty];
}

#pragma mark - Editable table view

- (BOOL)renameAllowed:(NSIndexPath *)path
{
    return ! (_subtreeRow > 0 && _subtreeRow == path.row);
}

- (NSString *)renameInit:(NSIndexPath *)path
{
    return [NSString stringWithUTF8String:[self packetForPath:path]->getPacketLabel().c_str()];
}

- (void)renameDone:(NSIndexPath *)path result:(NSString *)result
{
    [self packetForPath:path]->setPacketLabel([[result stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] UTF8String]);
}

#pragma mark - Table view

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return (_subtreeRow > 0 ? [_packets count] + 1 : [_packets count]);
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (_subtreeRow > 0 && indexPath.row == _subtreeRow)
        return [tableView dequeueReusableCellWithIdentifier:@"Subtree" forIndexPath:indexPath];

    regina::NPacket* p = [self packetForPath:indexPath];

    UITableViewCell *cell;
    if (p->getPacketType() == regina::NContainer::packetType)
        cell = [tableView dequeueReusableCellWithIdentifier:@"Container" forIndexPath:indexPath];
    else
        cell = [tableView dequeueReusableCellWithIdentifier:@"Packet" forIndexPath:indexPath];
    cell.textLabel.text = [NSString stringWithUTF8String:p->getPacketLabel().c_str()];
    unsigned long sub = p->getNumberOfChildren();
    if (sub == 0)
        cell.detailTextLabel.text = @"";
    else if (sub == 1)
        cell.detailTextLabel.text = @"1 subpacket";
    else
        cell.detailTextLabel.text = [NSString stringWithFormat:@"%lu subpackets", sub];
    cell.imageView.image = [PacketManagerIOS iconFor:p];

    return cell;
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    return (! self.actionPath) && (! (_subtreeRow > 0 && indexPath.row == _subtreeRow));
}

- (UITableViewCellEditingStyle)tableView:(UITableView *)tableView editingStyleForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (_subtreeRow > 0 && indexPath.row == _subtreeRow)
        return UITableViewCellEditingStyleNone;
    else
        return UITableViewCellEditingStyleDelete;
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (self.actionPath)
        return;

    if (editingStyle == UITableViewCellEditingStyleDelete) {
        self.actionPath = indexPath;
        CGRect cell = [tableView cellForRowAtIndexPath:indexPath].frame;
        
        NSString* deleteMsg;
        regina::NPacket* p = [self packetForPath:indexPath];
        unsigned long totalPackets = p->getNumberOfDescendants();
        if (totalPackets == 0)
            deleteMsg = @"Delete packet";
        else
            deleteMsg = [NSString stringWithFormat:@"Delete %ld packets", totalPackets + 1];

        UIActionSheet* sheet = [[UIActionSheet alloc] initWithTitle:nil
                                                           delegate:self
                                                  cancelButtonTitle:@"Cancel"
                                             destructiveButtonTitle:deleteMsg
                                                  otherButtonTitles:nil];
        [sheet showFromRect:cell inView:tableView animated:YES];
    }
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    if (_subtreeRow > 0 && indexPath.row == _subtreeRow) {
        // The user has selected the existing browse-subtree cell.
        // This is already attached to its own segue.
        // Nothing more to do here.
        return;
    }
    if (_subtreeRow == indexPath.row + 1) {
        // The user has selected the packet whose browse-subtree cell is
        // already visible.
        [self viewPacket:[self packetForPath:indexPath]];
        return;
    }

    NSInteger oldSubtreeRow = _subtreeRow;
    
    NSInteger packetIndex = [self packetIndexForPath:indexPath];
    regina::NPacket* p = static_cast<regina::NPacket*>([_packets pointerAtIndex:packetIndex]);
    if (p->getPacketType() != regina::NContainer::packetType && p->getFirstTreeChild()) {
        _subtreeRow = packetIndex + 1;
        if (oldSubtreeRow == 0) {
            [tableView insertRowsAtIndexPaths:[NSArray arrayWithObject:
                                               [NSIndexPath indexPathForRow:_subtreeRow inSection:0]]
                             withRowAnimation:UITableViewRowAnimationTop];
        } else {
            [tableView beginUpdates];
            [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:
                                               [NSIndexPath indexPathForRow:oldSubtreeRow inSection:0]]
                             withRowAnimation:UITableViewRowAnimationTop];
            [tableView insertRowsAtIndexPaths:[NSArray arrayWithObject:
                                               [NSIndexPath indexPathForRow:_subtreeRow inSection:0]]
                             withRowAnimation:UITableViewRowAnimationTop];
            [tableView endUpdates];
        }
    } else {
        _subtreeRow = 0;
        if (oldSubtreeRow > 0)
            [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:
                                               [NSIndexPath indexPathForRow:oldSubtreeRow inSection:0]]
                             withRowAnimation:UITableViewRowAnimationTop];
    }
    if (p->getPacketType() != regina::NContainer::packetType)
        [self viewPacket:p];
}

#pragma mark - Action sheet

- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex {
    if (buttonIndex == actionSheet.destructiveButtonIndex) {
        int packetIndex = [self packetIndexForPath:self.actionPath];
        regina::NPacket* packet = static_cast<regina::NPacket*>([_packets pointerAtIndex:packetIndex]);

        [_packets removePointerAtIndex:packetIndex];
        [_packetIndices removeObjectForKey:[NSValue valueWithPointer:packet]];

        if (_subtreeRow == self.actionPath.row + 1) {
            // We need to remove the subtree cell also.
            NSIndexPath* subtreePath = [NSIndexPath indexPathForRow:_subtreeRow inSection:0];
            _subtreeRow = 0;
            [self.tableView deleteRowsAtIndexPaths:@[self.actionPath, subtreePath]
                                  withRowAnimation:UITableViewRowAnimationFade];
        } else {
            [self.tableView deleteRowsAtIndexPaths:@[self.actionPath]
                                  withRowAnimation:UITableViewRowAnimationFade];
        }
        
        delete packet;
    }

    self.actionPath = nil;
}

#pragma mark - Alert view

- (void)alertView:(UIAlertView *)alertView willDismissWithButtonIndex:(NSInteger)buttonIndex
{
    // This is a consequence of failing to open a document.
    // Specifically, this is called after the corresponding "could not open" alert is dismissed.
    // Pop back to the main documents list.
    
    // We put the call to popViewControllerAnimated: here, after the alert is dismissed,
    // so that the forward animation that pushed this packet tree controller has time to finish
    // and we can thus avoid errors from nested push/pop segues.
    [self.navigationController popViewControllerAnimated:YES];
}

@end
