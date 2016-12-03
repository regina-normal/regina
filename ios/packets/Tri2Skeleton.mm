
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

#import "Tri2Skeleton.h"
#import "Tri2ViewController.h"
#import "TextHelper.h"
#import "triangulation/dim2.h"

#define KEY_LAST_DIM2_SKELETON_TYPE @"ViewTri2SkeletonWhich"

@interface Tri2Skeleton () <UITableViewDataSource, UITableViewDelegate> {
    CGFloat headerHeight, fatHeaderHeight;
}
@property (weak, nonatomic) IBOutlet UILabel *summary;
@property (weak, nonatomic) IBOutlet UILabel *fVector;
@property (weak, nonatomic) IBOutlet UISegmentedControl *viewWhich;

@property (strong, nonatomic) Tri2ViewController* viewer;
@property (assign, nonatomic) regina::Triangulation<2>* packet;
@end

@implementation Tri2Skeleton

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.viewer = static_cast<Tri2ViewController*>(self.parentViewController);
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.packet = self.viewer.packet;

    self.details.delegate = self;
    self.details.dataSource = self;

    [self reloadPacket];
}

- (void)reloadPacket
{
    [self.viewer updateHeader:self.summary];

    self.fVector.text = [NSString stringWithFormat:@"f-vector: (%ld, %ld, %ld)",
                         self.packet->countFaces<0>(),
                         self.packet->countFaces<1>(),
                         self.packet->size()];

    [self.viewWhich setTitle:[TextHelper countString:self.packet->countFaces<0>() singular:"vertex" plural:"vertices"] forSegmentAtIndex:0];
    [self.viewWhich setTitle:[TextHelper countString:self.packet->countFaces<1>() singular:"edge" plural:"edges"] forSegmentAtIndex:1];
    [self.viewWhich setTitle:[TextHelper countString:self.packet->size() singular:"triangle" plural:"triangles"] forSegmentAtIndex:2];
    [self.viewWhich setTitle:[TextHelper countString:self.packet->countComponents() singular:"component" plural:"components"] forSegmentAtIndex:3];
    [self.viewWhich setTitle:[TextHelper countString:self.packet->countBoundaryComponents() singular:"boundary" plural:"boundaries"] forSegmentAtIndex:4];

    self.viewWhich.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:KEY_LAST_DIM2_SKELETON_TYPE];

    [self.details reloadData];
}

- (IBAction)whichChanged:(id)sender {
    [self.details reloadData];
    [[NSUserDefaults standardUserDefaults] setInteger:self.viewWhich.selectedSegmentIndex forKey:KEY_LAST_DIM2_SKELETON_TYPE];
}

#pragma mark - Table view

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    switch (self.viewWhich.selectedSegmentIndex) {
        case 0: /* vertices */
            return 1 + MAX(self.packet->countVertices(), 1);
        case 1: /* edges */
            return 1 + MAX(self.packet->countEdges(), 1);
        case 2: /* triangles */
            return 1 + MAX(self.packet->size(), 1);;
        case 3: /* components */
            return 1 + MAX(self.packet->countComponents(), 1);
        case 4: /* boundary components */
            return 1 + MAX(self.packet->countBoundaryComponents(), 1);
        default:
            return 0;
    }
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == 0) {
        switch (self.viewWhich.selectedSegmentIndex) {
            case 0: /* vertices */
                return [tableView dequeueReusableCellWithIdentifier:@"VertexHeader"];
            case 1: /* edges */
                return [tableView dequeueReusableCellWithIdentifier:@"EdgeHeader"];
            case 2: /* triangles */
                return [tableView dequeueReusableCellWithIdentifier:@"TriangleHeader"];
            case 3: /* components */
                return [tableView dequeueReusableCellWithIdentifier:@"ComponentHeader"];
            case 4: /* boundary components */
                return [tableView dequeueReusableCellWithIdentifier:@"BdryHeader"];
            default:
                return nil;
        }
    }

    SkeletonCell *cell;
    switch (self.viewWhich.selectedSegmentIndex) {
        case 0: /* vertices */
            if (self.packet->countVertices() == 0) {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Empty" forIndexPath:indexPath];
                cell.index.text = @"No vertices";
                cell.data0.text = cell.data1.text = cell.data2.text = @"";
            } else {
                regina::Vertex<2>* v = self.packet->vertex(indexPath.row - 1);
                cell = [tableView dequeueReusableCellWithIdentifier:@"Vertex" forIndexPath:indexPath];
                cell.index.text = [NSString stringWithFormat:@"%d.", indexPath.row - 1];
                cell.data0.text = (v->isBoundary() ? @"Bdry" : @"Internal");
                cell.data1.text = [NSString stringWithFormat:@"%ld", v->degree()];

                NSMutableString* pieces = [NSMutableString string];
                for (auto& emb : *v)
                    [TextHelper appendToList:pieces
                                        item:[NSString stringWithFormat:@"%ld (%d)",
                                              emb.triangle()->index(),
                                              emb.vertex()]];
                cell.data2.text = pieces;
            }
            break;
        case 1: /* edges */
            if (self.packet->countEdges() == 0) {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Empty" forIndexPath:indexPath];
                cell.index.text = @"No edges";
                cell.data0.text = cell.data1.text = cell.data2.text = @"";
            } else {
                regina::Edge<2>* e = self.packet->edge(indexPath.row - 1);
                cell = [tableView dequeueReusableCellWithIdentifier:@"Edge" forIndexPath:indexPath];
                cell.index.text = [NSString stringWithFormat:@"%d.", indexPath.row - 1];
                cell.data0.text = (e->isBoundary() ? @"Bdry" : @"Internal");
                cell.data1.text = [NSString stringWithFormat:@"%zu", e->degree()];

                NSMutableString* pieces = [NSMutableString string];
                for (unsigned i = 0; i < e->degree(); i++)
                    [TextHelper appendToList:pieces
                                        item:[NSString stringWithFormat:@"%ld (%s)",
                                              e->embedding(i).triangle()->index(),
                                              e->embedding(i).vertices().trunc2().c_str()]];
                cell.data2.text = pieces;
            }
            break;
        case 2: /* triangles */
            if (self.packet->isEmpty()) {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Empty" forIndexPath:indexPath];
                cell.index.text = @"No triangles";
                cell.data0.text = cell.data1.text = @"";
            } else {
                regina::Triangle<2> *t = self.packet->triangle(indexPath.row - 1);
                cell = [tableView dequeueReusableCellWithIdentifier:@"Triangle" forIndexPath:indexPath];
                cell.index.text = [NSString stringWithFormat:@"%d.", indexPath.row - 1];

                cell.data0.text = [NSString stringWithFormat:@"%ld, %ld, %ld",
                                  t->vertex(0)->markedIndex(),
                                  t->vertex(1)->markedIndex(),
                                  t->vertex(2)->markedIndex()];

                cell.data1.text = [NSString stringWithFormat:@"%ld, %ld, %ld",
                                   t->edge(2)->markedIndex(),
                                   t->edge(1)->markedIndex(),
                                   t->edge(0)->markedIndex()];
            }
            break;
        case 3: /* components */
            if (self.packet->countComponents() == 0) {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Empty" forIndexPath:indexPath];
                cell.index.text = @"No components";
                cell.data0.text = cell.data1.text = cell.data2.text = @"";
            } else {
                regina::Component<2>* c = self.packet->component(indexPath.row - 1);
                cell = [tableView dequeueReusableCellWithIdentifier:@"Component" forIndexPath:indexPath];
                cell.index.text = [NSString stringWithFormat:@"%d.", indexPath.row - 1];
                cell.data0.text = (c->isOrientable() ? @"Orbl" : @"Non-orbl");
                cell.data1.text = [TextHelper countString:c->size() singular:"triangle" plural:"triangles"];
                if (self.packet->countComponents() == 1) {
                    cell.data2.text = @"All triangles";
                } else {
                    NSMutableString* pieces = [NSMutableString string];
                    for (unsigned long i = 0; i < c->size(); ++i)
                        [TextHelper appendToList:pieces
                                            item:[NSString stringWithFormat:@"%ld",
                                                  c->triangle(i)->index()]];
                    cell.data2.text = pieces;
                }
            }
            break;
        case 4: /* boundary components */
            if (self.packet->countBoundaryComponents() == 0) {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Empty" forIndexPath:indexPath];
                cell.index.text = @"No boundary components";
                cell.data0.text = cell.data1.text = @"";
            } else {
                regina::BoundaryComponent<2>* b = self.packet->boundaryComponent(indexPath.row - 1);
                cell = [tableView dequeueReusableCellWithIdentifier:@"Bdry" forIndexPath:indexPath];
                cell.index.text = [NSString stringWithFormat:@"%d.", indexPath.row - 1];
                cell.data0.text = [TextHelper countString:b->countEdges() singular:"edge" plural:"edges"];

                NSMutableString* pieces = [NSMutableString string];
                for (unsigned long i = 0; i < b->countEdges(); ++i) {
                    const regina::EdgeEmbedding<2>& emb = b->edge(i)->front();
                    [TextHelper appendToList:pieces
                                        item:[NSString stringWithFormat:@"%ld (%s)",
                                              emb.triangle()->index(),
                                              emb.vertices().trunc2().c_str()]];
                }
                cell.data1.text = pieces;
            }
            break;
    }
    return cell;
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    return NO;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row > 0)
        return self.details.rowHeight;

    if (self.viewWhich.selectedSegmentIndex != 2) {
        // The header row is smaller.  Calculate it based on the cell contents, which include
        // auto-layout constraints that pin the labels to the upper and lower boundaries.
        if (headerHeight == 0) {
            UITableViewCell* cell = [self.details dequeueReusableCellWithIdentifier:@"EdgeHeader"];
            [cell layoutIfNeeded];
            CGSize size = [cell.contentView systemLayoutSizeFittingSize:UILayoutFittingCompressedSize];
            headerHeight = size.height;
        }
        return headerHeight;
    } else {
        // Same deal for a two-line header.
        if (fatHeaderHeight == 0) {
            UITableViewCell* cell = [self.details dequeueReusableCellWithIdentifier:@"TriangleHeader"];
            [cell layoutIfNeeded];
            CGSize size = [cell.contentView systemLayoutSizeFittingSize:UILayoutFittingCompressedSize];
            fatHeaderHeight = size.height;
        }
        return fatHeaderHeight;
    }
}

@end
