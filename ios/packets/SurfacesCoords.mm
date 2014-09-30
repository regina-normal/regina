
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

#import "Coordinates.h"
#import "SpreadHelper.h"
#import "SurfacesCoords.h"
#import "SurfacesViewController.h"
#import "MDSpreadViewClasses.h"
#import "surfaces/nnormalsurfacelist.h"
#import "triangulation/ntriangulation.h"

#define KEY_SURFACES_COORDS_COMPACT @"SurfacesCoordsCompact"

static NSString *compactCellID = @"_ReginaCompactSpreadCell";
static NSString *regularCellID = @"_ReginaRegularSpreadCell";

@interface SurfacesCoords () <MDSpreadViewDataSource, MDSpreadViewDelegate, PacketDelegate> {
    CGSize cellSize;
    CGFloat widthHeader;
    regina::NormalCoords viewCoords;
}
@property (weak, nonatomic) IBOutlet UISwitch *compact;
@property (weak, nonatomic) IBOutlet UISegmentedControl *selectCoords;
@property (weak, nonatomic) IBOutlet MDSpreadView *grid;
@end

@implementation SurfacesCoords

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];

    self.compact.on = [[NSUserDefaults standardUserDefaults] boolForKey:KEY_SURFACES_COORDS_COMPACT];

    if (self.packet->allowsAlmostNormal()) {
        [self.selectCoords setTitle:@"Quad-oct" forSegmentAtIndex:1];
    } else {
        [self.selectCoords setTitle:@"Quad" forSegmentAtIndex:1];
    }
    viewCoords = self.packet->coords();
    switch (viewCoords) {
        case regina::NS_STANDARD:
        case regina::NS_AN_STANDARD:
        case regina::NS_AN_LEGACY:
        case regina::NS_ORIENTED:
            self.selectCoords.selectedSegmentIndex = 0;
            break;
        case regina::NS_QUAD:
        case regina::NS_AN_QUAD_OCT:
        case regina::NS_ORIENTED_QUAD:
            self.selectCoords.selectedSegmentIndex = 1;
            break;
        case regina::NS_EDGE_WEIGHT:
        case regina::NS_TRIANGLE_ARCS:
        case regina::NS_ANGLE:
            // Should never occur.
            self.selectCoords.selectedSegmentIndex = 0;
            break;
    }

    [self initMetrics];
    self.grid.defaultHeaderColumnCellClass = [RegularSpreadHeaderCellRight class];
    self.grid.defaultHeaderRowCellClass = [RegularSpreadHeaderCellCentre class];
    self.grid.defaultHeaderCornerCellClass = [RegularSpreadHeaderCellCentre class];
    self.grid.dataSource = self;
    self.grid.delegate = self;
    self.grid.allowsRowHeaderSelection = YES;
}

- (void)initMetrics
{
    // TODO: Redo.
    widthHeader = [RegularSpreadHeaderCell cellSizeFor:[NSString stringWithFormat:@"%ld.", self.packet->getNumberOfSurfaces() - 1]].width;

    if (self.compact.on) {
        cellSize = [CompactSpreadViewCell cellSizeFor:@"-0"];
    } else {
        cellSize = [RegularSpreadHeaderCell cellSizeFor:
                    [Coordinates columnName:viewCoords
                                 whichCoord:0 // TODO
                                        tri:self.packet->getTriangulation()]];
    }
}

- (IBAction)compactChanged:(id)sender {
    [[NSUserDefaults standardUserDefaults] setBool:self.compact.on forKey:KEY_SURFACES_COORDS_COMPACT];

    [self initMetrics];
    [self.grid reloadData];
}

- (IBAction)coordsChanged:(id)sender {
    switch (self.selectCoords.selectedSegmentIndex) {
        case 0:
            viewCoords = (self.packet->allowsAlmostNormal() ? regina::NS_AN_STANDARD : regina::NS_STANDARD);
            break;
        case 1:
            viewCoords = (self.packet->allowsAlmostNormal() ? regina::NS_AN_QUAD_OCT : regina::NS_QUAD);
            break;
        case 2:
            viewCoords = regina::NS_EDGE_WEIGHT;
            break;
        case 3:
            viewCoords = regina::NS_FACE_ARCS;
            break;
    }

    [self initMetrics];
    [self.grid reloadData];
}

#pragma mark - MDSpreadView data source

- (NSInteger)spreadView:(MDSpreadView *)aSpreadView numberOfColumnsInSection:(NSInteger)section
{
    // TODO.
    return [Coordinates numColumns:viewCoords tri:self.packet->getTriangulation()];
}

- (NSInteger)spreadView:(MDSpreadView *)aSpreadView numberOfRowsInSection:(NSInteger)section
{
    return self.packet->getNumberOfSurfaces();
}

- (id)spreadView:(MDSpreadView *)aSpreadView titleForHeaderInRowSection:(NSInteger)section forColumnAtIndexPath:(MDIndexPath *)columnPath
{
    return (self.compact.on ? @"" :
            [Coordinates columnName:viewCoords
                         whichCoord:columnPath.column
                                tri:self.packet->getTriangulation()]);
}

- (id)spreadView:(MDSpreadView *)aSpreadView titleForHeaderInColumnSection:(NSInteger)section forRowAtIndexPath:(MDIndexPath *)rowPath
{
    return [NSString stringWithFormat:@"%ld.", (long)rowPath.row];
}

- (MDSpreadViewCell *)spreadView:(MDSpreadView *)aSpreadView cellForRowAtIndexPath:(MDIndexPath *)rowPath forColumnAtIndexPath:(MDIndexPath *)columnPath
{
    MDSpreadViewCell* cell;
    if (self.compact.on) {
        cell = [aSpreadView dequeueReusableCellWithIdentifier:compactCellID];
        if (! cell)
            cell = [[CompactSpreadViewCell alloc] initWithReuseIdentifier:compactCellID];
    } else {
        cell = [aSpreadView dequeueReusableCellWithIdentifier:regularCellID];
        if (! cell)
            cell = [[RegularSpreadViewCell alloc] initWithReuseIdentifier:regularCellID];
    }

    regina::NLargeInteger val = [Coordinates getCoordinate:viewCoords
                                                   surface:*self.packet->getSurface(rowPath.row)
                                                whichCoord:columnPath.column];
    if (val.isZero())
        cell.textLabel.text = @"";
    else if (val.isInfinite())
        cell.textLabel.text = @"∞";
    else
        cell.textLabel.text = @(val.stringValue().c_str());

    cell.textLabel.textAlignment = NSTextAlignmentRight;
    return cell;
}

#pragma mark - MDSpreadView delegate

- (CGFloat)spreadView:(MDSpreadView *)aSpreadView widthForColumnHeaderInSection:(NSInteger)columnSection
{
    return widthHeader;
}

- (CGFloat)spreadView:(MDSpreadView *)aSpreadView widthForColumnAtIndexPath:(MDIndexPath *)indexPath
{
    return cellSize.width;
}

- (CGFloat)spreadView:(MDSpreadView *)aSpreadView heightForRowHeaderInSection:(NSInteger)rowSection
{
    return (self.compact.on ? 1 : cellSize.height);
}

- (CGFloat)spreadView:(MDSpreadView *)aSpreadView heightForRowAtIndexPath:(MDIndexPath *)indexPath
{
    return cellSize.height;
}

@end
