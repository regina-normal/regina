
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

#import "SurfacesSummary.h"
#import "TextHelper.h"
#import "surfaces/normalsurface.h"
#import "surfaces/normalsurfaces.h"
#import <iterator>
#import <map>

#pragma mark - Surfaces summary table cell

@interface SurfacesSummaryTableCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UILabel *euler;
@property (weak, nonatomic) IBOutlet UILabel *count0;
@property (weak, nonatomic) IBOutlet UILabel *count1;
@property (weak, nonatomic) IBOutlet UILabel *count2;
@property (weak, nonatomic) IBOutlet UILabel *count3;
@end

@implementation SurfacesSummaryTableCell
@end

#pragma mark - Surfaces summary

struct CountSet {
    unsigned long n1sOr, n2sOr, n1sNor, n2sNor, tot;
    int mask;
    
    CountSet() : n1sOr(0), n2sOr(0), n1sNor(0), n2sNor(0), tot(0), mask(0) {
    }
    
    void reset() {
        n1sOr = n2sOr = n1sNor = n2sNor = tot = mask = 0;
    }
    
    void append(const regina::NormalSurface* s) {
        if (s->isOrientable()) {
            if (s->isTwoSided()) {
                ++n2sOr;
                mask |= 1;
            } else {
                ++n1sOr;
                mask |= 2;
            }
        } else {
            if (s->isTwoSided()) {
                ++n2sNor;
                mask |= 4;
            } else {
                ++n1sNor;
                mask |= 8;
            }
        }
        ++tot;
    }
    
    unsigned long field(int usingMask, int pos) const {
        if (usingMask & 1) {
            if (pos == 0)
                return n2sOr;
            --pos;
        }
        if (usingMask & 2) {
            if (pos == 0)
                return n1sOr;
            --pos;
        }
        if (usingMask & 4) {
            if (pos == 0)
                return n2sNor;
            --pos;
        }
        if (usingMask & 8) {
            if (pos == 0)
                return n1sNor;
            --pos;
        }
        return 0;
    }
    
    NSString* fieldString(int usingMask, int pos) const {
        unsigned long n = field(usingMask, pos);
        if (n == 0)
            return @" ";
        else
            return [NSString stringWithFormat:@"%ld", n];
    }
    
    static NSString* fieldTitle(int usingMask, int pos) {
        if (usingMask & 1) {
            if (pos == 0)
                return @"2-sided orbl";
            --pos;
        }
        if (usingMask & 2) {
            if (pos == 0)
                return @"1-sided orbl";
            --pos;
        }
        if (usingMask & 4) {
            if (pos == 0)
                return @"2-sided non-orbl";
            --pos;
        }
        if (usingMask & 8) {
            if (pos == 0)
                return @"1-sided non-orbl";
            --pos;
        }
        // Return an empty string for positions that are too large,
        // so we can use this for extraneous labels.
        return @" ";
    }
};

@interface SurfacesSummary () <UITableViewDataSource, UITableViewDelegate> {
    CGFloat headerHeight, cellHeight;
    
    CountSet nClosed, nBounded;
    std::map<regina::LargeInteger, CountSet> eulerClosed, eulerBounded;
    unsigned long nSpun;
}
@property (weak, nonatomic) IBOutlet UITableView *table;
@end

@implementation SurfacesSummary

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];

    self.table.dataSource = self;
    self.table.delegate = self;

    [self reloadPacket];
}

- (void)reloadPacket
{
    [super reloadPacket];
    
    nClosed.reset();
    nBounded.reset();
    eulerClosed.clear();
    eulerBounded.clear();
    nSpun = 0;
    
    unsigned long n = self.packet->size();
    const regina::NormalSurface* s;
    regina::LargeInteger euler;
    for (unsigned long i = 0; i < n; ++i) {
        s = self.packet->surface(i);
        if (! s->isCompact())
            ++nSpun;
        else {
            euler = s->eulerChar();
            if (s->hasRealBoundary()) {
                nBounded.append(s);
                eulerBounded[euler].append(s);
            } else {
                nClosed.append(s);
                eulerClosed[euler].append(s);
            }
        }
    }
    
    [self.table reloadData];
}

#pragma mark - Table view

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if (nSpun) {
        if (section == 0)
            return 2;
        --section;
    }
    
    if (nClosed.tot) {
        if (section == 0)
            return 1 + eulerClosed.size();
        --section;
    }
    
    if (nBounded.tot) {
        if (section == 0)
            return 1 + eulerBounded.size();
        --section;
    }

    return 0;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return (nSpun ? 1 : 0) + (nClosed.tot ? 1 : 0) + (nBounded.tot ? 1 : 0);
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    if (nSpun) {
        if (section == 0)
            return [TextHelper countString:nSpun
                                  singular:"non-compact surface"
                                    plural:"non-compact surfaces"];
        --section;
    }
    
    if (nClosed.tot) {
        if (section == 0)
            return [TextHelper countString:nClosed.tot
                                  singular:"closed surface"
                                    plural:"closed surfaces"];
        --section;
    }
    
    if (nBounded.tot) {
        if (section == 0)
            return [TextHelper countString:nBounded.tot
                                  singular:"bounded surface"
                                    plural:"bounded surfaces"];
        --section;
    }

    return @"No surfaces";
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    int section = indexPath.section;
    SurfacesSummaryTableCell* cell;
    
    if (nSpun) {
        if (section == 0) {
            if (indexPath.row == 0) {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Header" forIndexPath:indexPath];
                cell.count0.text = @"Total";
                cell.count1.text = cell.count2.text = cell.count3.text = @" ";
            } else {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Count" forIndexPath:indexPath];
                cell.euler.text = @"Spun";
                cell.count0.text = [NSString stringWithFormat:@"%ld", nSpun];
                cell.count1.text = cell.count2.text = cell.count3.text = @" ";
            }
            return cell;
        }
        --section;
    }
    
    if (nClosed.tot) {
        if (section == 0) {
            if (indexPath.row == 0) {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Header" forIndexPath:indexPath];
                cell.count0.text = nClosed.fieldTitle(nClosed.mask, 0);
                cell.count1.text = nClosed.fieldTitle(nClosed.mask, 1);
                cell.count2.text = nClosed.fieldTitle(nClosed.mask, 2);
                cell.count3.text = nClosed.fieldTitle(nClosed.mask, 3);
            } else {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Count" forIndexPath:indexPath];
                std::map<regina::LargeInteger, CountSet>::const_reverse_iterator it = eulerClosed.rbegin();
                advance(it, indexPath.row - 1);
                cell.euler.text = [NSString stringWithFormat:@"χ = %s", it->first.stringValue().c_str()];
                cell.count0.text = it->second.fieldString(nClosed.mask, 0);
                cell.count1.text = it->second.fieldString(nClosed.mask, 1);
                cell.count2.text = it->second.fieldString(nClosed.mask, 2);
                cell.count3.text = it->second.fieldString(nClosed.mask, 3);
            }
            return cell;
        }
        --section;
    }
    
    if (nBounded.tot) {
        if (section == 0) {
            if (indexPath.row == 0) {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Header" forIndexPath:indexPath];
                cell.count0.text = nBounded.fieldTitle(nBounded.mask, 0);
                cell.count1.text = nBounded.fieldTitle(nBounded.mask, 1);
                cell.count2.text = nBounded.fieldTitle(nBounded.mask, 2);
                cell.count3.text = nBounded.fieldTitle(nBounded.mask, 3);
            } else {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Count" forIndexPath:indexPath];
                std::map<regina::LargeInteger, CountSet>::const_reverse_iterator it = eulerBounded.rbegin();
                advance(it, indexPath.row - 1);
                cell.euler.text = [NSString stringWithFormat:@"χ = %s", it->first.stringValue().c_str()];
                cell.count0.text = it->second.fieldString(nBounded.mask, 0);
                cell.count1.text = it->second.fieldString(nBounded.mask, 1);
                cell.count2.text = it->second.fieldString(nBounded.mask, 2);
                cell.count3.text = it->second.fieldString(nBounded.mask, 3);
            }
            return cell;
        }
        --section;
    }

    // Should never reach here.
    return nil;
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    return NO;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == 0) {
        if (headerHeight == 0) {
            UITableViewCell* cell = [self.table dequeueReusableCellWithIdentifier:@"Header"];
            [cell layoutIfNeeded];
            CGSize size = [cell.contentView systemLayoutSizeFittingSize:UILayoutFittingCompressedSize];
            headerHeight = size.height;
        }
        return headerHeight;
    } else {
        if (cellHeight == 0) {
            SurfacesSummaryTableCell* cell = [self.table dequeueReusableCellWithIdentifier:@"Count"];
            cell.euler.text = @"χ = -9";
            [cell layoutIfNeeded];
            CGSize size = [cell.contentView systemLayoutSizeFittingSize:UILayoutFittingCompressedSize];
            cellHeight = size.height;
        }
        return cellHeight;
    }
}

@end
