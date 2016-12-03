
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
#import "ReginaHelper.h"
#import "SpreadHelper.h"
#import "HyperCoords.h"
#import "HyperViewController.h"
#import "TextHelper.h"
#import "MDSpreadViewClasses.h"
#import "hypersurface/normalhypersurfaces.h"
#import "triangulation/dim3.h"
#import "triangulation/dim4.h"

#define KEY_HYPER_COORDS_COMPACT @"HypersurfacesCoordsCompact"

static NSString *compactCellID = @"_ReginaCompactCell";
static NSString *regularCellID = @"_ReginaRegularCell";
static NSString *headerCellID = @"_ReginaHeaderCell";

static UIColor *headerBlack = [UIColor blackColor];
static UIColor *headerBdry = [UIColor colorWithRed:(0xB8 / 256.0) green:(0x86 / 256.0) blue:(0x0B / 256.0) alpha:1.0];

#define PROP_NONE 0
#define PROP_NAME 1
#define PROP_HOMOLOGY 2
#define PROP_ORBL 3
#define PROP_SIDES 4
#define PROP_BDRY 5
#define PROP_LINK 6

static NSArray* embProps = @[@PROP_HOMOLOGY, @PROP_ORBL, @PROP_SIDES, @PROP_BDRY, @PROP_LINK];
static NSArray* nonEmbProps = @[@PROP_BDRY, @PROP_LINK];

@interface HypersurfacesCoords () <MDSpreadViewDataSource, MDSpreadViewDelegate, PacketDelegate> {
    NSArray* widthProps;
    CGFloat widthCoord;
    CGFloat height;
    CGFloat widthHeader;
    regina::HyperCoords viewCoords;
    long selectedRow; // 0 for no selection, or (surface index + 1) for a surface.
}
@property (weak, nonatomic) IBOutlet UISwitch *compact;
@property (weak, nonatomic) IBOutlet UISegmentedControl *selectCoords;
@property (weak, nonatomic) IBOutlet MDSpreadView *grid;
@property (weak, nonatomic) IBOutlet UIButton *triangulateButton;
@end

@implementation HypersurfacesCoords

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];

    self.compact.on = [[NSUserDefaults standardUserDefaults] boolForKey:KEY_HYPER_COORDS_COMPACT];

    self.grid.defaultHeaderColumnCellClass = [RegularSpreadHeaderCellRight class];
    self.grid.defaultHeaderCornerCellClass = [RegularSpreadHeaderCellCentre class];
    // The column headers are managed manually, so that we can colour them if required.
    self.grid.dataSource = self;
    self.grid.delegate = self;
    self.grid.selectionMode = self.grid.highlightMode = MDSpreadViewSelectionModeRow;
    self.grid.allowsRowHeaderSelection = YES;

    [self reloadPacket];
}

- (void)reloadPacket
{
    [super reloadPacket];

    viewCoords = self.packet->coords();
    switch (viewCoords) {
        case regina::HS_STANDARD:
            self.selectCoords.selectedSegmentIndex = 0;
            break;
        case regina::HS_PRISM:
            self.selectCoords.selectedSegmentIndex = 1;
            break;
        case regina::HS_EDGE_WEIGHT:
            // Should never occur.
            self.selectCoords.selectedSegmentIndex = 0;
            break;
    }

    [self initMetrics];
    [self.grid reloadData];
    [self refreshActions];
}

- (void)initMetrics
{
    NSMutableArray* w = [[NSMutableArray alloc] init];

    CGFloat tmp;
    for (NSNumber* prop in (self.packet->isEmbeddedOnly() ? embProps : nonEmbProps)) {
        switch (prop.intValue) {
            case PROP_NAME:
                tmp = [RegularSpreadViewCell cellSizeFor:@"Name"].width;
                break;
            case PROP_HOMOLOGY:
                tmp = [RegularSpreadViewCell cellSizeFor:@"0 Z₉₉ + 0 Z₉₉"].width;
                break;
            case PROP_ORBL:
                tmp = [RegularSpreadViewCell cellSizeFor:(self.compact.on ? @"✓" : @"Non-or.")].width;
                break;
            case PROP_SIDES:
                tmp = (self.compact.on ? [RegularSpreadViewCell cellSizeFor:@"2"] : [RegularSpreadHeaderCell cellSizeFor:@"Sides"]).width;
                break;
            case PROP_BDRY:
                if (self.packet->triangulation()->isClosed())
                    tmp = (self.compact.on ? [RegularSpreadViewCell cellSizeFor:@"—"] : [RegularSpreadHeaderCell cellSizeFor:@"Bdry"]).width;
                else // TODO: fix this when we support prism coordinates: if (! self.packet->allowsSpun())
                    tmp = [RegularSpreadViewCell cellSizeFor:@"Real"].width;
                // else
                //    tmp = [RegularSpreadViewCell cellSizeFor:@"Non-compact"].width;
                break;
            case PROP_LINK:
                tmp = [RegularSpreadViewCell cellSizeFor:@"Vertex 99"].width;
                break;
        }
        [w addObject:[NSNumber numberWithFloat:tmp]];
    }

    widthProps = w;

    widthHeader = [RegularSpreadHeaderCell cellSizeFor:[NSString stringWithFormat:@"%ld.", self.packet->size() - 1]].width;

    if (self.compact.on) {
        CGSize s = [CompactSpreadViewCell cellSizeFor:@"g00"];
        widthCoord = s.width;
        height = s.height;
    } else {
        NSString* longestColumnName = [HyperCoordinates longestColumnName:viewCoords tri:self.packet->triangulation()];
        if (longestColumnName.length < 2)
            longestColumnName = @"99"; // Ah, edge weight space.
        widthCoord = [RegularSpreadHeaderCell cellSizeFor:longestColumnName].width;
        height = [RegularSpreadViewCell cellSizeFor:@"g0"].height;
    }
}

- (IBAction)compactChanged:(id)sender {
    [[NSUserDefaults standardUserDefaults] setBool:self.compact.on forKey:KEY_HYPER_COORDS_COMPACT];

    [self initMetrics];
    [self.grid reloadData];
}

- (IBAction)coordsChanged:(id)sender {
    switch (self.selectCoords.selectedSegmentIndex) {
        case 0:
            viewCoords = regina::HS_STANDARD;
            break;
        case 1:
            viewCoords = regina::HS_PRISM;
            break;
        case 2:
            viewCoords = regina::HS_EDGE_WEIGHT;
            break;
    }

    [self initMetrics];
    [self.grid reloadData];
}

- (IBAction)triangulate:(id)sender {
    if (selectedRow == 0 || selectedRow > self.packet->size()) {
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"Please Select a Hypersurface"
                                                        message:nil
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }
    
    const regina::NormalHypersurface* s = self.packet->hypersurface(selectedRow - 1);
    if (! s->isCompact()) {
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"Hypersurface Not Compact"
                                                        message:@"I can only triangulate compact hypersurfaces, not spun-normal hypersurfaces."
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }
    if (! s->embedded()) {
        UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"Hypersurface Not Embedded"
                                                        message:@"I can only triangulate embedded hypersurfaces, not immersed and/or branched hypersurfaces."
                                                       delegate:nil
                                              cancelButtonTitle:@"Close"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }
    
    regina::Triangulation<3>* ans = s->triangulate();
    ans->intelligentSimplify();
    ans->setLabel(self.packet->triangulation()->adornedLabel("Hypersurface #" + std::to_string(selectedRow - 1)));
    self.packet->insertChildLast(ans);
    [ReginaHelper viewPacket:ans];
}

- (void)refreshActions
{
    self.triangulateButton.enabled = (selectedRow > 0 && selectedRow <= self.packet->size());
}

#pragma mark - MDSpreadView data source

- (NSInteger)spreadView:(MDSpreadView *)aSpreadView numberOfColumnsInSection:(NSInteger)section
{
    return (self.packet->isEmbeddedOnly() ? embProps.count : nonEmbProps.count) +
           [HyperCoordinates numColumns:viewCoords tri:self.packet->triangulation()];
}

- (NSInteger)spreadView:(MDSpreadView *)aSpreadView numberOfRowsInSection:(NSInteger)section
{
    return self.packet->size();
}

- (id)spreadView:(MDSpreadView *)aSpreadView titleForHeaderInColumnSection:(NSInteger)section forRowAtIndexPath:(MDIndexPath *)rowPath
{
    return [NSString stringWithFormat:@"%ld.", (long)rowPath.row];
}

- (MDSpreadViewCell *)spreadView:(MDSpreadView *)aSpreadView cellForHeaderInRowSection:(NSInteger)section forColumnAtIndexPath:(MDIndexPath *)columnPath
{
    MDSpreadViewCell* cell = [aSpreadView dequeueReusableCellWithIdentifier:headerCellID];
    if (! cell)
        cell = [[RegularSpreadHeaderCellCentre alloc] initWithStyle:MDSpreadViewHeaderCellStyleRow reuseIdentifier:headerCellID];

    cell.textLabel.textColor = headerBlack;

    if (self.compact.on) {
        cell.textLabel.text = @"";
        return cell;
    }

    int prop = PROP_NONE;
    int coord = columnPath.column;

    if (self.packet->isEmbeddedOnly()) {
        if (coord < embProps.count)
            prop = [embProps[coord] intValue];
        else
            coord -= embProps.count;
    } else {
        if (coord < nonEmbProps.count)
            prop = [nonEmbProps[coord] intValue];
        else
            coord -= nonEmbProps.count;
    }

    switch (prop) {
        case PROP_NONE: break;
        case PROP_NAME: cell.textLabel.text = @"Name"; return cell;
        case PROP_HOMOLOGY: cell.textLabel.text = @"Homology"; return cell;
        case PROP_ORBL: cell.textLabel.text = @"Orient"; return cell;
        case PROP_SIDES: cell.textLabel.text = @"Sides"; return cell;
        case PROP_BDRY: cell.textLabel.text = @"Bdry"; return cell;
        case PROP_LINK: cell.textLabel.text = @"Link"; return cell;
    }

    cell.textLabel.text = [HyperCoordinates columnName:viewCoords
                                            whichCoord:coord
                                                   tri:self.packet->triangulation()];

    if (viewCoords == regina::HS_EDGE_WEIGHT && self.packet->triangulation()->edge(coord)->isBoundary())
        cell.textLabel.textColor = headerBdry;

    return cell;
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

    const regina::NormalHypersurface* s = self.packet->hypersurface(rowPath.row);

    int prop = PROP_NONE;
    int coord = columnPath.column;

    if (self.packet->isEmbeddedOnly()) {
        if (coord < embProps.count)
            prop = [embProps[coord] intValue];
        else
            coord -= embProps.count;
    } else {
        if (coord < nonEmbProps.count)
            prop = [nonEmbProps[coord] intValue];
        else
            coord -= nonEmbProps.count;
    }

    switch (prop) {
        case PROP_NONE:
            break;
        case PROP_NAME:
            cell.textLabel.text = @(s->name().c_str());
            cell.textLabel.textAlignment = NSTextAlignmentLeft;
            return cell;
        case PROP_HOMOLOGY:
            if (s->isCompact())
                cell.textLabel.text = @(s->homology().utf8().c_str());
            else
                cell.textLabel.text = @"";
            cell.textLabel.textAlignment = NSTextAlignmentLeft;
            return cell;
        case PROP_ORBL:
            if (s->isCompact())
                cell.textLabel.attributedText = [TextHelper yesNoString:s->isOrientable()
                                                                    yes:@"✓"
                                                                     no:(self.compact.on ? @"✕" : @"Non-or.")];
            else
                cell.textLabel.text = @"";
            cell.textLabel.textAlignment = NSTextAlignmentCenter;
            return cell;
        case PROP_SIDES:
            if (s->isCompact())
                cell.textLabel.attributedText = [TextHelper yesNoString:s->isTwoSided() yes:@"2" no:@"1"];
            else
                cell.textLabel.text = @"";
            cell.textLabel.textAlignment = NSTextAlignmentCenter;
            return cell;
        case PROP_BDRY:
            if (! s->isCompact())
                cell.textLabel.attributedText = [TextHelper markedString:@"Non-compact"];
            else if (s->hasRealBoundary())
                cell.textLabel.attributedText = [TextHelper yesNoString:@"Real" yesNo:NO];
            else
                cell.textLabel.attributedText = [TextHelper yesNoString:@"—" yesNo:YES];
            cell.textLabel.textAlignment = NSTextAlignmentLeft;
            return cell;
        case PROP_LINK:
        {
            const regina::Vertex<4>* v;
            const regina::Edge<4>* e;

            if ((v = s->isVertexLink()))
                cell.textLabel.text = [NSString stringWithFormat:@"Vertex %ld",
                                       v->index()];
            else if ((e = s->isThinEdgeLink()))
                cell.textLabel.text = [NSString stringWithFormat:@"Edge %ld",
                                       e->index()];
            else
                cell.textLabel.text = @"";
            cell.textLabel.textAlignment = NSTextAlignmentLeft;
            return cell;
        }
    }

    regina::LargeInteger val = [HyperCoordinates getCoordinate:viewCoords surface:*s whichCoord:coord];
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
    if (indexPath.column < widthProps.count)
        return [widthProps[indexPath.column] floatValue];
    else
        return widthCoord;
}

- (CGFloat)spreadView:(MDSpreadView *)aSpreadView heightForRowHeaderInSection:(NSInteger)rowSection
{
    return (self.compact.on ? 1 : height);
}

- (CGFloat)spreadView:(MDSpreadView *)aSpreadView heightForRowAtIndexPath:(MDIndexPath *)indexPath
{
    return height;
}

- (void)spreadView:(MDSpreadView *)aSpreadView didSelectCellForRowAtIndexPath:(MDIndexPath *)rowPath forColumnAtIndexPath:(MDIndexPath *)columnPath
{
    if (rowPath.row >= 0)
        selectedRow = rowPath.row + 1;
    else
        selectedRow = 0;
    
    [self refreshActions];
}

// We don't implement didDeselectCell..., since this is always followed immediately by a new selection.

@end
