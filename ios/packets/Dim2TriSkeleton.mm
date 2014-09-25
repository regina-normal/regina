
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

#import "Dim2TriSkeleton.h"
#import "Dim2TriangulationViewController.h"
#import "dim2/dim2triangulation.h"

@interface Dim2TriSkeleton ()
@property (weak, nonatomic) IBOutlet UILabel *summary;
@property (weak, nonatomic) IBOutlet UILabel *vertexCount;
@property (weak, nonatomic) IBOutlet UILabel *edgeCount;
@property (weak, nonatomic) IBOutlet UILabel *triangleCount;
@property (weak, nonatomic) IBOutlet UILabel *componentCount;
@property (weak, nonatomic) IBOutlet UILabel *bdryCount;
@property (weak, nonatomic) IBOutlet UISegmentedControl *viewWhich;
@property (weak, nonatomic) IBOutlet UITableView *details;

@property (strong, nonatomic) Dim2TriangulationViewController* viewer;
@property (assign, nonatomic) regina::Dim2Triangulation* packet;
@end

@implementation Dim2TriSkeleton

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.viewer = static_cast<Dim2TriangulationViewController*>(self.parentViewController);
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];

    self.packet = static_cast<regina::Dim2Triangulation*>(static_cast<id<PacketViewer> >(self.viewer).packet);

    self.summary.text = self.viewer.headerText;

    self.vertexCount.text = [NSString stringWithFormat:@"%ld", self.packet->getNumberOfFaces<0>()];
    self.edgeCount.text = [NSString stringWithFormat:@"%ld", self.packet->getNumberOfFaces<1>()];
    self.triangleCount.text = [NSString stringWithFormat:@"%ld", self.packet->getNumberOfFaces<2>()];
    self.componentCount.text = [NSString stringWithFormat:@"%ld", self.packet->getNumberOfComponents()];
    self.bdryCount.text = [NSString stringWithFormat:@"%ld", self.packet->getNumberOfBoundaryComponents()];
}

@end
