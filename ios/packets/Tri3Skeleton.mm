
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
#import "Tri3ViewController.h"
#import "Tri3Skeleton.h"
#import "TextHelper.h"
#import "triangulation/dim2.h"
#import "triangulation/dim3.h"

#define KEY_LAST_TRI_SKELETON_TYPE @"ViewTri3SkeletonWhich"

@interface Tri3Skeleton () <UITableViewDataSource, UITableViewDelegate> {
    CGFloat headerHeight, fatHeaderHeight;
}
@property (weak, nonatomic) IBOutlet UILabel *header;
@property (weak, nonatomic) IBOutlet UILabel *volume;
@property (weak, nonatomic) IBOutlet UILabel *solnType;
@property (weak, nonatomic) IBOutlet UIButton *lockIcon;

@property (weak, nonatomic) IBOutlet UILabel *fVector;
@property (weak, nonatomic) IBOutlet UISegmentedControl *viewWhich;

@property (weak, nonatomic) IBOutlet UIButton *vtxLinksButton;
@property (weak, nonatomic) IBOutlet UIButton *vtxLinksIcon;
@property (weak, nonatomic) IBOutlet UIButton *drillEdgeButton;
@property (weak, nonatomic) IBOutlet UIButton *drillEdgeIcon;

@property (assign, nonatomic) regina::Triangulation<3>* packet;
@end

@implementation Tri3Skeleton

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.packet = static_cast<regina::Triangulation<3>*>(static_cast<id<PacketViewer> >(self.parentViewController).packet);

    self.details.delegate = self;
    self.details.dataSource = self;

    [self reloadPacket];
}

- (void)reloadPacket
{
    if ([self.parentViewController isKindOfClass:[SnapPeaViewController class]])
        [static_cast<SnapPeaViewController*>(self.parentViewController) updateHeader:self.header volume:self.volume solnType:self.solnType];
    else
        [static_cast<Tri3ViewController*>(self.parentViewController) updateHeader:self.header lockIcon:self.lockIcon];

    self.fVector.text = [NSString stringWithFormat:@"f-vector: (%ld, %ld, %ld, %ld)",
                         self.packet->countFaces<0>(),
                         self.packet->countFaces<1>(),
                         self.packet->countFaces<2>(),
                         self.packet->size()];

    [self.viewWhich setTitle:[TextHelper countString:self.packet->countFaces<0>() singular:"vertex" plural:"vertices"] forSegmentAtIndex:0];
    [self.viewWhich setTitle:[TextHelper countString:self.packet->countFaces<1>() singular:"edge" plural:"edges"] forSegmentAtIndex:1];
    [self.viewWhich setTitle:[TextHelper countString:self.packet->countFaces<2>() singular:"triangle" plural:"triangles"] forSegmentAtIndex:2];
    [self.viewWhich setTitle:[TextHelper countString:self.packet->size() singular:"tetrahedron" plural:"tetrahedra"] forSegmentAtIndex:3];
    [self.viewWhich setTitle:[TextHelper countString:self.packet->countComponents() singular:"component" plural:"components"] forSegmentAtIndex:4];
    [self.viewWhich setTitle:[TextHelper countString:self.packet->countBoundaryComponents() singular:"boundary" plural:"boundaries"] forSegmentAtIndex:5];

    self.viewWhich.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:KEY_LAST_TRI_SKELETON_TYPE];

    [self.details reloadData];
    [self updateActions];
}

- (IBAction)whichChanged:(id)sender {
    [self.details reloadData];
    [self updateActions];
    [[NSUserDefaults standardUserDefaults] setInteger:self.viewWhich.selectedSegmentIndex forKey:KEY_LAST_TRI_SKELETON_TYPE];
}

- (void)updateActions {
    // Vertex links:
    BOOL enable = NO;
    if (self.viewWhich.selectedSegmentIndex == 0) {
        NSIndexPath* seln = self.details.indexPathForSelectedRow;
        if (seln && seln.row > 0 && seln.row <= self.packet->countVertices())
            enable = YES;
    }
    self.vtxLinksButton.enabled = self.vtxLinksIcon.enabled = enable;
    
    // Drill edge:
    enable = NO;
    if (self.viewWhich.selectedSegmentIndex == 1 && self.packet->type() == regina::PACKET_TRIANGULATION3) {
        NSIndexPath* seln = self.details.indexPathForSelectedRow;
        if (seln && seln.row > 0 && seln.row <= self.packet->countEdges())
            enable = YES;
    }
    self.drillEdgeButton.enabled = self.drillEdgeIcon.enabled = enable;
}

- (IBAction)vertexLinks:(id)sender {
    if (self.viewWhich.selectedSegmentIndex != 0)
        return;
    
    NSIndexPath* seln = self.details.indexPathForSelectedRow;
    if ((! seln) || seln.row <= 0 || seln.row > self.packet->countVertices()) {
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"Please Select a Vertex"
                                                        message:nil
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }
    
    regina::Triangulation<2>* ans = new regina::Triangulation<2>(*self.packet->vertex(seln.row - 1)->buildLink());
    ans->setLabel([NSString stringWithFormat:@"Link of vertex %zd", seln.row - 1].UTF8String);
    self.packet->insertChildLast(ans);
    [ReginaHelper viewPacket:ans];
}

- (IBAction)drillEdge:(id)sender {
    if (self.viewWhich.selectedSegmentIndex != 1)
        return;
    
    NSIndexPath* seln = self.details.indexPathForSelectedRow;
    if ((! seln) || seln.row <= 0 || seln.row > self.packet->countEdges()) {
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"Please Select an Edge"
                                                        message:nil
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }
    
    regina::Triangulation<3>* ans = new regina::Triangulation<3>(*self.packet);
    [ReginaHelper runWithHUD:@"Drilling…"
                        code:^{
                            ans->drillEdge(ans->edge(seln.row - 1));
                            ans->setLabel(self.packet->adornedLabel("Drilled #" + std::to_string(seln.row - 1)));
                        }
                     cleanup:^{
                         self.packet->insertChildLast(ans);
                         [ReginaHelper viewPacket:ans];
                     }];
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
            return 1 + MAX(self.packet->countTriangles(), 1);;
        case 3: /* tetrahedra */
            return 1 + MAX(self.packet->size(), 1);;
        case 4: /* components */
            return 1 + MAX(self.packet->countComponents(), 1);
        case 5: /* boundary components */
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
            case 3: /* tetrahedra */
                return [tableView dequeueReusableCellWithIdentifier:@"TetrahedronHeader"];
            case 4: /* components */
                return [tableView dequeueReusableCellWithIdentifier:@"ComponentHeader"];
            case 5: /* boundary components */
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
                regina::Vertex<3>* v = self.packet->vertex(indexPath.row - 1);
                cell = [tableView dequeueReusableCellWithIdentifier:@"Vertex" forIndexPath:indexPath];
                cell.index.text = [NSString stringWithFormat:@"%zd.", indexPath.row - 1];
                cell.data1.text = [NSString stringWithFormat:@"%ld", v->degree()];

                switch (v->link()) {
                    case regina::Vertex<3>::SPHERE:
                        cell.data0.text = @"Internal";
                        break;
                    case regina::Vertex<3>::DISC:
                        cell.data0.text = @"Bdry";
                        break;
                    case regina::Vertex<3>::TORUS:
                        cell.data0.text = @"Ideal: Torus";
                        break;
                    case regina::Vertex<3>::KLEIN_BOTTLE:
                        cell.data0.text = @"Ideal: Klein bottle";
                        break;
                    case regina::Vertex<3>::NON_STANDARD_CUSP:
                        if (v->isLinkOrientable())
                            cell.data0.text = [NSString stringWithFormat:@"Ideal: Genus %ld orbl", (1 - (v->linkEulerChar() / 2))];
                        else
                            cell.data0.text = [NSString stringWithFormat:@"Ideal: Genus %ld non-orbl", (2 - v->linkEulerChar())];
                        break;
                    case regina::Vertex<3>::INVALID:
                        cell.data0.attributedText = [TextHelper badString:@"Invalid"];
                        break;
                    default:
                        cell.data0.attributedText = [TextHelper badString:@"Unknown"];
                }

                NSMutableString* pieces = [NSMutableString string];
                for (auto& emb : *v)
                    [TextHelper appendToList:pieces
                                        item:[NSString stringWithFormat:@"%ld (%d)",
                                              emb.tetrahedron()->index(),
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
                regina::Edge<3>* e = self.packet->edge(indexPath.row - 1);
                cell = [tableView dequeueReusableCellWithIdentifier:@"Edge" forIndexPath:indexPath];
                cell.index.text = [NSString stringWithFormat:@"%zd.", indexPath.row - 1];
                cell.data1.text = [NSString stringWithFormat:@"%ld", e->degree()];

                if (! e->isValid())
                    cell.data0.attributedText = [TextHelper badString:@"Invalid"];
                else if (e->isBoundary())
                    cell.data0.text = @"Bdry";
                else
                    cell.data0.text = @"Internal";

                NSMutableString* pieces = [NSMutableString string];
                for (auto& emb : *e)
                    [TextHelper appendToList:pieces
                                        item:[NSString stringWithFormat:@"%ld (%s)",
                                              emb.tetrahedron()->index(),
                                              emb.vertices().trunc2().c_str()]];
                cell.data2.text = pieces;
            }
            break;
        case 2: /* triangles */
            if (self.packet->countTriangles() == 0) {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Empty" forIndexPath:indexPath];
                cell.index.text = @"No triangles";
                cell.data0.text = cell.data1.text = cell.data2.text = @"";
            } else {
                regina::Triangle<3>* t = self.packet->triangle(indexPath.row - 1);
                cell = [tableView dequeueReusableCellWithIdentifier:@"Triangle" forIndexPath:indexPath];
                cell.index.text = [NSString stringWithFormat:@"%zd.", indexPath.row - 1];

                cell.data0.text = (t->isBoundary() ? @"Bdry" : @"Internal");

                switch (t->type()) {
                    case regina::Triangle<3>::TRIANGLE:
                        cell.data1.text = @"Triangle";
                        break;
                    case regina::Triangle<3>::SCARF:
                        cell.data1.text = @"Scarf";
                        break;
                    case regina::Triangle<3>::PARACHUTE:
                        cell.data1.text = @"Parachute";
                        break;
                    case regina::Triangle<3>::MOBIUS:
                        cell.data1.text = @"Möbius band";
                        break;
                    case regina::Triangle<3>::CONE:
                        cell.data1.text = @"Cone";
                        break;
                    case regina::Triangle<3>::HORN:
                        cell.data1.text = @"Horn";
                        break;
                    case regina::Triangle<3>::DUNCEHAT:
                        cell.data1.text = @"Dunce hat";
                        break;
                    case regina::Triangle<3>::L31:
                        cell.data1.text = @"L(3,1)";
                        break;
                    case regina::Triangle<3>::UNKNOWN_TYPE:
                        cell.data1.attributedText = [TextHelper badString:@"Unknown"];
                        break;
                }

                NSMutableString* pieces = [NSMutableString string];
                for (unsigned i = 0; i < t->degree(); i++)
                    [TextHelper appendToList:pieces
                                        item:[NSString stringWithFormat:@"%ld (%s)",
                                              t->embedding(i).tetrahedron()->index(),
                                              t->embedding(i).vertices().trunc3().c_str()]];
                cell.data2.text = pieces;
            }
            break;
        case 3: /* tetrahedra */
            if (self.packet->isEmpty()) {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Empty" forIndexPath:indexPath];
                cell.index.text = @"No tetrahedra";
                cell.data0.text = cell.data1.text = cell.data2.text = @"";
            } else {
                regina::Tetrahedron<3> *t = self.packet->tetrahedron(indexPath.row - 1);
                cell = [tableView dequeueReusableCellWithIdentifier:@"Tetrahedron" forIndexPath:indexPath];
                cell.index.text = [NSString stringWithFormat:@"%zd.", indexPath.row - 1];

                cell.data0.text = [NSString stringWithFormat:@"%ld, %ld, %ld, %ld",
                                  t->vertex(0)->markedIndex(),
                                  t->vertex(1)->markedIndex(),
                                  t->vertex(2)->markedIndex(),
                                  t->vertex(3)->markedIndex()];

                cell.data1.text = [NSString stringWithFormat:@"%ld, %ld, %ld, %ld, %ld, %ld",
                                  t->edge(0)->markedIndex(),
                                  t->edge(1)->markedIndex(),
                                  t->edge(2)->markedIndex(),
                                  t->edge(3)->markedIndex(),
                                  t->edge(4)->markedIndex(),
                                  t->edge(5)->markedIndex()];

                cell.data2.text = [NSString stringWithFormat:@"%ld, %ld, %ld, %ld",
                                    t->triangle(3)->markedIndex(),
                                    t->triangle(2)->markedIndex(),
                                    t->triangle(1)->markedIndex(),
                                    t->triangle(0)->markedIndex()];
            }
            break;
        case 4: /* components */
            if (self.packet->countComponents() == 0) {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Empty" forIndexPath:indexPath];
                cell.index.text = @"No components";
                cell.data0.text = cell.data1.text = cell.data2.text = @"";
            } else {
                regina::Component<3>* c = self.packet->component(indexPath.row - 1);
                cell = [tableView dequeueReusableCellWithIdentifier:@"Component" forIndexPath:indexPath];
                cell.index.text = [NSString stringWithFormat:@"%zd.", indexPath.row - 1];
                cell.data1.text = [TextHelper countString:c->size() singular:"tetrahedron" plural:"tetrahedra"];

                cell.data0.text = [NSString stringWithFormat:@"%s %s",
                                   ((! c->isValid()) ? "Invalid, " : c->isIdeal() ? "Ideal, " : "Real, "),
                                   (c->isOrientable() ? "orbl" : "non-orbl")];

                if (self.packet->countComponents() == 1) {
                    cell.data2.text = @"All tetrahedra";
                } else {
                    NSMutableString* pieces = [NSMutableString string];
                    for (unsigned long i = 0; i < c->size(); ++i)
                        [TextHelper appendToList:pieces
                                            item:[NSString stringWithFormat:@"%ld",
                                                  c->tetrahedron(i)->index()]];
                    cell.data2.text = pieces;
                }
            }
            break;
        case 5: /* boundary components */
            if (self.packet->countBoundaryComponents() == 0) {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Empty" forIndexPath:indexPath];
                cell.index.text = @"No boundary components";
                cell.data0.text = cell.data1.text = cell.data2.text = @"";
            } else {
                regina::BoundaryComponent<3>* b = self.packet->boundaryComponent(indexPath.row - 1);
                cell = [tableView dequeueReusableCellWithIdentifier:@"Bdry" forIndexPath:indexPath];
                cell.index.text = [NSString stringWithFormat:@"%zd.", indexPath.row - 1];
                cell.data0.text = (b->isIdeal() ? @"Ideal" : @"Real");
                // Note: by parity, #triangles must be >= 2 and so we can safely use the plural form.
                cell.data1.text = (b->isIdeal() ?
                                  [NSString stringWithFormat:@"Degree %ld", b->vertex(0)->degree()] :
                                  [NSString stringWithFormat:@"%ld triangles", b->countTriangles()]);

                NSMutableString* pieces = [NSMutableString string];
                if (b->isIdeal()) {
                    regina::Vertex<3>* v = b->vertex(0);
                    for (auto& emb : *v)
                        [TextHelper appendToList:pieces
                                            item:[NSString stringWithFormat:@"%ld (%d)",
                                                  emb.tetrahedron()->index(),
                                                  emb.vertex()]];
                    cell.data2.text = [NSString stringWithFormat:@"Vertices %@", pieces];
                    // Parity says #vertices >= 2, so always use plural form.
                } else {
                    for (unsigned long i = 0; i < b->countTriangles(); ++i) {
                        const regina::TriangleEmbedding<3>& emb = b->triangle(i)->front();
                        [TextHelper appendToList:pieces
                                            item:[NSString stringWithFormat:@"%ld (%s)",
                                                  emb.tetrahedron()->index(),
                                                  emb.vertices().trunc3().c_str()]];
                    }
                    cell.data2.text = [NSString stringWithFormat:@"Faces %@", pieces];
                    // Parity says #faces >= 2, so always use plural form.
                }
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

    if (self.viewWhich.selectedSegmentIndex != 3) {
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
            UITableViewCell* cell = [self.details dequeueReusableCellWithIdentifier:@"TetrahedronHeader"];
            [cell layoutIfNeeded];
            CGSize size = [cell.contentView systemLayoutSizeFittingSize:UILayoutFittingCompressedSize];
            fatHeaderHeight = size.height;
        }
        return fatHeaderHeight;
    }
}

- (void)tableView:(UITableView *)tableView didDeselectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [self updateActions];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [self updateActions];
}

@end
