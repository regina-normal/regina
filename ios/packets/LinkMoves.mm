
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#import "LinkMoves.h"
#import "LinkViewController.h"
#import "TextHelper.h"
#import "link/link.h"

namespace {
    NSString* strandDesc(const regina::StrandRef& strand) {
        // PRE: strand is not a null strand.
        if (strand.strand() == 0)
            return [NSString stringWithFormat:@"%d lower", strand.crossing()->index()];
        else
            return [NSString stringWithFormat:@"%d upper", strand.crossing()->index()];
    }
}

#pragma mark - R1 up

@interface R1UpArg : NSObject
@property (assign, nonatomic) regina::StrandRef strand;
@property (assign, nonatomic) int side;
@property (assign, nonatomic) int sign;
- (id)init:(const regina::StrandRef&)strand side:(int)side sign:(int)sign;
- (NSAttributedString*)display;
@end

@implementation R1UpArg
- (id)init:(const regina::StrandRef&)strand side:(int)side sign:(int)sign
{
    self = [super init];
    if (self) {
        _strand = strand;
        _side = side;
        _sign = sign;
    }
    return self;
}
- (NSAttributedString*)display
{
    NSMutableAttributedString* text = [[NSMutableAttributedString alloc] init];
    
    if (self.strand.crossing()) {
        regina::StrandRef s = self.strand;
        [text appendAttributedString:[[NSAttributedString alloc] initWithString:strandDesc(s)]];
        [text appendAttributedString:[[NSAttributedString alloc] initWithString:@" → "]];
        ++s;
        [text appendAttributedString:[[NSAttributedString alloc] initWithString:strandDesc(s)]];
        
        [text appendAttributedString:[[NSAttributedString alloc] initWithString:@", "]];
        
        if (self.side == 0)
            [text appendAttributedString:[[NSAttributedString alloc] initWithString:@"left" attributes:@{NSForegroundColorAttributeName: LinkViewController.leftColour}]];
        else
            [text appendAttributedString:[[NSAttributedString alloc] initWithString:@"right" attributes:@{NSForegroundColorAttributeName: LinkViewController.rightColour}]];
    } else {
        [text appendAttributedString:[[NSAttributedString alloc] initWithString:@"Unknotted circle"]];
    }
    
    [text appendAttributedString:[[NSAttributedString alloc] initWithString:@", "]];
    
    if (self.sign < 0) {
        [text appendAttributedString:[[NSAttributedString alloc] initWithString:@"−ve" attributes:@{NSForegroundColorAttributeName: LinkViewController.negColour}]];
    } else {
        [text appendAttributedString:[[NSAttributedString alloc] initWithString:@"+ve" attributes:@{NSForegroundColorAttributeName: LinkViewController.posColour}]];
    }
    
    return text;
}
@end

#pragma mark - R1 down

@interface R1DownArg : NSObject
@property (assign, nonatomic) regina::Crossing* crossing;
- (id)init:(regina::Crossing*)crossing;
- (NSString*)display;
@end

@implementation R1DownArg
- (id)init:(regina::Crossing*)crossing
{
    self = [super init];
    if (self) {
        _crossing = crossing;
    }
    return self;
}
- (NSString*)display
{
    return [NSString stringWithFormat:@"Crossing %d", self.crossing->index()];
}
@end

#pragma mark - R2 up

@interface R2UpArg : NSObject
@property (assign, nonatomic) regina::StrandRef strand;
@property (assign, nonatomic) int side;
- (id)init:(const regina::StrandRef&)strand side:(int)side;
- (NSAttributedString*)display:(int)position;
@end

@implementation R2UpArg
- (id)init:(const regina::StrandRef&)strand side:(int)side
{
    self = [super init];
    if (self) {
        _strand = strand;
        _side = side;
    }
    return self;
}
- (NSAttributedString*)display:(int)position
{
    NSMutableAttributedString* text = [[NSMutableAttributedString alloc] init];
    
    if (position > 0) {
        [text appendAttributedString:[[NSAttributedString alloc] initWithString:@"Over:" attributes:@{NSUnderlineStyleAttributeName: @1}]];
    } else {
        [text appendAttributedString:[[NSAttributedString alloc] initWithString:@"Under:" attributes:@{NSUnderlineStyleAttributeName: @1}]];
    }
    [text appendAttributedString:[[NSAttributedString alloc] initWithString:@" "]];

    if (self.strand.crossing()) {
        regina::StrandRef s = self.strand;
        [text appendAttributedString:[[NSAttributedString alloc] initWithString:strandDesc(s)]];
        [text appendAttributedString:[[NSAttributedString alloc] initWithString:@" → "]];
        ++s;
        [text appendAttributedString:[[NSAttributedString alloc] initWithString:strandDesc(s)]];
    } else {
        if (position > 0)
            [text appendAttributedString:[[NSAttributedString alloc] initWithString:@"Unknotted circle #1"]];
        else
            [text appendAttributedString:[[NSAttributedString alloc] initWithString:@"Unknotted circle #2"]];
    }
    
    [text appendAttributedString:[[NSAttributedString alloc] initWithString:@", "]];
    
    if (self.side == 0)
        [text appendAttributedString:[[NSAttributedString alloc] initWithString:@"left" attributes:@{NSForegroundColorAttributeName: LinkViewController.leftColour}]];
    else
        [text appendAttributedString:[[NSAttributedString alloc] initWithString:@"right" attributes:@{NSForegroundColorAttributeName: LinkViewController.rightColour}]];

    return text;
}
@end

#pragma mark - R2 down

@interface R2DownArg : NSObject {
    int _displayCrossing[2];
}
@property (assign, nonatomic, readonly) regina::Crossing* crossing;

- (id)init:(regina::Crossing*)crossing;
- (NSString*)display;
- (NSComparisonResult)compare:(R2DownArg*)otherObject;
@end

@implementation R2DownArg
- (id)init:(regina::Crossing*)crossing
{
    self = [super init];
    if (self) {
        _crossing = crossing;
        
        _displayCrossing[0] = crossing->index();
        _displayCrossing[1] = crossing->next(1).crossing()->index();
        
        std::sort(_displayCrossing, _displayCrossing + 2);
    }
    return self;
}
- (NSString*)display
{
    return [NSString stringWithFormat:@"Crossings %d, %d",
            _displayCrossing[0], _displayCrossing[1]];
}
- (NSComparisonResult)compare:(R2DownArg*)rhs
{
    if (_displayCrossing[0] < rhs->_displayCrossing[0])
        return NSOrderedAscending;
    if (_displayCrossing[0] > rhs->_displayCrossing[0])
        return NSOrderedDescending;
    if (_displayCrossing[1] < rhs->_displayCrossing[1])
        return NSOrderedAscending;
    if (_displayCrossing[1] > rhs->_displayCrossing[1])
        return NSOrderedDescending;
    return NSOrderedSame;
}
@end

#pragma mark - R3

@interface R3Arg : NSObject {
    int _displayCrossing[3];
}
@property (assign, nonatomic, readonly) regina::Crossing* crossing;
@property (assign, nonatomic, readonly) int side;

- (id)init:(regina::Crossing*)crossing side:(int)side;
- (NSString*)display;
- (NSComparisonResult)compare:(R3Arg*)otherObject;
@end

@implementation R3Arg
- (id)init:(regina::Crossing*)crossing side:(int)side
{
    self = [super init];
    if (self) {
        _crossing = crossing;
        _side = side;
        
        regina::Crossing* c2 = crossing->upper().next().crossing();
        // The upper arc of the move is crossing -> c2 in the forward direction.
        regina::Crossing* c3;
        if ((side == 0 && c2->sign() > 0) || (side == 1 && c2->sign() < 0))
            c3 = c2->lower().next().crossing();
        else
            c3 = c2->lower().prev().crossing();

        _displayCrossing[0] = crossing->index();
        _displayCrossing[1] = c2->index();
        _displayCrossing[2] = c3->index();
        
        std::sort(_displayCrossing, _displayCrossing + 3);
    }
    return self;
}
- (NSString*)display
{
    return [NSString stringWithFormat:@"Crossings %d, %d, %d",
            _displayCrossing[0], _displayCrossing[1], _displayCrossing[2]];
}
- (NSComparisonResult)compare:(R3Arg*)rhs
{
    if (_displayCrossing[0] < rhs->_displayCrossing[0])
        return NSOrderedAscending;
    if (_displayCrossing[0] > rhs->_displayCrossing[0])
        return NSOrderedDescending;
    if (_displayCrossing[1] < rhs->_displayCrossing[1])
        return NSOrderedAscending;
    if (_displayCrossing[1] > rhs->_displayCrossing[1])
        return NSOrderedDescending;
    if (_displayCrossing[2] < rhs->_displayCrossing[2])
        return NSOrderedAscending;
    if (_displayCrossing[2] > rhs->_displayCrossing[2])
        return NSOrderedDescending;
    return NSOrderedSame;
}
@end

#pragma mark - Link moves

@interface LinkMoves () {
    NSMutableArray* options1up;
    NSMutableArray* options1down;
    NSMutableArray* options2upOver;
    NSMutableArray* options2upUnder;
    NSMutableArray* options2down;
    NSMutableArray* options3;

    NSAttributedString* unavailable;
}

@property (weak, nonatomic) IBOutlet UIButton *button1up;
@property (weak, nonatomic) IBOutlet UIButton *button1down;
@property (weak, nonatomic) IBOutlet UIButton *button2up;
@property (weak, nonatomic) IBOutlet UIButton *button2down;
@property (weak, nonatomic) IBOutlet UIButton *button3;

@property (weak, nonatomic) IBOutlet UIStepper *stepper1up;
@property (weak, nonatomic) IBOutlet UIStepper *stepper1down;
@property (weak, nonatomic) IBOutlet UIStepper *stepper2upOver;
@property (weak, nonatomic) IBOutlet UIStepper *stepper2upUnder;
@property (weak, nonatomic) IBOutlet UIStepper *stepper2down;
@property (weak, nonatomic) IBOutlet UIStepper *stepper3;

@property (weak, nonatomic) IBOutlet UILabel *detail1up;
@property (weak, nonatomic) IBOutlet UILabel *detail1down;
@property (weak, nonatomic) IBOutlet UILabel *detail2upOver;
@property (weak, nonatomic) IBOutlet UILabel *detail2upUnder;
@property (weak, nonatomic) IBOutlet UILabel *detail2down;
@property (weak, nonatomic) IBOutlet UILabel *detail3;

@property (weak, nonatomic) IBOutlet UILabel *crossings;

@end

@implementation LinkMoves

- (void)viewWillAppear:(BOOL)animated
{
    unavailable = [TextHelper dimString:@"Not available"];
    [self reloadMoves];
}

- (void)reloadMoves
{
    if (self.packet->size() == 1)
        self.crossings.text = @"1 crossing";
    else
        self.crossings.text = [NSString stringWithFormat:@"%zu crossings", self.packet->size()];

    unsigned long i;
    int strand, side;
    
    // R1 twist moves on arcs are always valid.
    options1up = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->size(); ++i)
        for (strand = 0; strand < 2; ++strand)
            for (side = 0; side < 2; ++side) {
                [options1up addObject:[[R1UpArg alloc] init:self.packet->crossing(i)->strand(strand)
                                                       side:side
                                                       sign:1]];
                [options1up addObject:[[R1UpArg alloc] init:self.packet->crossing(i)->strand(strand)
                                                       side:side
                                                       sign:-1]];
            }
    if (self.packet->r1(regina::StrandRef(nullptr, 0), 0, 1, true, false)) {
        // We have unknot component(s) that we can use for R1 twists also.
        [options1up addObject:[[R1UpArg alloc] init:regina::StrandRef(nullptr, 0)
                                               side:0
                                               sign:1]];
        [options1up addObject:[[R1UpArg alloc] init:regina::StrandRef(nullptr, 0)
                                               side:0
                                               sign:-1]];
    }
    if (options1up.count > 0) {
        self.button1up.enabled = self.stepper1up.enabled = YES;
        self.stepper1up.maximumValue = options1up.count - 1;
        if (self.stepper1up.value >= options1up.count)
            self.stepper1up.value = options1up.count - 1;
        else
            [self changedR1Up:nil];
    } else {
        self.button1up.enabled = self.stepper1up.enabled = NO;
        self.detail1up.attributedText = unavailable;
    }

    options1down = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->size(); ++i)
        if (self.packet->r1(self.packet->crossing(i), true, false))
            [options1down addObject:[[R1DownArg alloc] init:self.packet->crossing(i)]];
    if (options1down.count > 0) {
        self.button1down.enabled = self.stepper1down.enabled = YES;
        self.stepper1down.maximumValue = options1down.count - 1;
        if (self.stepper1down.value >= options1down.count)
            self.stepper1down.value = options1down.count - 1;
        else
            [self changedR1Down:nil];
    } else {
        self.button1down.enabled = self.stepper1down.enabled = NO;
        self.detail1down.attributedText = unavailable;
    }
    
    // R2 overlap moves can be done with any arc that is not the inside of a twist.
    // Note that, if you are the inside of a twist, then you cannot also be the outside of a twist.
    options2upOver = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->size(); ++i) {
        regina::Crossing* crossing = self.packet->crossing(i);
        for (strand = 0; strand < 2; ++strand) {
            if (crossing->next(strand).crossing() == crossing) {
                // We are part of a twist.
                if ((strand == 0 && crossing->sign() > 0) || (strand == 1 && crossing->sign() < 0)) {
                    // Left side is bad.
                    [options2upOver addObject:[[R2UpArg alloc] init:crossing->strand(strand)
                                                               side:1]];
                } else {
                    // Right side is bad.
                    [options2upOver addObject:[[R2UpArg alloc] init:crossing->strand(strand)
                                                               side:0]];
                }
            } else {
                // We are not part of a twist.
                // Both sides are usable.
                for (side = 0; side < 2; ++side) {
                    [options2upOver addObject:[[R2UpArg alloc] init:crossing->strand(strand)
                                                               side:side]];
                }
            }
        }
    }
    if (self.packet->countComponents() > 1 && self.packet->r1(regina::StrandRef(nullptr, 0), 0, 1, true, false)) {
        // We have unknot component(s), as identified by the R1 test, and we
        // can use R2 on this with any *different* component.
        [options2upOver addObject:[[R2UpArg alloc] init:regina::StrandRef(nullptr, 0)
                                                   side:0]];
        [options2upOver addObject:[[R2UpArg alloc] init:regina::StrandRef(nullptr, 0)
                                                   side:1]];
    }
    if (options2upOver.count > 0) {
        self.button2up.enabled = self.stepper2upOver.enabled = self.stepper2upUnder.enabled = YES;
        self.stepper2upOver.maximumValue = options2upOver.count - 1;
        if (self.stepper2upOver.value >= options2upOver.count)
            self.stepper2upOver.value = options2upOver.count - 1;
        else
            [self changedR2UpOver:nil];
    } else {
        self.button2up.enabled = self.stepper2upOver.enabled = self.stepper2upUnder.enabled = NO;
        self.detail2upOver.attributedText = self.detail2upUnder.attributedText = unavailable;
    }

    options2down = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->size(); ++i)
        if (self.packet->r2(self.packet->crossing(i), true, false))
            [options2down addObject:[[R2DownArg alloc] init:self.packet->crossing(i)]];
    if (options2down.count > 0) {
        [options2down sortUsingSelector:@selector(compare:)];
        self.button2down.enabled = self.stepper2down.enabled = YES;
        self.stepper2down.maximumValue = options2down.count - 1;
        if (self.stepper2down.value >= options2down.count)
            self.stepper2down.value = options2down.count - 1;
        else
            [self changedR2Down:nil];
    } else {
        self.button2down.enabled = self.stepper2down.enabled = NO;
        self.detail2down.attributedText = unavailable;
    }
    
    options3 = [[NSMutableArray alloc] init];
    for (i = 0; i < self.packet->size(); ++i)
        for (side = 0; side < 2; ++side)
            if (self.packet->r3(self.packet->crossing(i), side, true, false))
                [options3 addObject:[[R3Arg alloc] init:self.packet->crossing(i) side:side]];
    if (options3.count > 0) {
        [options3 sortUsingSelector:@selector(compare:)];
        self.button3.enabled = self.stepper3.enabled = YES;
        self.stepper3.maximumValue = options3.count - 1;
        if (self.stepper3.value >= options3.count)
            self.stepper3.value = options3.count - 1;
        else
            [self changedR3:nil];
    } else {
        self.button3.enabled = self.stepper3.enabled = NO;
        self.detail3.attributedText = unavailable;
    }
}

- (IBAction)doR1Up:(id)sender
{
    NSInteger use = self.stepper1up.value;
    if (use < 0 || use >= options1up.count) {
        NSLog(@"Invalid R1 twist stepper value: %ld", (long)use);
        return;
    }
    R1UpArg* arg = (R1UpArg*)options1up[use];
    self.packet->r1(arg.strand, arg.side, arg.sign, true, true);
    [self reloadMoves];
}

- (IBAction)doR1Down:(id)sender
{
    NSInteger use = self.stepper1down.value;
    if (use < 0 || use >= options1down.count) {
        NSLog(@"Invalid R1 untwist stepper value: %ld", (long)use);
        return;
    }
    R1DownArg* arg = (R1DownArg*)options1down[use];
    self.packet->r1(arg.crossing, true, true);
    [self reloadMoves];
}

- (IBAction)doR2Up:(id)sender
{
    NSInteger useOver = self.stepper2upOver.value;
    if (useOver < 0 || useOver >= options2upOver.count) {
        NSLog(@"Invalid R2 overlap (over) stepper value: %ld", (long)useOver);
        return;
    }
    R2UpArg* over = (R2UpArg*)options2upOver[useOver];

    NSInteger useUnder = self.stepper2upUnder.value;
    if (useUnder < 0 || useUnder >= options2upUnder.count) {
        NSLog(@"Invalid R2 overlap (under) stepper value: %ld", (long)useUnder);
        return;
    }
    R2UpArg* under = (R2UpArg*)options2upUnder[useUnder];

    self.packet->r2(over.strand, over.side, under.strand, under.side, true, true);
    [self reloadMoves];
}

- (IBAction)doR2Down:(id)sender
{
    NSInteger use = self.stepper2down.value;
    if (use < 0 || use >= options2down.count) {
        NSLog(@"Invalid R2 separate stepper value: %ld", (long)use);
        return;
    }
    R2DownArg* arg = (R2DownArg*)options2down[use];
    self.packet->r2(arg.crossing, true, true);
    [self reloadMoves];
}

- (IBAction)doR3:(id)sender
{
    NSInteger use = self.stepper3.value;
    if (use < 0 || use >= options3.count) {
        NSLog(@"Invalid R3 stepper value: %ld", (long)use);
        return;
    }
    R3Arg* arg = (R3Arg*)options3[use];
    self.packet->r3(arg.crossing, arg.side, true, true);
    [self reloadMoves];
}

- (IBAction)changedR1Up:(id)sender
{
    NSInteger use = self.stepper1up.value;
    if (use < 0 || use >= options1up.count) {
        self.detail1up.attributedText = [TextHelper badString:@"Invalid R1 arguments"];
        return;
    }
    R1UpArg* arg = (R1UpArg*)options1up[use];
    self.detail1up.attributedText = arg.display;
}

- (IBAction)changedR1Down:(id)sender
{
    NSInteger use = self.stepper1down.value;
    if (use < 0 || use >= options1down.count) {
        NSLog(@"Invalid R1 untwist stepper value: %ld", (long)use);
        return;
    }
    R1DownArg* arg = (R1DownArg*)options1down[use];
    self.detail1down.attributedText = [[NSAttributedString alloc]
                                       initWithString:arg.display];
}

- (IBAction)changedR2UpOver:(id)sender
{
    NSInteger use = self.stepper2upOver.value;
    if (use < 0 || use >= options2upOver.count) {
        NSLog(@"Invalid R2 overlap (over) stepper value: %ld", (long)use);
        return;
    }
    R2UpArg* over = (R2UpArg*)options2upOver[use];
    self.detail2upOver.attributedText = [over display:1];
    
    // TODO: Make this faster by walking around the region (and then sorting),
    // instead of iterating through all potential strands.
    
    options2upUnder = [[NSMutableArray alloc] init];
    int i, strand, side;
    for (i = 0; i < self.packet->size(); ++i)
        for (strand = 0; strand < 2; ++strand)
            for (side = 0; side < 2; ++side)
                if (self.packet->r2(over.strand, over.side,
                                    self.packet->crossing(i)->strand(strand), side,
                                    true, false))
                    [options2upUnder addObject:[[R2UpArg alloc] init:self.packet->crossing(i)->strand(strand) side:side]];
    for (side = 0; side < 2; ++side)
        if (self.packet->r2(over.strand, over.side,
                            regina::StrandRef(nullptr, 0), side,
                            true, false))
            [options2upUnder addObject:[[R2UpArg alloc] init:regina::StrandRef(nullptr, 0) side:side]];
    if (options2upUnder.count > 0) {
        // [options2upUnder sortUsingSelector:@selector(compare:)];
        self.stepper2upUnder.maximumValue = options2upUnder.count - 1;
        if (self.stepper2upUnder.value >= options2upUnder.count)
            self.stepper2upUnder.value = options2upUnder.count - 1;
        else
            [self changedR2UpUnder:nil];
    } else {
        // This should never happen.
        self.button2up.enabled = self.stepper2upOver.enabled = self.stepper2upUnder.enabled = NO;
        self.detail2upOver.attributedText = self.detail2upUnder.attributedText = unavailable;
        NSLog(@"ERROR: No viable options for R2 up (under).");
    }
}

- (IBAction)changedR2UpUnder:(id)sender
{
    NSInteger use = self.stepper2upUnder.value;
    if (use < 0 || use >= options2upUnder.count) {
        NSLog(@"Invalid R2 overlap (under) stepper value: %ld", (long)use);
        return;
    }
    R2UpArg* arg = (R2UpArg*)options2upUnder[use];
    self.detail2upUnder.attributedText = [arg display:-1];
}

- (IBAction)changedR2Down:(id)sender
{
    NSInteger use = self.stepper2down.value;
    if (use < 0 || use >= options2down.count) {
        NSLog(@"Invalid R2 separate stepper value: %ld", (long)use);
        return;
    }
    R2DownArg* arg = (R2DownArg*)options2down[use];
    self.detail2down.attributedText = [[NSAttributedString alloc]
                                       initWithString:arg.display];
}

- (IBAction)changedR3:(id)sender
{
    NSInteger use = self.stepper3.value;
    if (use < 0 || use >= options3.count) {
        NSLog(@"Invalid R3 stepper value: %ld", (long)use);
        return;
    }
    R3Arg* arg = (R3Arg*)options3[use];
    self.detail3.attributedText = [[NSAttributedString alloc]
                                   initWithString:arg.display];
}

- (IBAction)close:(id)sender
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

@end
