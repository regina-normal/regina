
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

#import "SnapPeaViewController.h"
#import "SnapPeaCusps.h"
#import "snappea/nsnappeatriangulation.h"

// TODO: Edit fillings!
// TODO: Shapes: truncate trailing 0s.

#pragma mark - Table cells

@interface SnapPeaCuspCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UILabel *cusp;
@property (weak, nonatomic) IBOutlet UILabel *vertex;
@property (weak, nonatomic) IBOutlet UILabel *filling;
@end

@implementation SnapPeaCuspCell
@end

@interface SnapPeaShapeCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UILabel *index;
@property (weak, nonatomic) IBOutlet UILabel *real;
@property (weak, nonatomic) IBOutlet UILabel *imag;
@end

@implementation SnapPeaShapeCell
@end

#pragma mark - SnapPea cusps

@interface SnapPeaCusps () <UITableViewDataSource, UITableViewDelegate> {
    CGFloat headerHeight;
}
@property (weak, nonatomic) IBOutlet UILabel *header;
@property (weak, nonatomic) IBOutlet UILabel *volume;
@property (weak, nonatomic) IBOutlet UILabel *solnType;
@property (weak, nonatomic) IBOutlet UITableView *cusps;
@property (weak, nonatomic) IBOutlet UITableView *shapes;
@property (weak, nonatomic) IBOutlet UIButton *fill;

@property (strong, nonatomic) SnapPeaViewController* viewer;
@property (assign, nonatomic) regina::NSnapPeaTriangulation* packet;
@end

@implementation SnapPeaCusps

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.viewer = static_cast<SnapPeaViewController*>(self.parentViewController);
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.packet = self.viewer.packet;
    
    self.cusps.delegate = self;
    self.cusps.dataSource = self;
    
    self.shapes.delegate = self;
    self.shapes.dataSource = self;
    
    [self reloadPacket];
}

- (void)reloadPacket
{
    [self.viewer updateHeader:self.header volume:self.volume solnType:self.solnType];

    self.fill.enabled = (self.packet->countFilledCusps() > 0);
    [self.cusps reloadData];
    [self.shapes reloadData];
}

- (void)endEditing
{
    // TODO.
}

- (IBAction)fillCusps:(id)sender
{
    // TODO.  Note: makes an edit?
}

#pragma mark - Table view

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    return NO;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == 0)
        return [tableView dequeueReusableCellWithIdentifier:@"Header"];
    
    if (tableView == self.cusps) {
        SnapPeaCuspCell *cell = [tableView dequeueReusableCellWithIdentifier:@"Cusp" forIndexPath:indexPath];
        const regina::NCusp* cusp = self.packet->cusp(indexPath.row - 1);
        cell.cusp.text = [NSString stringWithFormat:@"%d.", indexPath.row - 1];
        cell.vertex.text = [NSString stringWithFormat:@"%ld", cusp->vertex()->markedIndex()];
        if (cusp->complete())
            cell.filling.text = @"—";
        else
            cell.filling.text = [NSString stringWithFormat:@"%d, %d", cusp->m(), cusp->l()];
        return cell;
    } else {
        SnapPeaShapeCell *cell = [tableView dequeueReusableCellWithIdentifier:@"Shape" forIndexPath:indexPath];
        std::complex<double> shape = self.packet->shape(indexPath.row - 1);
        cell.index.text = [NSString stringWithFormat:@"%d.", indexPath.row - 1];
        cell.real.text = [NSString stringWithFormat:@"%lf", shape.real()];
        cell.imag.text = [NSString stringWithFormat:@"%lf", shape.imag()];
        return cell;
    }
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row > 0)
        return tableView.rowHeight;
    
    // The header row is smaller.  Calculate it based on the cell contents, which include
    // auto-layout constraints that pin the labels to the upper and lower boundaries.
    if (headerHeight == 0) {
        UITableViewCell* cell = [tableView dequeueReusableCellWithIdentifier:@"Header"];
        [cell layoutIfNeeded];
        CGSize size = [cell.contentView systemLayoutSizeFittingSize:UILayoutFittingCompressedSize];
        headerHeight = size.height;
    }
    return headerHeight;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 1 + (tableView == self.cusps ? self.packet->countCusps() : self.packet->getNumberOfTetrahedra());
}

@end
