
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

@implementation SpreadHelper

+ (CGSize)cellSize:(NSString *)text
{
    return [text sizeWithAttributes:@{NSFontAttributeName: cellFont}];
}

+ (CGSize)headerSize:(NSString *)text
{
    return [text sizeWithAttributes:@{NSFontAttributeName: headerFont}];
}

@end

@interface CompactSpreadViewCell ()
@end

@implementation CompactSpreadViewCell

- (void)layoutSubviews
{
    [super layoutSubviews];
    self.textLabel.frame = CGRectMake((MD_CELL_TIGHT_WIDTH_PADDING / 2),
                                      (MD_CELL_TIGHT_HEIGHT_PADDING / 2),
                                      self.bounds.size.width - (MD_CELL_TIGHT_WIDTH_PADDING / 2),
                                      self.bounds.size.height - (MD_CELL_TIGHT_HEIGHT_PADDING / 2));
}

@end
