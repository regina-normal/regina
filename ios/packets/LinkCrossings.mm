
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2017, Ben Burton                                   *
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

#import "LinkViewController.h"
#import "LinkCrossings.h"
#import "link/link.h"

#define KEY_LINK_CROSSINGS_STYLE @"LinkCrossingsStyle"

static UIColor* negColour = [UIColor colorWithRed:(0xB8 / 256.0)
                                            green:(0x86 / 256.0)
                                             blue:(0x0B / 256.0)
                                            alpha:1.0]; // Dark goldenrod

static UIColor* posColour = [UIColor colorWithRed:(0x2B / 256.0)
                                            green:(0x54 / 256.0)
                                             blue:(0x7E / 256.0)
                                            alpha:1.0]; // Blue jay

#pragma mark - Crossing cell

@interface LinkCrossingCell : UICollectionViewCell
@property (weak, nonatomic) IBOutlet UILabel *index;
@end

@implementation LinkCrossingCell
@end

#pragma mark - Component header

@interface LinkComponentHeader : UICollectionReusableView
@property (weak, nonatomic) IBOutlet UILabel *label;
@end

@implementation LinkComponentHeader
@end

#pragma mark - Link crossings

@interface LinkCrossings () <UICollectionViewDataSource, UICollectionViewDelegateFlowLayout> {
    NSMutableArray* components;
    NSString* refLabel;
    CGSize sizeLabel;
    CGSize sizeHeader;
}

@property (weak, nonatomic) IBOutlet UILabel *header;

@property (weak, nonatomic) IBOutlet UISegmentedControl *style;
@property (weak, nonatomic) IBOutlet UICollectionView *crossings;

@property (assign, nonatomic) regina::Link* packet;
@end

@implementation LinkCrossings

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.style.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:KEY_LINK_CROSSINGS_STYLE];

    self.packet = static_cast<regina::Link*>(static_cast<id<PacketViewer> >(self.parentViewController).packet);
    
    self.crossings.delegate = self;
    self.crossings.dataSource = self;
    
    components = [[NSMutableArray alloc] initWithCapacity:self.packet->countComponents()];
    [self reloadPacket];
}

- (void)reloadPacket
{
    
    [static_cast<LinkViewController*>(self.parentViewController) updateHeader:self.header];

    // Cache the crossings for each component.
    [components removeAllObjects];
    for (size_t i = 0; i < self.packet->countComponents(); ++i) {
        NSMutableArray* comp = [[NSMutableArray alloc] init];
        regina::StrandRef start = self.packet->component(i);
        regina::StrandRef s = start;
        do {
            [comp addObject:[NSValue valueWithBytes:&s objCType:@encode(regina::StrandRef)]];
            ++s;
        } while (s != start);
        [components addObject:comp];
    }
    
    if (self.packet->size() < 10)
        refLabel = @"0";
    else if (self.packet->size() < 20)
        refLabel = @"10";
    else if (self.packet->size() < 100)
        refLabel = @"00";
    else if (self.packet->size() < 200)
        refLabel = @"100";
    else if (self.packet->size() < 1000)
        refLabel = @"000";
    else if (self.packet->size() < 2000)
        refLabel = @"1000";
    else if (self.packet->size() < 10000)
        refLabel = @"0000";
    else
        refLabel = @"00000";
    
    if (self.style.selectedSegmentIndex == 0) {
        // Pictorial display
        sizeLabel = [refLabel sizeWithAttributes:@{NSFontAttributeName: self.header.font}];
    } else {
        // Text display
        sizeLabel = [[NSString stringWithFormat:@"%@₊", refLabel] sizeWithAttributes:@{NSFontAttributeName: self.header.font}];
    }
    sizeHeader = [refLabel sizeWithAttributes:@{NSFontAttributeName: [UIFont boldSystemFontOfSize:self.header.font.pointSize]}];
    
    [self.crossings reloadData];
}

- (IBAction)complement:(id)sender {
    // TODO
}

- (IBAction)simplify:(id)sender {
    // TODO... both code and icon
}

- (IBAction)actions:(id)sender {
    // TODO
}

- (IBAction)styleChanged:(id)sender {
    [[NSUserDefaults standardUserDefaults] setInteger:self.style.selectedSegmentIndex forKey:KEY_LINK_CROSSINGS_STYLE];
    [self reloadPacket];
}

#pragma mark - Collection view

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView {
    return self.packet->countComponents();
}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    return ((NSArray*)components[section]).count;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    regina::StrandRef s;
    [(NSValue*)components[indexPath.section][indexPath.row] getValue:&s];
    
    if (self.style.selectedSegmentIndex == 0) {
        // Pictorial display.
        NSString* cellId;
        if (s.crossing()->sign() > 0) {
            if (s.strand() == 0)
                cellId = @"+l";
            else
                cellId = @"+u";
        } else {
            if (s.strand() == 0)
                cellId = @"-l";
            else
                cellId = @"-u";
        }
        
        LinkCrossingCell* cell = (LinkCrossingCell*)[collectionView dequeueReusableCellWithReuseIdentifier:cellId forIndexPath:indexPath];
        cell.index.text = [NSString stringWithFormat:@"%d", s.crossing()->index()];
        return cell;
    } else {
        // Text-based display.
        LinkCrossingCell* cell = (LinkCrossingCell*)[collectionView dequeueReusableCellWithReuseIdentifier:@"text" forIndexPath:indexPath];
        
        if (s.crossing()->sign() > 0) {
            if (s.strand() == 0)
                cell.index.text = [NSString stringWithFormat:@"%d₊", s.crossing()->index()];
            else
                cell.index.text = [NSString stringWithFormat:@"%d⁺", s.crossing()->index()];
            cell.index.textColor = posColour;
        } else {
            if (s.strand() == 0)
                cell.index.text = [NSString stringWithFormat:@"%d₋", s.crossing()->index()];
            else
                cell.index.text = [NSString stringWithFormat:@"%d⁻", s.crossing()->index()];
            cell.index.textColor = negColour;
        }
        
        return cell;
    }
}

- (UICollectionReusableView *)collectionView:(UICollectionView *)collectionView viewForSupplementaryElementOfKind:(NSString *)kind atIndexPath:(NSIndexPath *)indexPath {
    LinkComponentHeader* header = (LinkComponentHeader*)[collectionView dequeueReusableSupplementaryViewOfKind:kind withReuseIdentifier:@"component" forIndexPath:indexPath];
    header.label.text = [NSString stringWithFormat:@"Component %ld", (long)indexPath.section];
    return header;
}

- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath {
    if (self.style.selectedSegmentIndex == 0) {
        // Pictorial display

        // Labels are positioned 10 points from the left edge of the cell, though
        // the UILabel itself will add a bit more padding.
        // The crossing image in the cell is 22x22, and we need to clear a bit more than
        // halfway in both directions.
        // We give a minimum width of 40 because, if the original width is smaller,
        // it means there are very few crossings and we do not exactly need to cram
        // things in.
        return CGSizeMake(fmax(ceil(sizeLabel.width) + 18, 40),
                          ceil(sizeLabel.height) + 18);
    } else {
        // Text display
        // Labels are vertically centred, horizontally push to the very left of the cell.
        return CGSizeMake(ceil(sizeLabel.width) + 10,
                          ceil(sizeLabel.height) + 10);
    }
}

- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout referenceSizeForHeaderInSection:(NSInteger)section {
    return CGSizeMake(0, ceil(sizeHeader.height) + 30);
}

@end
