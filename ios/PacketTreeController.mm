
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

#pragma mark - Packet tree row

@interface PacketTreeRow : NSObject

@property (assign, nonatomic, readonly) regina::NPacket* packet;
@property (assign, nonatomic, readonly) bool subtree;

- (id)initWithPacket:(regina::NPacket*)p subtree:(bool)s;
+ (id)packetTreeRowWithPacket:(regina::NPacket*)p subtree:(bool) s;

@end

@implementation PacketTreeRow

- (id)initWithPacket:(regina::NPacket*)p subtree:(bool)s {
    self = [super init];
    if (self) {
        _packet = p;
        _subtree = s;
    }
    return self;
}

+ (id)packetTreeRowWithPacket:(regina::NPacket*)p subtree:(bool) s {
    return [[PacketTreeRow alloc] initWithPacket:p subtree:s];
}

@end

#pragma mark - Custom packet listener

@interface PacketTreeListener : PacketListenerIOS {
    PacketTreeController* _tree;
}
- (id)initWithTree:(PacketTreeController*)tree;
@end

@implementation PacketTreeListener

- (id)initWithTree:(PacketTreeController *)tree {
    self = [super init];
    if (self) {
        _tree = tree;
        
        // Listen on both the subtree root and its immediate children.
        [self listen:_tree.node];
        
        for (regina::NPacket* p = _tree.node->getFirstTreeChild(); p; p = p->getNextTreeSibling())
            [self listen:p];
    }
    return self;
}

- (void)packetWasChanged:(regina::NPacket *)packet {
    [_tree.document setDirty];
}

- (void)packetWasRenamed:(regina::NPacket *)packet {
    // TODO
    [_tree.document setDirty];
    [_tree refreshPackets];
}

- (void)childWasAddedTo:(regina::NPacket*)packet child:(regina::NPacket*)child {
    // TODO
    [_tree.document setDirty];
    [_tree refreshPackets];
    if (packet == _tree.node)
        [self listen:child];
}

- (void)childWasRemovedFrom:(regina::NPacket *)packet child:(regina::NPacket *)child inParentDestructor:(bool)d {
    [_tree.document setDirty];
}

- (void)childrenWereReordered:(regina::NPacket *)packet {
    [_tree.document setDirty];
}

@end

#pragma mark - Packet tree controller

@interface PacketTreeController () {
    NSMutableArray *_rows;
    NSInteger _subtreeRow;
    PacketListenerIOS* _listener;
    __weak DetailViewController* _detail;
    __weak UIPopoverController* _newPacketPopover;
}

- (void)fill;

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
        _listener = [[PacketTreeListener alloc] initWithTree:self];
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
        NSLog(@"Opened.");
        _node = doc.tree;
        _listener = [[PacketTreeListener alloc] initWithTree:self];
        self.title = doc.localizedName;
        [self refreshPackets];
        [MBProgressHUD hideHUDForView:rootView animated:YES];
        // TODO: Enable packet + button.
    }];
    
    _detail.doc = doc;
    // TODO: Trap errors.
}

- (void)openSubtree:(regina::NPacket *)p detail:(DetailViewController *)d {
    _detail = d;
    _node = p;
    [self setTitle:[NSString stringWithUTF8String:p->getPacketLabel().c_str()]];

    _listener = [[PacketTreeListener alloc] initWithTree:self];
}

- (void)fill {
    if (! _node)
        return;
    
    _rows = [NSMutableArray array];
    
    regina::NPacket* p;
    for (p = _node->getFirstTreeChild(); p; p = p->getNextTreeSibling())
        [_rows addObject:[PacketTreeRow packetTreeRowWithPacket:p subtree:false]];
}

- (void)refreshPackets {
    [self fill];
    [self.tableView reloadData];
}

- (void)viewPacket:(regina::NPacket *)p {
    _detail.packet = p;
}

- (void)newPacket:(regina::PacketType)type {
    if (_newPacketPopover)
        [_newPacketPopover dismissPopoverAnimated:NO];

    NewPacketSpec* spec = [NewPacketSpec specWithType:type tree:self];
    if (! [spec parentWithAlert:YES])
        return;

    switch (type) {
        case regina::PACKET_CONTAINER:
        {
            // We can do this immediately, no input required.
            regina::NContainer* c = new regina::NContainer();
            c->setPacketLabel("Container");
            _node->insertChildLast(c);
            break;
        }
        case regina::PACKET_TEXT:
        {
            // We can do this immediately, no input required.
            regina::NText* t = new regina::NText();
            t->setPacketLabel("Text");
            t->setText("Type your text here.");
            _node->insertChildLast(t);
            _detail.packet = t;
            break;
        }
        case regina::PACKET_TRIANGULATION:
            [self performSegueWithIdentifier:@"newTriangulation" sender:spec];
            break;
        case regina::PACKET_DIM2TRIANGULATION:
            [self performSegueWithIdentifier:@"newDim2Triangulation" sender:spec];
            break;
        case regina::PACKET_NORMALSURFACELIST:
            [self performSegueWithIdentifier:@"newSurfaces" sender:spec];
            break;
        default:
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Not yet implemented."
                                                            message:@"I do not know how to create this type of packet.  The iOS version of Regina is still under development – please be patient!"
                                                           delegate:self
                                                  cancelButtonTitle:@"Close"
                                                  otherButtonTitles:nil];
            [alert show];
            break;
    }
}

- (ReginaDocument *)document
{
    return _detail.doc;
}

- (regina::NPacket *)viewingPacket
{
    return _detail.packet;
}

#pragma mark - Table View

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return (_subtreeRow > 0 ? [_rows count] + 1 : [_rows count]);
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (_subtreeRow > 0 && indexPath.row == _subtreeRow)
        return [tableView dequeueReusableCellWithIdentifier:@"Subtree" forIndexPath:indexPath];

    NSInteger rowIndex;
    if (_subtreeRow == 0 || _subtreeRow > indexPath.row)
        rowIndex = indexPath.row;
    else
        rowIndex = indexPath.row - 1;
    
    PacketTreeRow* r = _rows[rowIndex];
    regina::NPacket* p = [r packet];

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
    return NO;
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
        PacketTreeRow* r = _rows[indexPath.row];
        [self viewPacket:r.packet];
        return;
    }

    NSInteger rowIndex;
    if (_subtreeRow == 0 || _subtreeRow > indexPath.row)
        rowIndex = indexPath.row;
    else
        rowIndex = indexPath.row - 1;

    NSInteger oldSubtreeRow = _subtreeRow;
    
    PacketTreeRow* r = _rows[rowIndex];
    regina::NPacket* p = [r packet];
    if (p->getPacketType() != regina::NContainer::packetType && p->getFirstTreeChild()) {
        _subtreeRow = rowIndex + 1;
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
        [self viewPacket:r.packet];
}

#pragma mark Segues

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

        NSInteger packetIndex;
        if (_subtreeRow <= indexPath.row && _subtreeRow > 0)
            packetIndex = indexPath.row - 1;
        else
            packetIndex = indexPath.row;
        
        [[segue destinationViewController] openSubtree:[_rows[packetIndex] packet] detail:_detail];
        [[segue destinationViewController] refreshPackets];
    } else if ([[segue identifier] isEqualToString:@"newPacket"]) {
        _newPacketPopover = [(UIStoryboardPopoverSegue*)segue popoverController];
        ((NewPacketMenu*)segue.destinationViewController).packetTree = self;
    } else {
        // This must be one of the new packet segues.
        // Pass through the parent packet.
        ((NewPacketController*)segue.destinationViewController).spec = sender;
    }
}

@end
