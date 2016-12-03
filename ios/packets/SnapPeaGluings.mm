
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#import "ReginaHelper.h"
#import "SnapPeaViewController.h"
#import "SnapPeaGluings.h"
#import "snappea/snappeatriangulation.h"

#pragma mark - Table cell

@interface SnapPeaGluingCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UILabel *index;
@property (weak, nonatomic) IBOutlet UILabel *face3;
@property (weak, nonatomic) IBOutlet UILabel *face2;
@property (weak, nonatomic) IBOutlet UILabel *face1;
@property (weak, nonatomic) IBOutlet UILabel *face0;
@end

@implementation SnapPeaGluingCell
@end

#pragma mark - SnapPea gluings

@interface SnapPeaGluings () <UITableViewDataSource, UITableViewDelegate> {
    CGFloat headerHeight;
}
@property (weak, nonatomic) IBOutlet UILabel *header;
@property (weak, nonatomic) IBOutlet UILabel *volume;
@property (weak, nonatomic) IBOutlet UILabel *solnType;
@property (weak, nonatomic) IBOutlet UITableView *tetrahedra;

@property (strong, nonatomic) SnapPeaViewController* viewer;
@property (assign, nonatomic) regina::SnapPeaTriangulation* packet;
@end

@implementation SnapPeaGluings

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.viewer = static_cast<SnapPeaViewController*>(self.parentViewController);
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.packet = self.viewer.packet;

    self.tetrahedra.delegate = self;
    self.tetrahedra.dataSource = self;
    
    [self reloadPacket];
}

- (void)reloadPacket
{
    [self.viewer updateHeader:self.header volume:self.volume solnType:self.solnType];
    [self.tetrahedra reloadData];
}

+ (NSString*)destStringFromFace:(int)srcFace dest:(regina::Tetrahedron<3>*)destTet gluing:(const regina::Perm<4>&)gluing
{
    if (! destTet)
        return @" ";
    else
        return [NSString stringWithFormat:@"%ld (%s)",
                destTet->markedIndex(),
                (gluing * regina::Triangle<3>::ordering(srcFace)).trunc3().c_str()];
}

- (IBAction)randomise:(id)sender {
    if (self.packet->isNull()) {
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"This is a Null Triangulation"
                                                        message:nil
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }

    if (! self.packet->isPacketEditable()) {
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"Cannot Change Gluings"
                                                        message:@"This triangulation has associated normal surfaces and/or angle structures, and so you cannot change its tetrahedron gluings."
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }
    
    self.packet->randomise();
}

- (IBAction)toRegina:(id)sender {
    if (self.packet->isNull()) {
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"This is a Null Triangulation"
                                                        message:nil
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }
    
    regina::Triangulation<3>* ans = new regina::Triangulation<3>(*self.packet);
    ans->setLabel(self.packet->label());
    self.packet->insertChildLast(ans);
    [ReginaHelper viewPacket:ans];
}

#pragma mark - Table view

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 1 + self.packet->size();
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == 0)
        return [tableView dequeueReusableCellWithIdentifier:@"Header"];
    
    SnapPeaGluingCell *cell = [tableView dequeueReusableCellWithIdentifier:@"Tetrahedron" forIndexPath:indexPath];
    regina::Tetrahedron<3>* t = self.packet->simplex(indexPath.row - 1);
    cell.index.text = [NSString stringWithFormat:@"%d.", indexPath.row - 1];
    cell.face0.text = [SnapPeaGluings destStringFromFace:0 dest:t->adjacentSimplex(0) gluing:t->adjacentGluing(0)];
    cell.face1.text = [SnapPeaGluings destStringFromFace:1 dest:t->adjacentSimplex(1) gluing:t->adjacentGluing(1)];
    cell.face2.text = [SnapPeaGluings destStringFromFace:2 dest:t->adjacentSimplex(2) gluing:t->adjacentGluing(2)];
    cell.face3.text = [SnapPeaGluings destStringFromFace:3 dest:t->adjacentSimplex(3) gluing:t->adjacentGluing(3)];
    return cell;
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    return NO;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row > 0)
        return self.tetrahedra.rowHeight;
    
    // The header row is smaller.  Calculate it based on the cell contents, which include
    // auto-layout constraints that pin the labels to the upper and lower boundaries.
    if (headerHeight == 0) {
        UITableViewCell* cell = [self.tetrahedra dequeueReusableCellWithIdentifier:@"Header"];
        [cell layoutIfNeeded];
        CGSize size = [cell.contentView systemLayoutSizeFittingSize:UILayoutFittingCompressedSize];
        headerHeight = size.height;
    }
    return headerHeight;
}

@end

