
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

#import "PacketTreeController.h"
#import "file/nxmlfile.h"
#import "packet/npacket.h"

@interface PacketTreeController ()

@property (assign, nonatomic) regina::NPacket* tree;
@property (assign, nonatomic) regina::NPacket* node;
@property (assign, nonatomic) bool example;
@property (strong, nonatomic) NSString* filename;

- (bool)loadTreeResource:(NSString*)filename;
- (void)configureView;

@end

@implementation PacketTreeController

- (void)viewDidLoad {
    [super viewDidLoad];
    [self configureView];
}

- (void)viewDidDisappear:(BOOL)animated {
    [super viewDidDisappear:animated];
    if (! (_node && _node->getTreeParent())) {
        delete _tree;
        NSLog(@"Closing file and deleting from memory.");
    }
}

- (void)openExample:(Example*)e {
    _example = true;
    _filename = e.desc;
    [self loadTreeResource:e.file];
    // TODO: Trap errors.
    [self configureView];
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

@end
