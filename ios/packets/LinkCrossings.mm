
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

@property (weak, nonatomic) IBOutlet UICollectionView *crossings;

@property (assign, nonatomic) regina::Link* packet;
@end

@implementation LinkCrossings

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];

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
    
    sizeLabel = [refLabel sizeWithAttributes:@{NSFontAttributeName: self.header.font}];
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

#pragma mark - Collection view

- (NSInteger)numberOfSectionsInCollectionView:(UICollectionView *)collectionView {
    return self.packet->countComponents();
}

- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    return ((NSArray*)components[section]).count;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    // TODO: Offer two options via a segmented control: diagrams vs 0^+ 0_+ attributed text.
    regina::StrandRef s;
    [(NSValue*)components[indexPath.section][indexPath.row] getValue:&s];
    
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
}

- (UICollectionReusableView *)collectionView:(UICollectionView *)collectionView viewForSupplementaryElementOfKind:(NSString *)kind atIndexPath:(NSIndexPath *)indexPath {
    LinkComponentHeader* header = (LinkComponentHeader*)[collectionView dequeueReusableSupplementaryViewOfKind:kind withReuseIdentifier:@"component" forIndexPath:indexPath];
    header.label.text = [NSString stringWithFormat:@"Component %ld", (long)indexPath.section];
    return header;
}

- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath {
    // Labels are positioned 10 points from the left edge of the cell, though
    // the UILabel itself will add a bit more padding.
    // The crossing image in the cell is 22x22, and we need to clear a bit more than
    // halfway in both directions.
    // We give a minimum width of 40 because, if the original width is smaller,
    // it means there are very few crossings and we do not exactly need to cram
    // things in.
    return CGSizeMake(fmax(ceil(sizeLabel.width) + 18, 40),
                      ceil(sizeLabel.height) + 18);
}

- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout referenceSizeForHeaderInSection:(NSInteger)section {
    return CGSizeMake(0, ceil(sizeHeader.height) + 30);
}

@end