
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

#import "Coordinates.h"
#import "SpreadHelper.h"
#import "HyperMatching.h"
#import "HyperViewController.h"
#import "MDSpreadViewClasses.h"
#import "hypersurface/normalhypersurfaces.h"
#import "maths/matrix.h"
#import "triangulation/dim4.h"

#define KEY_HYPER_MATCHING_COMPACT @"HypersurfacesMatchingCompact"

static NSString *compactCellID = @"_ReginaCompactSpreadCell";
static NSString *regularCellID = @"_ReginaRegularSpreadCell";

@interface HypersurfacesMatching () <MDSpreadViewDataSource, MDSpreadViewDelegate, PacketDelegate> {
    CGSize cellSize;
    CGFloat widthHeader;
}
@property (weak, nonatomic) IBOutlet UISwitch *compact;
@property (weak, nonatomic) IBOutlet MDSpreadView *grid;
@property (assign, nonatomic) regina::MatrixInt* matrix;
@end

@implementation HypersurfacesMatching

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];

    self.compact.on = [[NSUserDefaults standardUserDefaults] boolForKey:KEY_HYPER_MATCHING_COMPACT];

    self.grid.defaultHeaderColumnCellClass = [RegularSpreadHeaderCellRight class];
    self.grid.defaultHeaderRowCellClass = [RegularSpreadHeaderCellCentre class];
    self.grid.defaultHeaderCornerCellClass = [RegularSpreadHeaderCellCentre class];
    self.grid.dataSource = self;
    self.grid.delegate = self;
    self.grid.allowsRowHeaderSelection = YES;

    [self reloadPacket];
}

- (void)dealloc
{
    delete self.matrix;
}

- (void)reloadPacket
{
    [super reloadPacket];

    delete self.matrix;
    self.matrix = self.packet->recreateMatchingEquations();

    [self initMetrics];
    [self.grid reloadData];
}

- (void)initMetrics
{
    widthHeader = [RegularSpreadHeaderCell cellSizeFor:[NSString stringWithFormat:@"%ld.", self.matrix->rows() - 1]].width;

    if (self.compact.on) {
        cellSize = [CompactSpreadViewCell cellSizeFor:@"-0"];
    } else {
        cellSize = [RegularSpreadHeaderCell cellSizeFor:
                    [HyperCoordinates longestColumnName:self.packet->coords()
                                                    tri:self.packet->triangulation()]];
    }
}

- (IBAction)compactChanged:(id)sender {
    [[NSUserDefaults standardUserDefaults] setBool:self.compact.on forKey:KEY_HYPER_MATCHING_COMPACT];

    [self initMetrics];
    [self.grid reloadData];
}

#pragma mark - MDSpreadView data source

- (NSInteger)spreadView:(MDSpreadView *)aSpreadView numberOfColumnsInSection:(NSInteger)section
{
    return self.matrix->columns();
}

- (NSInteger)spreadView:(MDSpreadView *)aSpreadView numberOfRowsInSection:(NSInteger)section
{
    return self.matrix->rows();
}

- (id)spreadView:(MDSpreadView *)aSpreadView titleForHeaderInRowSection:(NSInteger)section forColumnAtIndexPath:(MDIndexPath *)columnPath
{
    return (self.compact.on ? @"" :
            [HyperCoordinates columnName:self.packet->coords()
                              whichCoord:columnPath.column
                                     tri:self.packet->triangulation()]);
}

- (id)spreadView:(MDSpreadView *)aSpreadView titleForHeaderInColumnSection:(NSInteger)section forRowAtIndexPath:(MDIndexPath *)rowPath
{
    return [NSString stringWithFormat:@"%d.", rowPath.row];
}

- (id)spreadView:(MDSpreadView *)aSpreadView objectValueForRowAtIndexPath:(MDIndexPath *)rowPath forColumnAtIndexPath:(MDIndexPath *)columnPath
{
    regina::Integer entry = self.matrix->entry(rowPath.row, columnPath.column);
    if (entry.isZero())
        return @"";
    else
        return @(entry.stringValue().c_str());
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
    cell.objectValue = [self spreadView:aSpreadView objectValueForRowAtIndexPath:rowPath forColumnAtIndexPath:columnPath];
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
