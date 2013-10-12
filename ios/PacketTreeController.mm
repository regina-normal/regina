//
//  PacketTreeController.m
//  Regina
//
//  Created by Ben Burton on 12/10/13.
//  Copyright (c) 2013 Regina Development Team. All rights reserved.
//

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
