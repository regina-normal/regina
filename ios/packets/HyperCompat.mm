
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

#import "HyperCompat.h"
#import "HyperViewController.h"
#import "hypersurface/normalhypersurfaces.h"

#define MARGIN 20
#define TICK_LENGTH 5
#define TICK_GAP_HORIZ 5
#define TICK_GAP_VERT 2

#pragma mark - Compatibility grid view

@interface HyperCompatGrid : UIView {
    NSMutableArray* usable;
    NSMutableArray* local;
}
@property (assign, nonatomic) regina::NormalHypersurfaces* packet;

@property (assign, nonatomic) CGFloat minCellSize;
@property (assign, nonatomic) CGFloat maxCellSize;
@property (assign, nonatomic) CGFloat lineWidth;
@property (assign, nonatomic) CGFloat lineOffset;
@property (strong, nonatomic) UIColor *gridColour;
@property (strong, nonatomic) UIColor *labelColour;
@property (strong, nonatomic) UIColor *localCellColour;
@property (strong, nonatomic) UIColor *strikeoutColour;
@property (strong, nonatomic) UIFont *labelFont;
@end

@implementation HyperCompatGrid

- (id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if (self) {
        self.minCellSize = 4;
        self.maxCellSize = 20;
        if (self.contentScaleFactor >= 2.0) {
            self.lineWidth = 0.5;
            self.lineOffset = 0.25;
        } else {
            self.lineWidth = 1.0;
            self.lineOffset = 0.5;
        }
        self.gridColour = [UIColor lightGrayColor];
        self.labelColour = [UIColor darkGrayColor];
        // Powder blue: #B0E0E6
        self.localCellColour = [UIColor colorWithRed:(0xb0 / 256.0) green:(0xe0 / 256.0) blue:(0xe6 / 256.0) alpha:1];
        // Dark red: #8B0000
        self.strikeoutColour = [UIColor colorWithRed:(0x8b / 256.0) green:0 blue:0 alpha:1];
        self.labelFont = [UIFont systemFontOfSize:14];
        
        self.backgroundColor = [UIColor whiteColor];
        self.opaque = YES;
        self.contentMode = UIViewContentModeRedraw;
    }
    return self;
}

- (void)buildLocal
{
    size_t n = self.packet->size();
    size_t i, j;
    NSMutableArray* row;
    const regina::NormalHypersurface* s;
    
    local = [[NSMutableArray alloc] initWithCapacity:n];
    for (i = 0; i < n; ++i) {
        s = self.packet->hypersurface(i);
        row = [[NSMutableArray alloc] initWithCapacity:n];
        for (j = 0; j < n; ++j)
            [row addObject:[NSNumber numberWithBool:s->locallyCompatible(*(self.packet->hypersurface(j)))]];
        [local addObject:row];
    }
}

- (void)reload
{
    local = nil;
    [self setNeedsDisplay];
}

- (void)drawError:(NSString*)text
{
    UIFont* errorFont = [UIFont systemFontOfSize:[UIFont systemFontSize]];
    CGSize textBounds = [text sizeWithAttributes:@{NSFontAttributeName: errorFont}];
 
    [text drawAtPoint:CGPointMake((self.bounds.size.width - textBounds.width) / 2,
                                  (self.bounds.size.height - textBounds.height) / 2)
      withAttributes:@{NSFontAttributeName:errorFont,
                       NSForegroundColorAttributeName:self.strikeoutColour}];
}

- (void)drawRect:(CGRect)rect
{
    if (! self.packet)
        return;
    
    size_t n = self.packet->size();
    
    if (n == 0) {
        [self drawError:@"No hypersurfaces to display"];
        return;
    }
    
    // Compute the cell size.
    CGSize maxLabel = [[NSString stringWithFormat:@"%ld", n] sizeWithAttributes:@{NSFontAttributeName: self.labelFont}];
    
    CGFloat availableHeight = self.bounds.size.height - 2 * MARGIN - maxLabel.height - TICK_LENGTH - TICK_GAP_VERT;
    CGFloat availableWidth = self.bounds.size.width - 2 * MARGIN - maxLabel.width - TICK_LENGTH - TICK_GAP_HORIZ;
    
    CGFloat cellSize = floor(MIN(availableHeight, availableWidth) / n);
    if (cellSize > self.maxCellSize)
        cellSize = self.maxCellSize;
    
    if (cellSize < self.minCellSize) {
        [self drawError:@"Too many hypersurfaces to display"];
        return;
    }
    
    // Compute the compatibility values if necessary.
    NSArray* show;
    if (! local)
        [self buildLocal];
    show = local;
    
    // How often to draw ticks?
    int tickPeriod;
    CGFloat cellsForLabel = (maxLabel.width / cellSize);
    if (cellsForLabel < 2.5)
        tickPeriod = 5;
    else if (cellsForLabel < 6)
        tickPeriod = 10;
    else if (cellsForLabel < 15)
        tickPeriod = 20;
    else if (cellsForLabel < 40)
        tickPeriod = 50;
    else
        cellsForLabel = 100;

    // Draw it!
    UIBezierPath *path = [UIBezierPath bezierPath];
    path.lineWidth = self.lineWidth;
    
    CGSize displaySize = CGSizeMake(n * cellSize + self.lineWidth + maxLabel.width + TICK_LENGTH + TICK_GAP_HORIZ,
                                    n * cellSize + self.lineWidth + maxLabel.height + TICK_LENGTH + TICK_GAP_VERT);
    
    CGFloat originX = round((self.bounds.size.width - displaySize.width) / 2 + maxLabel.width + TICK_LENGTH + TICK_GAP_HORIZ);
    CGFloat originY = round((self.bounds.size.height - displaySize.height) / 2 + maxLabel.height + TICK_LENGTH + TICK_GAP_VERT);
    
    unsigned long i, j;
    CGFloat pos;
    NSString* label;
    NSMutableParagraphStyle* align = [[NSMutableParagraphStyle alloc] init];
    for (i = 0, pos = originX + self.lineOffset; i <= n; ++i, pos += cellSize) {
        [path moveToPoint:CGPointMake(pos, originY + self.lineOffset)];
        [path addLineToPoint:CGPointMake(pos, originY + self.lineOffset + n * cellSize)];
        
        if (i % tickPeriod == 0) {
            [path moveToPoint:CGPointMake(pos + (cellSize / 2), originY + self.lineOffset)];
            [path addLineToPoint:CGPointMake(pos + (cellSize / 2), originY + self.lineOffset - TICK_LENGTH)];
            
            label = [NSString stringWithFormat:@"%ld", i];
            align.alignment = NSTextAlignmentCenter;
            [label drawInRect:CGRectMake(pos + (cellSize / 2) - maxLabel.width,
                                         originY + self.lineOffset - TICK_LENGTH - TICK_GAP_VERT - maxLabel.height,
                                         maxLabel.width * 2,
                                         maxLabel.height)
               withAttributes:@{NSFontAttributeName:self.labelFont,
                                NSForegroundColorAttributeName:self.labelColour,
                                NSParagraphStyleAttributeName:align}];
        }
    }
    for (i = 0, pos = originY + self.lineOffset; i <= n; ++i, pos += cellSize) {
        [path moveToPoint:CGPointMake(originX + self.lineOffset, pos)];
        [path addLineToPoint:CGPointMake(originX + self.lineOffset + n * cellSize, pos)];
        
        if (i % tickPeriod == 0) {
            [path moveToPoint:CGPointMake(originX + self.lineOffset, pos + (cellSize / 2))];
            [path addLineToPoint:CGPointMake(originX + self.lineOffset - TICK_LENGTH, pos + (cellSize / 2))];

            
            label = [NSString stringWithFormat:@"%ld", i];
            align.alignment = NSTextAlignmentRight;
            [label drawInRect:CGRectMake(originX + self.lineOffset - TICK_LENGTH - TICK_GAP_HORIZ - maxLabel.width * 2,
                                         pos + (cellSize / 2) - (maxLabel.height / 2),
                                         maxLabel.width * 2,
                                         maxLabel.height)
               withAttributes:@{NSFontAttributeName:self.labelFont,
                                NSForegroundColorAttributeName:self.labelColour,
                                NSParagraphStyleAttributeName:align}];
        }
    }
    
    [self.gridColour setStroke];
    [path stroke];
    
    [self.localCellColour setFill];
    
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            if (show[i] == NSNull.null || show[i][j] == NSNull.null) {
                path = [UIBezierPath bezierPath];
                path.lineWidth = self.lineWidth;
                
                [path moveToPoint:CGPointMake(originX + self.lineOffset + self.lineWidth + j * cellSize,
                                              originY + self.lineOffset + self.lineWidth + i * cellSize)];
                [path addLineToPoint:CGPointMake(originX + self.lineOffset - self.lineWidth + (j+1) * cellSize,
                                                 originY + self.lineOffset - self.lineWidth + (i+1) * cellSize)];
                
                [path moveToPoint:CGPointMake(originX + self.lineOffset + self.lineWidth + j * cellSize,
                                              originY + self.lineOffset - self.lineWidth + (i+1) * cellSize)];
                [path addLineToPoint:CGPointMake(originX + self.lineOffset - self.lineWidth + (j+1) * cellSize,
                                                 originY + self.lineOffset + self.lineWidth + i * cellSize)];
                
                [self.strikeoutColour setStroke];
                [path stroke];
            } else if ([show[i][j] boolValue]) {
                path = [UIBezierPath bezierPathWithRect:CGRectMake(originX + self.lineWidth + j * cellSize,
                                                                   originY + self.lineWidth + i * cellSize,
                                                                   cellSize - self.lineWidth,
                                                                   cellSize - self.lineWidth)];
                [path fill];
            }
        }
    }
}

@end

#pragma mark - Hypersurfaces compatibility tab

@interface HypersurfacesCompat ()
@property (weak, nonatomic) IBOutlet HyperCompatGrid *grid;
@end

@implementation HypersurfacesCompat

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];

    self.grid.packet = self.packet;

    [self reloadPacket];
}

- (void)reloadPacket
{
    [super reloadPacket];
    [self.grid reload];
}

@end
