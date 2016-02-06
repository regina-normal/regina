
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

#import "SpreadHelper.h"

// These fonts are hard-coded into the MDSpreadView classes.
// Replicate them here so we can compute cell sizes.
static UIFont* cellFont = [UIFont systemFontOfSize:16];
static UIFont* headerFont = [UIFont boldSystemFontOfSize:14];

#define MD_CELL_WIDTH_PADDING 10
#define MD_CELL_HEIGHT_PADDING 3

#define MD_CELL_TIGHT_WIDTH_PADDING 2
#define MD_CELL_TIGHT_HEIGHT_PADDING 1

// Note: The use of clearColor in the header cells is a cheat.
// I suspect that if we try to select such a cell, the clear color will be lost.
// The symptom it addresses is that, for header height 1, the UILabel inside the cell bleeds colour outside the cell.

@interface RegularSpreadViewCell ()
@end

@implementation RegularSpreadViewCell

+ (CGSize)cellSizeFor:(NSString *)text
{
    CGSize s = [text sizeWithAttributes:@{NSFontAttributeName: cellFont}];
    s.width += (2 * MD_CELL_WIDTH_PADDING + 2);
    s.height += (2 * MD_CELL_HEIGHT_PADDING);
    return s;
}

- (id)initWithReuseIdentifier:(NSString *)identifier
{
    return [super initWithStyle:MDSpreadViewCellStyleDefault reuseIdentifier:identifier];
}

- (void)layoutSubviews
{
    [super layoutSubviews];
    self.textLabel.frame = CGRectMake(MD_CELL_WIDTH_PADDING,
                                      MD_CELL_HEIGHT_PADDING,
                                      self.bounds.size.width - 2 * MD_CELL_WIDTH_PADDING,
                                      self.bounds.size.height - 2 * MD_CELL_HEIGHT_PADDING);
}

@end

@interface CompactSpreadViewCell ()
@end

@implementation CompactSpreadViewCell

+ (CGSize)cellSizeFor:(NSString *)text
{
    CGSize s = [text sizeWithAttributes:@{NSFontAttributeName: cellFont}];
    s.width += (2 * MD_CELL_TIGHT_WIDTH_PADDING + 2);
    s.height += (2 * MD_CELL_TIGHT_HEIGHT_PADDING);
    return s;
}

- (id)initWithReuseIdentifier:(NSString *)identifier
{
    return [super initWithStyle:MDSpreadViewCellStyleDefault reuseIdentifier:identifier];
}

- (void)layoutSubviews
{
    [super layoutSubviews];
    self.textLabel.frame = CGRectMake(MD_CELL_TIGHT_WIDTH_PADDING,
                                      MD_CELL_TIGHT_HEIGHT_PADDING,
                                      self.bounds.size.width - 2 * MD_CELL_TIGHT_WIDTH_PADDING,
                                      self.bounds.size.height - 2 * MD_CELL_TIGHT_HEIGHT_PADDING);
}

@end

@implementation RegularSpreadHeaderCell

+ (CGSize)cellSizeFor:(NSString *)text
{
    CGSize s = [text sizeWithAttributes:@{NSFontAttributeName: headerFont}];
    s.width += (2 * MD_CELL_WIDTH_PADDING + 2);
    s.height += (2 * MD_CELL_HEIGHT_PADDING);
    return s;
}

- (void)layoutSubviews
{
    [super layoutSubviews];
    self.textLabel.frame = CGRectMake(MD_CELL_WIDTH_PADDING,
                                      MD_CELL_HEIGHT_PADDING,
                                      self.bounds.size.width - 2 * MD_CELL_WIDTH_PADDING,
                                      self.bounds.size.height - 2 * MD_CELL_HEIGHT_PADDING);
}

@end

@implementation RegularSpreadHeaderCellCentre

- (id)initWithStyle:(MDSpreadViewHeaderCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        self.textLabel.textAlignment = NSTextAlignmentCenter;
        self.textLabel.backgroundColor = [UIColor clearColor];
    }
    return self;
}

@end

@implementation RegularSpreadHeaderCellRight

- (id)initWithStyle:(MDSpreadViewHeaderCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        self.textLabel.textAlignment = NSTextAlignmentRight;
        self.textLabel.backgroundColor = [UIColor clearColor];
    }
    return self;
}

@end

@implementation RegularSpreadHeaderCellEmpty

- (id)initWithStyle:(MDSpreadViewHeaderCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self)
        self.textLabel.hidden = YES;
    return self;
}

@end
