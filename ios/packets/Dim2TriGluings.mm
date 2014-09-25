
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

#import "Dim2TriGluings.h"
#import "Dim2TriangulationViewController.h"
#import "dim2/dim2triangulation.h"

#pragma mark - Table cell

@interface Dim2GluingCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UILabel *index;
@property (weak, nonatomic) IBOutlet UITextField *edge2;
@property (weak, nonatomic) IBOutlet UITextField *edge1;
@property (weak, nonatomic) IBOutlet UITextField *edge0;
@end

@implementation Dim2GluingCell
@end

#pragma mark - Dim2 gluings viewer

@interface Dim2TriGluings () <UITableViewDelegate, UITableViewDataSource> {
    CGFloat headerHeight;
}
@property (weak, nonatomic) IBOutlet UILabel *properties;
@property (weak, nonatomic) IBOutlet UITableView *triangles;

@property (strong, nonatomic) Dim2TriangulationViewController* viewer;
@property (assign, nonatomic) regina::Dim2Triangulation* packet;
@end

@implementation Dim2TriGluings

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.viewer = static_cast<Dim2TriangulationViewController*>(self.parentViewController);
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    // Do any additional setup after loading the view.

    self.packet = static_cast<regina::Dim2Triangulation*>(static_cast<id<PacketViewer> >(self.viewer).packet);

    self.properties.text = self.viewer.headerText;

    self.triangles.delegate = self;
    self.triangles.dataSource = self;
}

#pragma mark - Table view

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 1 + self.packet->getNumberOfTriangles();
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == 0)
        return [tableView dequeueReusableCellWithIdentifier:@"Header"];

    // TODO
    if (self.packet->isEmpty())
        return [tableView dequeueReusableCellWithIdentifier:@"Empty"];

    Dim2GluingCell *cell = [tableView dequeueReusableCellWithIdentifier:@"Triangle" forIndexPath:indexPath];
    cell.index.text = [NSString stringWithFormat:@"%d.", indexPath.row];
    cell.edge0.text = @"TODO";
    cell.edge1.text = @"TODO";
    cell.edge2.text = @"TODO";
    return cell;

}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    return NO;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row > 0)
        return self.triangles.rowHeight;

    // The header row is smaller.  Calculate it based on the cell contents, which include
    // auto-layout constraints that pin the labels to the upper and lower boundaries.
    if (headerHeight == 0) {
        UITableViewCell* cell = [self.triangles dequeueReusableCellWithIdentifier:@"Header"];
        [cell layoutIfNeeded];
        CGSize size = [cell.contentView systemLayoutSizeFittingSize:UILayoutFittingCompressedSize];
        headerHeight = size.height;
    }
    return headerHeight;
}

@end
