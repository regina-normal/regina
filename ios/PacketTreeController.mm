
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
#import "NewPacketMenu.h"
#import "PacketManagerIOS.h"
#import "PacketTreeController.h"

#import "file/nxmlfile.h"
#import "packet/npacket.h"
#import "packet/ncontainer.h"
#import "packet/ntext.h"

#pragma mark - New packet specification

@interface NewPacketSpec : NSObject

/**
 * The type of packet to create.
 */
@property (assign, nonatomic, readonly) regina::PacketType type;
/**
 * The subtree that is currently open in the master packet tree view.
 */
@property (assign, nonatomic, readonly) regina::NPacket* subtree;
/**
 * The packet that we are currently viewing in the detail view.
 */
@property (assign, nonatomic, readonly) regina::NPacket* viewing;

- (id)initFor:(regina::PacketType)t subtree:(regina::NPacket*)s viewing:(regina::NPacket*)v;
+ (id)specFor:(regina::PacketType)t subtree:(regina::NPacket*)s viewing:(regina::NPacket*)v;

- (regina::NPacket*)parent;
- (regina::NPacket*)parentWithAlert:(BOOL)alert;

@end

@interface NewPacketSpec () {
    regina::NPacket* _parent;
}
@end

@implementation NewPacketSpec

- (id)initFor:(regina::PacketType)t subtree:(regina::NPacket *)s viewing:(regina::NPacket *)v {
    self = [super init];
    if (self) {
        _type = t;
        _subtree = s;
        _viewing = v;
    }
    return self;
}

+ (id)specFor:(regina::PacketType)t subtree:(regina::NPacket *)s viewing:(regina::NPacket *)v {
    return [[NewPacketSpec alloc] initFor:t subtree:s viewing:v];
}

- (regina::NPacket*)parent {
    if (_parent)
        return _parent;
    else
        return [self parentWithAlert:NO];
}

- (regina::NPacket*)parentWithAlert:(BOOL)alert {
    if (_type == regina::PACKET_NORMALSURFACELIST || _type == regina::PACKET_ANGLESTRUCTURELIST) {
        if (_viewing && _viewing->getPacketType() == regina::PACKET_TRIANGULATION)
            _parent = _viewing;
        else if (_subtree->getPacketType() == regina::PACKET_TRIANGULATION)
            _parent = _subtree;
        else {
            if (alert) {
                NSString *title, *msg;
                if (_type == regina::PACKET_NORMALSURFACELIST) {
                    title = @"Surfaces need a triangulation.";
                    msg = @"Please select the triangulation in which I should enumerate normal surfaces.";
                } else {
                    title = @"Angle structures need a triangulation.";
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

@end

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

#pragma mark - Packet tree controller

@interface PacketTreeController () {
    NSMutableArray *_rows;
    NSInteger _subtreeRow;
    NewPacketSpec *_newPacketSpec;
    __weak UIPopoverController* _newPacketPopover;
}

@property (assign, nonatomic) regina::NPacket* tree;
@property (assign, nonatomic) regina::NPacket* node;
@property (weak, nonatomic) DetailViewController *detail;

- (bool)loadTreeResource:(NSString*)filename;
- (void)fill;

@end

@implementation PacketTreeController

- (DetailViewController *)detail {
    // Sometimes this is initialised in viewDidLoad, but sometimes not..
    // in deeper levels of the tree it seems that viewDidLoad cannot
    // access the split view controller (in particular
    // [self splitViewController] returns nil).  Better re-initialise it
    // here if necessary.
    if (! _detail) {
        UISplitViewController* s = [self splitViewController];
        UINavigationController* nav = [s.viewControllers lastObject];
        _detail = (DetailViewController*)nav.topViewController;
    }
    return _detail;
}

- (void)viewDidLoad {
    [super viewDidLoad];

    UISplitViewController* s = [self splitViewController];
    UINavigationController* nav = [s.viewControllers lastObject];
    _detail = (DetailViewController*)nav.topViewController;
}

- (void)viewWillDisappear:(BOOL)animated {
    if (_node && ! _node->getTreeParent()) {
        if ([self.navigationController.viewControllers indexOfObject:self] == NSNotFound) {
            NSLog(@"Closing file and deleting from memory.");
            delete _tree;
            [self.detail viewWelcome];
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
    [self.detail viewPacket:p];
}

- (void)newPacket:(regina::PacketType)type {
    if (_newPacketPopover)
        [_newPacketPopover dismissPopoverAnimated:NO];
    
    _newPacketSpec = [NewPacketSpec specFor:type subtree:_node viewing:self.detail.packet];
    if (! [_newPacketSpec parentWithAlert:YES])
        return;

    // TODO: View the new packet immediately.
    // TODO: Replace calls to refreshPackets with a listener-based mechanism.
    switch (type) {
        case regina::PACKET_CONTAINER:
        {
            // We can do this immediately, no input required.
            regina::NContainer* c = new regina::NContainer();
            c->setPacketLabel("Container");
            _node->insertChildLast(c);
            [self refreshPackets];
            break;
        }
        case regina::PACKET_TEXT:
        {
            // We can do this immediately, no input required.
            regina::NText* t = new regina::NText();
            t->setPacketLabel("Text");
            t->setText("Type your text here...");
            _node->insertChildLast(t);
            [self refreshPackets];
            break;
        }
        case regina::PACKET_NORMALSURFACELIST:
            [self performSegueWithIdentifier:@"newSurfaces" sender:self];
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
        
        [[segue destinationViewController] openSubtree:[_rows[packetIndex] packet] root:_tree];
        [[segue destinationViewController] refreshPackets];
    } else if ([[segue identifier] isEqualToString:@"newPacket"]) {
        _newPacketPopover = [(UIStoryboardPopoverSegue*)segue popoverController];
        ((NewPacketMenu*)segue.destinationViewController).packetTree = self;
    } else {
        // This must be one of the new packet segues.
        // Pass through the parent packet.
        ((NewPacketController*)segue.destinationViewController).createBeneath = [_newPacketSpec parent];
    }
}

@end
