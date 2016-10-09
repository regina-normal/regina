
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

#import "HyperSummary.h"
#import "TextHelper.h"
#import "hypersurface/normalhypersurface.h"
#import "hypersurface/normalhypersurfaces.h"
#import <iterator>
#import <map>

#pragma mark - Hypersurfaces summary table cell

@interface HypersurfacesSummaryTableCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UILabel *homology;
@property (weak, nonatomic) IBOutlet UILabel *count0;
@property (weak, nonatomic) IBOutlet UILabel *count1;
@property (weak, nonatomic) IBOutlet UILabel *count2;
@property (weak, nonatomic) IBOutlet UILabel *count3;
@end

@implementation HypersurfacesSummaryTableCell
@end

#pragma mark - Hypersurfaces summary

struct HyperCountSet {
    size_t n1sOr, n2sOr, n1sNor, n2sNor, tot;
    int mask;
    
    HyperCountSet() : n1sOr(0), n2sOr(0), n1sNor(0), n2sNor(0), tot(0), mask(0) {
    }
    
    void reset() {
        n1sOr = n2sOr = n1sNor = n2sNor = tot = mask = 0;
    }
    
    void append(const regina::NormalHypersurface* s) {
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
    
    size_t field(int usingMask, int pos) const {
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
        size_t n = field(usingMask, pos);
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

@interface HypersurfacesSummary () <UITableViewDataSource, UITableViewDelegate> {
    CGFloat headerHeight, cellHeight;
    
    HyperCountSet nClosed, nBounded;
    std::map<std::string, HyperCountSet> homologyClosed, homologyBounded;
    size_t nSpun;
}
@property (weak, nonatomic) IBOutlet UITableView *table;
@end

@implementation HypersurfacesSummary

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
    homologyClosed.clear();
    homologyBounded.clear();
    nSpun = 0;
    
    size_t n = self.packet->size();
    const regina::NormalHypersurface* s;
    std::string homology;
    for (size_t i = 0; i < n; ++i) {
        s = self.packet->hypersurface(i);
        if (! s->isCompact())
            ++nSpun;
        else {
            homology = s->homology().utf8();
            if (s->hasRealBoundary()) {
                nBounded.append(s);
                homologyBounded[homology].append(s);
            } else {
                nClosed.append(s);
                homologyClosed[homology].append(s);
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
            return 1 + homologyClosed.size();
        --section;
    }
    
    if (nBounded.tot) {
        if (section == 0)
            return 1 + homologyBounded.size();
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
                                  singular:"non-compact hypersurface"
                                    plural:"non-compact hypersurfaces"];
        --section;
    }
    
    if (nClosed.tot) {
        if (section == 0)
            return [TextHelper countString:nClosed.tot
                                  singular:"closed hypersurface"
                                    plural:"closed hypersurfaces"];
        --section;
    }
    
    if (nBounded.tot) {
        if (section == 0)
            return [TextHelper countString:nBounded.tot
                                  singular:"bounded hypersurface"
                                    plural:"bounded hypersurfaces"];
        --section;
    }

    return @"No surfaces";
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    int section = indexPath.section;
    HypersurfacesSummaryTableCell* cell;
    
    if (nSpun) {
        if (section == 0) {
            if (indexPath.row == 0) {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Header" forIndexPath:indexPath];
                cell.count0.text = @"Total";
                cell.count1.text = cell.count2.text = cell.count3.text = @" ";
            } else {
                cell = [tableView dequeueReusableCellWithIdentifier:@"Count" forIndexPath:indexPath];
                cell.homology.text = @"Spun";
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
                auto it = homologyClosed.begin();
                advance(it, indexPath.row - 1);
                // UTF-8 strings need to be converted to NSStrings first via @(...).
                cell.homology.text = [NSString stringWithFormat:@"H₁ = %@", @(it->first.c_str())];
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
                auto it = homologyBounded.begin();
                advance(it, indexPath.row - 1);
                // UTF-8 strings need to be converted to NSStrings first via @(...).
                cell.homology.text = [NSString stringWithFormat:@"H₁ = %@", @(it->first.c_str())];
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
            HypersurfacesSummaryTableCell* cell = [self.table dequeueReusableCellWithIdentifier:@"Count"];
            cell.homology.text = @"H₁ = 0 Z₉₉ + 0 Z₉₉";
            [cell layoutIfNeeded];
            CGSize size = [cell.contentView systemLayoutSizeFittingSize:UILayoutFittingCompressedSize];
            cellHeight = size.height;
        }
        return cellHeight;
    }
}

@end
