
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

#import "PacketManager.h"
#import "PacketTreeController.h"

#import "file/nxmlfile.h"
#import "packet/npacket.h"
#import "packet/ncontainer.h"

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

@interface PacketTreeController () {
    NSMutableArray *_rows;
}

@property (assign, nonatomic) regina::NPacket* tree;
@property (assign, nonatomic) regina::NPacket* node;
@property (assign, nonatomic) bool example;
@property (strong, nonatomic) NSString* filename;

- (bool)loadTreeResource:(NSString*)filename;
- (void)configureView;
- (void)fill;

@end

@implementation PacketTreeController

- (void)viewDidLoad {
    [super viewDidLoad];
    [self configureView];
}

- (void)viewWillDisappear:(BOOL)animated {
    if (_node && ! _node->getTreeParent()) {
        if ([self.navigationController.viewControllers indexOfObject:self] == NSNotFound) {
            delete _tree;
            NSLog(@"Closing file and deleting from memory.");
        }
    }
    [super viewDidDisappear:animated];
}

- (void)openExample:(Example*)e {
    _example = true;
    _filename = e.desc;
    [self loadTreeResource:e.file];
    // TODO: Trap errors.
}

- (void)openSubtree:(regina::NPacket *)p root:(regina::NPacket*)r {
    _example = false;
    _filename = @"Subtree";
    _tree = r;
    _node = p;
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

- (void)configureView {
    [self setTitle:_filename];
}

- (void)fill {
    if (! _node)
        return;
    
    _rows = [NSMutableArray array];
    
    regina::NPacket* p;
    for (p = _node->getFirstTreeChild(); p; p = p->getNextTreeSibling()) {
        if (p->getPacketType() == regina::NContainer::packetType) {
            [_rows addObject:[PacketTreeRow packetTreeRowWithPacket:p subtree:false]];
        } else {
            [_rows addObject:[PacketTreeRow packetTreeRowWithPacket:p subtree:false]];
            if (p->getFirstTreeChild()) {
                [_rows addObject:[PacketTreeRow packetTreeRowWithPacket:p subtree:true]];
            }
        }
    }
}

- (void)refreshPackets {
    [self fill];
    [self.tableView reloadData];
}

#pragma mark - Table View

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [_rows count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell;
    
    PacketTreeRow* r = _rows[indexPath.row];
    if (r.subtree) {
        cell = [tableView dequeueReusableCellWithIdentifier:@"Subtree" forIndexPath:indexPath];
        cell.textLabel.text = @"Browse subpackets";
    } else if ([r packet]->getPacketType() == regina::NContainer::packetType) {
        cell = [tableView dequeueReusableCellWithIdentifier:@"Container" forIndexPath:indexPath];
        cell.textLabel.text = [NSString stringWithUTF8String:[r packet]->getPacketLabel().c_str()];
        unsigned long sub = [r packet]->getNumberOfDescendants();
        if (sub == 0)
            cell.detailTextLabel.text = @"";
        else if (sub == 1)
            cell.detailTextLabel.text = @"1 subpacket";
        else
            cell.detailTextLabel.text = [NSString stringWithFormat:@"%lu subpackets", sub];
    } else {
        cell = [tableView dequeueReusableCellWithIdentifier:@"Packet" forIndexPath:indexPath];
        cell.textLabel.text = [NSString stringWithUTF8String:[r packet]->getPacketLabel().c_str()];
        unsigned long sub = [r packet]->getNumberOfDescendants();
        if (sub == 0)
            cell.detailTextLabel.text = @"";
        else if (sub == 1)
            cell.detailTextLabel.text = @"1 subpacket";
        else
            cell.detailTextLabel.text = [NSString stringWithFormat:@"%lu subpackets", sub];
        cell.imageView.image = [PacketManager iconFor:[r packet]];
    }
    return cell;
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    return NO;
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    if ([[segue identifier] isEqualToString:@"openSubtree"]) {
        NSIndexPath *indexPath = [self.tableView indexPathForSelectedRow];
        
        [[segue destinationViewController] openSubtree:[_rows[indexPath.row] packet] root:_tree];
        [[segue destinationViewController] refreshPackets];
    }
}

@end
