
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

#import "SurfacesCompat.h"
#import "SurfacesViewController.h"
#import "surfaces/nnormalsurfacelist.h"

#define KEY_LAST_TYPE @"SurfacesCompatType"

static NSArray* typeText = @[@"Do surfaces have compatible quadrilateral and/or octagon types?",
                             @"Can surfaces be made globally disjoint?"];

#pragma mark - Compatibility grid view

@interface CompatGrid : UIView {
    NSMutableArray* usable;
    NSMutableArray* local;
    NSMutableArray* global;
}
@property (assign, nonatomic) regina::NNormalSurfaceList* packet;
@property (assign, nonatomic) BOOL showLocal;

@property (assign, nonatomic) CGFloat maxCellSize;
@property (assign, nonatomic) CGFloat lineWidth;
@property (assign, nonatomic) CGFloat lineOffset;
@property (strong, nonatomic) UIColor *gridColour;
@property (strong, nonatomic) UIColor *localCellColour;
@property (strong, nonatomic) UIColor *globalCellColour;
@end

@implementation CompatGrid

- (id)initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if (self) {
        // TODO: Fix all defaults.
        self.maxCellSize = 20;
        if (self.contentScaleFactor >= 2.0) {
            self.lineWidth = 0.5;
            self.lineOffset = 0.25;
        } else {
            self.lineWidth = 1.0;
            self.lineOffset = 0.5;
        }
        self.gridColour = [UIColor lightGrayColor];
        self.localCellColour = [UIColor cyanColor];
        self.globalCellColour = [UIColor greenColor];
        
        self.backgroundColor = [UIColor whiteColor];
        self.opaque = YES;
        self.contentMode = UIViewContentModeRedraw;
    }
    return self;
}

- (void)buildLocal
{
    unsigned long n = self.packet->getNumberOfSurfaces();
    unsigned long i, j;
    NSMutableArray* row;
    const regina::NNormalSurface* s;
    
    local = [[NSMutableArray alloc] initWithCapacity:n];
    for (i = 0; i < n; ++i) {
        s = self.packet->getSurface(i);
        row = [[NSMutableArray alloc] initWithCapacity:n];
        for (j = 0; j < n; ++j)
            [row addObject:[NSNumber numberWithBool:s->locallyCompatible(*(self.packet->getSurface(j)))]];
        [local addObject:row];
    }
}

- (void)buildGlobal
{
    unsigned long n = self.packet->getNumberOfSurfaces();
    unsigned long i, j;
    NSMutableArray* row;
    const regina::NNormalSurface* s;
    
    usable = [[NSMutableArray alloc] initWithCapacity:n];
    for (i = 0; i < n; ++i) {
        s = self.packet->getSurface(i);
        [usable addObject:@(BOOL(s->isCompact() && (! s->isEmpty()) && s->isConnected()))];
    }
    
    global = [[NSMutableArray alloc] initWithCapacity:n];
    for (i = 0; i < n; ++i) {
        if ([usable[i] boolValue]) {
            s = self.packet->getSurface(i);
            row = [[NSMutableArray alloc] initWithCapacity:n];
            for (j = 0; j < n; ++j)
                if ([usable[j] boolValue])
                    [row addObject:[NSNumber numberWithBool:s->disjoint(*(self.packet->getSurface(j)))]];
                else
                    [row addObject:NSNull.null];
            [global addObject:row];
        } else {
            [global addObject:NSNull.null];
        }
    }
}

- (void)reload
{
    local = global = nil;
    
    if (! self.packet)
        return;
    
    if (self.showLocal)
        [self buildLocal];
    else
        [self buildGlobal];
    
    [self setNeedsDisplay];
}

- (void)reloadShowingLocal:(BOOL)showLocal
{
    _showLocal = showLocal;
    [self reload];
}

- (void)setShowLocal:(BOOL)showLocal
{
    if (showLocal && ! _showLocal) {
        _showLocal = YES;
        if (! local)
            [self buildLocal];
    } else if (_showLocal && ! showLocal) {
        _showLocal = NO;
        if (! global)
            [self buildGlobal];
    }
    [self setNeedsDisplay];
}

- (void)drawRect:(CGRect)rect
{
    unsigned long n = self.packet->getNumberOfSurfaces();
    
    // TODO: Proper labels.
    CGFloat margin = 20;
    
    CGFloat size = MIN(self.bounds.size.width, self.bounds.size.height);
    CGFloat cellSize = (size - 2 * margin) / n;
    if (cellSize > self.maxCellSize)
        cellSize = self.maxCellSize;
    
    UIBezierPath *path = [UIBezierPath bezierPath];
    path.lineWidth = self.lineWidth;
    
    CGFloat originX = round((self.bounds.size.width - n * cellSize) / 2) - self.lineOffset;
    CGFloat originY = round((self.bounds.size.height - n * cellSize) / 2) - self.lineOffset;
    
    unsigned long i, j;
    CGFloat pos;
    for (i = 0, pos = originX; i <= n; ++i, pos += cellSize) {
        [path moveToPoint:CGPointMake(pos, originY)];
        [path addLineToPoint:CGPointMake(pos, originY + n * cellSize)];
    }
    for (i = 0, pos = originY; i <= n; ++i, pos += cellSize) {
        [path moveToPoint:CGPointMake(originX, pos)];
        [path addLineToPoint:CGPointMake(originX + n * cellSize, pos)];
    }
    
    [self.gridColour setStroke];
    [path stroke];

    NSArray* show;
    if (self.showLocal) {
        show = local;
        [self.localCellColour setFill];
    } else {
        show = global;
        [self.globalCellColour setFill];
    }
    
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            if (show[i] == NSNull.null || show[i][j] == NSNull.null) {
                // TODO: Cross out.
            } else {
                if ([show[i][j] boolValue]) {
                    path = [UIBezierPath bezierPathWithRect:CGRectMake(originX + self.lineWidth + j * cellSize,
                                                                       originY + self.lineWidth + i * cellSize,
                                                                       cellSize - self.lineWidth,
                                                                       cellSize - self.lineWidth)];
                    [path fill];
                }
            }
        }
    }
}

@end

#pragma mark - Surfaces compatibility tab

@interface SurfacesCompat ()
@property (weak, nonatomic) IBOutlet UISegmentedControl *type;
@property (weak, nonatomic) IBOutlet UILabel *typeExpln;
@property (weak, nonatomic) IBOutlet CompatGrid *grid;
@end

@implementation SurfacesCompat

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    self.grid.packet = self.packet;

    self.type.selectedSegmentIndex = [[NSUserDefaults standardUserDefaults] integerForKey:KEY_LAST_TYPE];
    self.typeExpln.text = typeText[self.type.selectedSegmentIndex];

    [self reloadPacket];
}

- (void)reloadPacket
{
    [super reloadPacket];
    [self.grid reloadShowingLocal:(self.type.selectedSegmentIndex == 0)];
}

- (IBAction)typeChanged:(id)sender {
    self.typeExpln.text = typeText[self.type.selectedSegmentIndex];
    [[NSUserDefaults standardUserDefaults] setInteger:self.type.selectedSegmentIndex forKey:KEY_LAST_TYPE];
    
    self.grid.showLocal = (self.type.selectedSegmentIndex == 0);
}

@end
