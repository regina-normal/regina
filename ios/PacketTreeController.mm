
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
#import "NewSurfacesController.h" // TODO: Subclass
#import "PacketManagerIOS.h"
#import "PacketTreeController.h"

#import "file/nxmlfile.h"
#import "packet/npacket.h"
#import "packet/ncontainer.h"

#pragma mark Packet tree row

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

#pragma mark - Packet tree controller

@interface PacketTreeController () {
    NSMutableArray *_rows;
    NSInteger _subtreeRow;
    DetailViewController *_detail;
    __weak UIPopoverController* _newPacketPopover;
}

@property (assign, nonatomic) regina::NPacket* tree;
@property (assign, nonatomic) regina::NPacket* node;

- (bool)loadTreeResource:(NSString*)filename;
- (void)fill;

@end

@implementation PacketTreeController

- (void)viewDidLoad {
    [super viewDidLoad];

    _detail = (DetailViewController*)
        [[[[self splitViewController] viewControllers] lastObject] topViewController];
}

- (void)viewWillDisappear:(BOOL)animated {
    if (_node && ! _node->getTreeParent()) {
        if ([self.navigationController.viewControllers indexOfObject:self] == NSNotFound) {
            NSLog(@"Closing file and deleting from memory.");
            delete _tree;
            [_detail viewWelcome];
        }
    }
    [super viewDidDisappear:animated];
}

- (void)openExample:(Example*)e {
    [self loadTreeResource:e.file];
    // TODO: Trap errors.
}

- (void)openSubtree:(regina::NPacket *)p root:(regina::NPacket*)r {
    _tree = r;
    _node = p;
    [self setTitle:[NSString stringWithUTF8String:p->getPacketLabel().c_str()]];
}

- (bool)loadTreeResource:(NSString*)filename {
    NSBundle* mainBundle = [NSBundle mainBundle];
    if (! mainBundle) {
        NSLog(@"Could not access main bundle.");
        return false;
    }
    
    NSString* path = [mainBundle pathForResource:filename ofType:nil inDirectory:@"examples"];
    if (! path) {
        NSLog(@"Could not find resource: %@", filename);
        return false;
    }
    
    _tree = regina::readXMLFile([path UTF8String]);
    if (! _tree) {
        NSLog(@"Failed to read data file: %@", path);
        return false;
    }

    NSLog(@"Loaded file: %@", filename);
    _node = _tree;
    return true;
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
    // TODO: Make this easier to get to.
    UINavigationController* nav = [[self splitViewController].viewControllers lastObject];
    DetailViewController* detail = (DetailViewController*)nav.topViewController;
    [detail viewPacket:p];
}

- (void)dismissNewPacketPopover {
    [_newPacketPopover dismissPopoverAnimated:NO];
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
    [self viewPacket:r.packet];
}

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
        
        [[segue destinationViewController] openSubtree:[_rows[packetIndex] packet] root:_tree];
        [[segue destinationViewController] refreshPackets];
    } else if ([[segue identifier] isEqualToString:@"newPacket"]) {
        _newPacketPopover = [(UIStoryboardPopoverSegue*)segue popoverController];
        ((NewPacketController*)segue.destinationViewController).packetTree = self;
    } else {
        // This must be one of the new packet segues.
        // Pass through the parent packet.
        // TODO: Decide whether the "right" parent is the tree, or the visible packet.
        // TODO: Introduce a common base class.
        ((NewSurfacesController*)segue.destinationViewController).parentPacket = _node;
    }
}

@end
