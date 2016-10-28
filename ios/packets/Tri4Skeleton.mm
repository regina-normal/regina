
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
#import "Tri4ViewController.h"
#import "Tri4Skeleton.h"
#import "TextHelper.h"
#import "triangulation/dim3.h"
#import "triangulation/dim4.h"

#define KEY_LAST_DIM4TRI_SKELETON_TYPE @"ViewTri4SkeletonWhich"

@interface Tri4Skeleton () <UITableViewDataSource, UITableViewDelegate> {
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
@property (weak, nonatomic) IBOutlet UIButton *triangulateBdryButton;
@property (weak, nonatomic) IBOutlet UIButton *triangulateBdryIcon;

@property (assign, nonatomic) regina::Triangulation<4>* packet;
@end

@implementation Tri4Skeleton

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.packet = static_cast<regina::Triangulation<4>*>(static_cast<id<PacketViewer> >(self.parentViewController).packet);

    self.details.delegate = self;
    self.details.dataSource = self;

    [self reloadPacket];
}

- (void)reloadPacket
{
    [static_cast<Tri4ViewController*>(self.parentViewController) updateHeader:self.header lockIcon:self.lockIcon];

    self.fVector.text = [NSString stringWithFormat:@"f-vector: (%ld, %ld, %ld, %ld, %ld)",
                         self.packet->countFaces<0>(),
                         self.packet->countFaces<1>(),
                         self.packet->countFaces<2>(),
                         self.packet->countFaces<3>(),
                         self.packet->size()];

    [self.viewWhich setTitle:[TextHelper countString:self.packet->countFaces<0>() singular:"vertex" plural:"vertices"] forSegmentAtIndex:0];
    [self.viewWhich setTitle:[TextHelper countString:self.packet->countFaces<1>() singular:"edge" plural:"edges"] forSegmentAtIndex:1];
    [self.viewWhich setTitle:[TextHelper countString:self.packet->countFaces<2>() singular:"triangle" plural:"triangles"] forSegmentAtIndex:2];

    // We run out of horizontal space when some counts grow large.
    if (self.packet->countFaces<3>() < 10)
        [self.viewWhich setTitle:[TextHelper countString:self.packet->countFaces<3>() singular:"tetrahedron" plural:"tetrahedra"] forSegmentAtIndex:3];
    else
        [self.viewWhich setTitle:[NSString stringWithFormat:@"%ld tetra…", self.packet->countFaces<3>()] forSegmentAtIndex:3];

    if (self.packet->size() < 10)
        [self.viewWhich setTitle:[TextHelper countString:self.packet->size() singular:"pentachoron" plural:"pentachora"] forSegmentAtIndex:4];
    else
        [self.viewWhich setTitle:[NSString stringWithFormat:@"%ld penta…", self.packet->size()] forSegmentAtIndex:4];

    if (self.packet->countComponents() <= 1)
        [self.viewWhich setTitle:[TextHelper countString:self.packet->countComponents() singular:"component" plural:"components"] forSegmentAtIndex:5];
    else
        [self.viewWhich setTitle:[NSString stringWithFormat:@"%ld comp…", self.packet->countComponents()] forSegmentAtIndex:5];

    if (self.packet->countBoundaryComponents() < 10)
        [self.viewWhich setTitle:[TextHelper countString:self.packet->countBoundaryComponents() singular:"boundary" plural:"boundaries"] forSegmentAtIndex:6];
    else
        [self.viewWhich setTitle:[NSString stringWithFormat:@"%ld bound…", self.packet->countBoundaryComponents()] forSegmentAtIndex:6];

    self.viewWhich.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:KEY_LAST_DIM4TRI_SKELETON_TYPE];

    [self.details reloadData];
    [self updateActions];
}

- (IBAction)whichChanged:(id)sender {
    [self.details reloadData];
    [self updateActions];
    [[NSUserDefaults standardUserDefaults] setInteger:self.viewWhich.selectedSegmentIndex forKey:KEY_LAST_DIM4TRI_SKELETON_TYPE];
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
    
    // Triangulate boundary:
    enable = NO;
    if (self.viewWhich.selectedSegmentIndex == 6) {
        NSIndexPath* seln = self.details.indexPathForSelectedRow;
        if (seln && seln.row > 0 && seln.row <= self.packet->countBoundaryComponents())
            enable = YES;
    }
    self.triangulateBdryButton.enabled = self.triangulateBdryIcon.enabled = enable;
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
    
    regina::Triangulation<3>* ans = new regina::Triangulation<3>(*self.packet->vertex(seln.row - 1)->buildLink());
    ans->setLabel([NSString stringWithFormat:@"Link of vertex %zd", seln.row - 1].UTF8String);
    self.packet->insertChildLast(ans);
    [ReginaHelper viewPacket:ans];
}

- (IBAction)triangulateBdry:(id)sender {
    if (self.viewWhich.selectedSegmentIndex != 6)
        return;
    
    NSIndexPath* seln = self.details.indexPathForSelectedRow;
    if ((! seln) || seln.row <= 0 || seln.row > self.packet->countBoundaryComponents()) {
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"Please Select a Boundary Component"
                                                        message:nil
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }
    
    regina::Triangulation<3>* ans = new regina::Triangulation<3>(*self.packet->boundaryComponent(seln.row - 1)->build());
    ans->setLabel([NSString stringWithFormat:@"Boundary component %zd", seln.row - 1].UTF8String);
    self.packet->insertChildLast(ans);
    [ReginaHelper viewPacket:ans];
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
            return 1 + MAX(self.packet->countTetrahedra(), 1);;
        case 4: /* pentachora */
            return 1 + MAX(self.packet->size(), 1);;
        case 5: /* components */
            return 1 + MAX(self.packet->countComponents(), 1);
        case 6: /* boundary components */
            return 1 + MAX(self.packet->countBoundaryComponents(), 1);
        default:
            return 0;
    }
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    // TODO: Implement this!
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
            case 4: /* pentachora */
                return [tableView dequeueReusableCellWithIdentifier:@"PentachoronHeader"];
            case 5: /* components */
                return [tableView dequeueReusableCellWithIdentifier:@"ComponentHeader"];
            case 6: /* boundary components */
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
                regina::Vertex<4>* v = self.packet->vertex(indexPath.row - 1);
                cell = [tableView dequeueReusableCellWithIdentifier:@"Vertex" forIndexPath:indexPath];
                cell.index.text = [NSString stringWithFormat:@"%zd.", indexPath.row - 1];
                cell.data1.text = [NSString stringWithFormat:@"%ld", v->degree()];

                if (! v->isValid())
                    cell.data0.attributedText = [TextHelper badString:@"Invalid"];
                else if (v->isIdeal())
                    cell.data0.text = @"Ideal";
                else if (v->isBoundary())
                    cell.data0.text = @"Bdry";
                else
                    cell.data0.text = @"Internal";

                NSMutableString* pieces = [NSMutableString string];
                for (auto& emb : *v)
                    [TextHelper appendToList:pieces
                                        item:[NSString stringWithFormat:@"%ld (%d)",
                                              emb.pentachoron()->index(),
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
                regina::Edge<4>* e = self.packet->edge(indexPath.row - 1);
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
                                              emb.pentachoron()->index(),
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
                regina::Triangle<4>* t = self.packet->triangle(indexPath.row - 1);
                cell = [tableView dequeueReusableCellWithIdentifier:@"Triangle" forIndexPath:indexPath];
                cell.index.text = [NSString stringWithFormat:@"%zd.", indexPath.row - 1];
                cell.data1.text = [NSString stringWithFormat:@"%ld", t->degree()];

                if (! t->isValid())
                    cell.data0.attributedText = [TextHelper badString:@"Invalid"];
                else if (t->isBoundary())
                    cell.data0.text = @"Bdry";
                else
                    cell.data0.text = @"Internal";

                NSMutableString* pieces = [NSMutableString string];
                for (auto& emb : *t)
                    [TextHelper appendToList:pieces
                                        item:[NSString stringWithFormat:@"%ld (%s)",
                                              emb.pentachoron()->index(),
                                              emb.vertices().trunc3().c_str()]];
                cell.data2.text = pieces;
            }
            break;
        case 3: /* tetrahedra */
            if (self.packet->countTetrahedra() == 0) {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Empty" forIndexPath:indexPath];
                cell.index.text = @"No tetrahedra";
                cell.data0.text = cell.data1.text = @"";
            } else {
                regina::Tetrahedron<4>* t = self.packet->tetrahedron(indexPath.row - 1);
                cell = [tableView dequeueReusableCellWithIdentifier:@"Tetrahedron" forIndexPath:indexPath];
                cell.index.text = [NSString stringWithFormat:@"%zd.", indexPath.row - 1];

                cell.data0.text = (t->isBoundary() ? @"Bdry" : @"Internal");

                NSMutableString* pieces = [NSMutableString string];
                for (unsigned i = 0; i < t->degree(); i++)
                    [TextHelper appendToList:pieces
                                        item:[NSString stringWithFormat:@"%ld (%s)",
                                              t->embedding(i).pentachoron()->index(),
                                              t->embedding(i).vertices().trunc4().c_str()]];
                cell.data2.text = pieces;
            }
            break;
        case 4: /* pentachora */
            if (self.packet->isEmpty()) {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Empty" forIndexPath:indexPath];
                cell.index.text = @"No tetrahedra";
                cell.data0.text = cell.data1.text = cell.data2.text = cell.data3.text = @"";
            } else {
                regina::Pentachoron<4> *t = self.packet->pentachoron(indexPath.row - 1);
                cell = [tableView dequeueReusableCellWithIdentifier:@"Pentachoron" forIndexPath:indexPath];
                cell.index.text = [NSString stringWithFormat:@"%zd.", indexPath.row - 1];

                cell.data0.text = [NSString stringWithFormat:@"%ld, %ld, %ld, %ld, %ld",
                                   t->vertex(0)->markedIndex(),
                                   t->vertex(1)->markedIndex(),
                                   t->vertex(2)->markedIndex(),
                                   t->vertex(3)->markedIndex(),
                                   t->vertex(4)->markedIndex()];

                cell.data1.text = [NSString stringWithFormat:@"%ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld",
                                   t->edge(0)->markedIndex(),
                                   t->edge(1)->markedIndex(),
                                   t->edge(2)->markedIndex(),
                                   t->edge(3)->markedIndex(),
                                   t->edge(4)->markedIndex(),
                                   t->edge(5)->markedIndex(),
                                   t->edge(6)->markedIndex(),
                                   t->edge(7)->markedIndex(),
                                   t->edge(8)->markedIndex(),
                                   t->edge(9)->markedIndex()];

                cell.data2.text = [NSString stringWithFormat:@"%ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld",
                                   t->triangle(9)->markedIndex(),
                                   t->triangle(8)->markedIndex(),
                                   t->triangle(7)->markedIndex(),
                                   t->triangle(6)->markedIndex(),
                                   t->triangle(5)->markedIndex(),
                                   t->triangle(4)->markedIndex(),
                                   t->triangle(3)->markedIndex(),
                                   t->triangle(2)->markedIndex(),
                                   t->triangle(1)->markedIndex(),
                                   t->triangle(0)->markedIndex()];

                cell.data3.text = [NSString stringWithFormat:@"%ld, %ld, %ld, %ld, %ld",
                                   t->tetrahedron(4)->markedIndex(),
                                   t->tetrahedron(3)->markedIndex(),
                                   t->tetrahedron(2)->markedIndex(),
                                   t->tetrahedron(1)->markedIndex(),
                                   t->tetrahedron(0)->markedIndex()];
            }
            break;
        case 5: /* components */
            if (self.packet->countComponents() == 0) {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Empty" forIndexPath:indexPath];
                cell.index.text = @"No components";
                cell.data0.text = cell.data1.text = cell.data2.text = @"";
            } else {
                regina::Component<4>* c = self.packet->component(indexPath.row - 1);
                cell = [tableView dequeueReusableCellWithIdentifier:@"Component" forIndexPath:indexPath];
                cell.index.text = [NSString stringWithFormat:@"%zd.", indexPath.row - 1];
                cell.data1.text = [TextHelper countString:c->size() singular:"pentachoron" plural:"pentachora"];

                cell.data0.text = [NSString stringWithFormat:@"%s %s",
                                   ((! c->isValid()) ? "Invalid, " : c->isIdeal() ? "Ideal, " : "Real, "),
                                   (c->isOrientable() ? "orbl" : "non-orbl")];

                if (self.packet->countComponents() == 1) {
                    cell.data2.text = @"All pentachora";
                } else {
                    NSMutableString* pieces = [NSMutableString string];
                    for (size_t i = 0; i < c->size(); ++i)
                        [TextHelper appendToList:pieces
                                            item:[NSString stringWithFormat:@"%ld",
                                                  c->simplex(i)->index()]];
                    cell.data2.text = pieces;
                }
            }
            break;
        case 6: /* boundary components */
            if (self.packet->countBoundaryComponents() == 0) {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Empty" forIndexPath:indexPath];
                cell.index.text = @"No boundary components";
                cell.data0.text = cell.data1.text = cell.data2.text = @"";
            } else {
                regina::BoundaryComponent<4>* b = self.packet->boundaryComponent(indexPath.row - 1);
                cell = [tableView dequeueReusableCellWithIdentifier:@"Bdry" forIndexPath:indexPath];
                cell.index.text = [NSString stringWithFormat:@"%zd.", indexPath.row - 1];
                cell.data0.text = (b->isIdeal() ? @"Ideal" : b->isInvalidVertex() ? @"Invalid" : @"Real");
                cell.data1.text = ((b->isIdeal() || b->isInvalidVertex()) ?
                                   [NSString stringWithFormat:@"Degree %ld", b->vertex(0)->degree()] :
                                   b->countTetrahedra() == 1 ? @"1 tetrahedron" :
                                   [NSString stringWithFormat:@"%ld tetrahedra", b->countTetrahedra()]);

                NSMutableString* pieces = [NSMutableString string];
                if (b->isIdeal() || b->isInvalidVertex()) {
                    regina::Vertex<4>* v = b->vertex(0);
                    if (v->degree() == 1)
                        cell.data2.text = [NSString stringWithFormat:@"Vertex %ld (%d)",
                                           v->front().pentachoron()->index(),
                                           v->front().vertex()];
                    else {
                        for (auto& emb : *v)
                            [TextHelper appendToList:pieces
                                                item:[NSString stringWithFormat:@"%ld (%d)",
                                                      emb.pentachoron()->index(),
                                                      emb.vertex()]];
                        cell.data2.text = [NSString stringWithFormat:@"Vertices %@", pieces];
                    }
                } else {
                    if (b->countTetrahedra() == 1) {
                        const regina::TetrahedronEmbedding<4>& emb = b->tetrahedron(0)->front();
                        cell.data2.text = [NSString stringWithFormat:@"Facet %ld (%s)",
                                           emb.pentachoron()->index(),
                                           emb.vertices().trunc4().c_str()];
                    } else {
                        for (size_t i = 0; i < b->countTetrahedra(); ++i) {
                            const regina::TetrahedronEmbedding<4>& emb = b->tetrahedron(i)->front();
                            [TextHelper appendToList:pieces
                                                item:[NSString stringWithFormat:@"%ld (%s)",
                                                      emb.pentachoron()->index(),
                                                      emb.vertices().trunc4().c_str()]];
                        }
                        cell.data2.text = [NSString stringWithFormat:@"Facets %@", pieces];
                    }
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

    if (self.viewWhich.selectedSegmentIndex != 4) {
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
            UITableViewCell* cell = [self.details dequeueReusableCellWithIdentifier:@"PentachoronHeader"];
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
