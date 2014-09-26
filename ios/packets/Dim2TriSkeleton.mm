
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

#define KEY_LAST_DIM2_SKELETON_TYPE @"ViewDim2SkeletonWhich"

// TODO: Allow cells to spread over multiple lines.

#pragma mark - Table cell

@interface Dim2SkeletonCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UILabel *index;
@property (weak, nonatomic) IBOutlet UILabel *type;
@property (weak, nonatomic) IBOutlet UILabel *size;
@property (weak, nonatomic) IBOutlet UILabel *pieces;
@end

@implementation Dim2SkeletonCell
@end

#pragma mark - Dim2Triangulation skeleton viewer

@interface Dim2TriSkeleton () <UITableViewDataSource, UITableViewDelegate> {
    CGFloat headerHeight;
}
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
    self.packet = self.viewer.packet;

    [self.viewer updateHeader:self.summary];

    self.vertexCount.text = [NSString stringWithFormat:@"%ld", self.packet->getNumberOfFaces<0>()];
    self.edgeCount.text = [NSString stringWithFormat:@"%ld", self.packet->getNumberOfFaces<1>()];
    self.triangleCount.text = [NSString stringWithFormat:@"%ld", self.packet->getNumberOfFaces<2>()];
    self.componentCount.text = [NSString stringWithFormat:@"%ld", self.packet->getNumberOfComponents()];
    self.bdryCount.text = [NSString stringWithFormat:@"%ld", self.packet->getNumberOfBoundaryComponents()];

    self.viewWhich.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:KEY_LAST_DIM2_SKELETON_TYPE];

    self.details.delegate = self;
    self.details.dataSource = self;
}

- (IBAction)whichChanged:(id)sender {
    [self.details reloadData];
    [[NSUserDefaults standardUserDefaults] setInteger:self.viewWhich.selectedSegmentIndex forKey:KEY_LAST_DIM2_SKELETON_TYPE];
}

+ (void)appendToList:(NSMutableString*)str item:(NSString*)item
{
    if (str.length == 0)
        [str appendString:item];
    else
        [str appendFormat:@", %@", item];
}

#pragma mark - Table view

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    switch (self.viewWhich.selectedSegmentIndex) {
        case 0: /* vertices */
            return 1 + MAX(self.packet->getNumberOfVertices(), 1);
        case 1: /* edges */
            return 1 + MAX(self.packet->getNumberOfEdges(), 1);
        case 2: /* components */
            return 1 + MAX(self.packet->getNumberOfComponents(), 1);
        case 3: /* boundary components */
            return 1 + MAX(self.packet->getNumberOfBoundaryComponents(), 1);
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
            case 2: /* components */
                return [tableView dequeueReusableCellWithIdentifier:@"ComponentHeader"];
            case 3: /* boundary components */
                return [tableView dequeueReusableCellWithIdentifier:@"BdryHeader"];
            default:
                return nil;
        }
    }

    Dim2SkeletonCell *cell;
    switch (self.viewWhich.selectedSegmentIndex) {
        case 0: /* vertices */
            if (self.packet->getNumberOfVertices() == 0) {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Empty" forIndexPath:indexPath];
                cell.index.text = @"No vertices";
                cell.type.text = cell.size.text = cell.pieces.text = @"";
            } else {
                regina::Dim2Vertex* v = self.packet->getVertex(indexPath.row - 1);
                cell = [tableView dequeueReusableCellWithIdentifier:@"Vertex" forIndexPath:indexPath];
                cell.index.text = [NSString stringWithFormat:@"%d.", indexPath.row - 1];
                cell.size.text = [NSString stringWithFormat:@"%ld", v->getDegree()];
                cell.type.text = (v->isBoundary() ? @"Bdry" : @"Internal");

                NSMutableString* pieces = [NSMutableString string];
                std::deque<regina::Dim2VertexEmbedding>::const_iterator it;
                for (it = v->getEmbeddings().begin(); it != v->getEmbeddings().end(); it++)
                    [Dim2TriSkeleton appendToList:pieces
                                             item:[NSString stringWithFormat:@"%ld (%d)",
                                                   self.packet->triangleIndex((*it).getTriangle()),
                                                   (*it).getVertex()]];
                cell.pieces.text = pieces;
            }
            break;
        case 1: /* edges */
            if (self.packet->getNumberOfEdges() == 0) {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Empty" forIndexPath:indexPath];
                cell.index.text = @"No edges";
                cell.type.text = cell.size.text = cell.pieces.text = @"";
            } else {
                regina::Dim2Edge* e = self.packet->getEdge(indexPath.row - 1);
                cell = [tableView dequeueReusableCellWithIdentifier:@"Edge" forIndexPath:indexPath];
                cell.index.text = [NSString stringWithFormat:@"%d.", indexPath.row - 1];
                cell.size.text = [NSString stringWithFormat:@"%d", e->getNumberOfEmbeddings()];
                cell.type.text = (e->isBoundary() ? @"Bdry" : @"Internal");

                NSMutableString* pieces = [NSMutableString string];
                for (unsigned i = 0; i < e->getNumberOfEmbeddings(); i++)
                    [Dim2TriSkeleton appendToList:pieces
                                             item:[NSString stringWithFormat:@"%ld (%s)",
                                                   self.packet->triangleIndex(e->getEmbedding(i).getTriangle()),
                                                   e->getEmbedding(i).getVertices().trunc2().c_str()]];
                cell.pieces.text = pieces;
            }
            break;
        case 2: /* components */
            if (self.packet->getNumberOfComponents() == 0) {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Empty" forIndexPath:indexPath];
                cell.index.text = @"No components";
                cell.type.text = cell.size.text = cell.pieces.text = @"";
            } else {
                regina::Dim2Component* c = self.packet->getComponent(indexPath.row - 1);
                cell = [tableView dequeueReusableCellWithIdentifier:@"Component" forIndexPath:indexPath];
                cell.index.text = [NSString stringWithFormat:@"%d.", indexPath.row - 1];
                cell.size.text = [NSString stringWithFormat:@"%ld", c->getNumberOfTriangles()];
                cell.type.text = (c->isOrientable() ? @"Orbl" : @"Non-orbl");

                NSMutableString* pieces = [NSMutableString string];
                for (unsigned long i = 0; i < c->getNumberOfTriangles(); ++i)
                    [Dim2TriSkeleton appendToList:pieces
                                             item:[NSString stringWithFormat:@"%ld",
                                                   self.packet->triangleIndex(c->getTriangle(i))]];
                cell.pieces.text = pieces;
            }
            break;
        case 3: /* boundary components */
            if (self.packet->getNumberOfBoundaryComponents() == 0) {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Empty" forIndexPath:indexPath];
                cell.index.text = @"No boundary components";
                cell.type.text = cell.size.text = cell.pieces.text = @"";
            } else {
                regina::Dim2BoundaryComponent* b = self.packet->getBoundaryComponent(indexPath.row - 1);
                cell = [tableView dequeueReusableCellWithIdentifier:@"Bdry" forIndexPath:indexPath];
                cell.index.text = [NSString stringWithFormat:@"%d.", indexPath.row - 1];
                cell.size.text = (b->getNumberOfEdges() == 1 ?
                                  @"1 edge" :
                                  [NSString stringWithFormat:@"%ld edges", b->getNumberOfEdges()]);

                NSMutableString* pieces = [NSMutableString string];
                for (unsigned long i = 0; i < b->getNumberOfEdges(); ++i) {
                    const regina::Dim2EdgeEmbedding& emb = b->getEdge(i)->getEmbedding(0);
                    [Dim2TriSkeleton appendToList:pieces
                                             item:[NSString stringWithFormat:@"%ld (%s)",
                                                   self.packet->triangleIndex(emb.getTriangle()),
                                                   emb.getVertices().trunc2().c_str()]];
                }
                cell.pieces.text = pieces;
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

    // The header row is smaller.  Calculate it based on the cell contents, which include
    // auto-layout constraints that pin the labels to the upper and lower boundaries.
    if (headerHeight == 0) {
        UITableViewCell* cell = [self.details dequeueReusableCellWithIdentifier:@"EdgeHeader"];
        [cell layoutIfNeeded];
        CGSize size = [cell.contentView systemLayoutSizeFittingSize:UILayoutFittingCompressedSize];
        headerHeight = size.height;
    }
    return headerHeight;
}

@end
