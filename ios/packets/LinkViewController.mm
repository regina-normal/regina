
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

#import "LinkViewController.h"
#import "link/link.h"

static UIColor* negColour = [UIColor colorWithRed:(0xB8 / 256.0)
                                            green:(0x86 / 256.0)
                                             blue:(0x0B / 256.0)
                                            alpha:1.0]; // Dark goldenrod

/*
static UIColor* posColour = [UIColor colorWithRed:(0x2B / 256.0)
                                            green:(0x54 / 256.0)
                                             blue:(0x7E / 256.0)
                                            alpha:1.0]; // Blue jay
*/
static UIColor* posColour = [UIColor colorWithRed:(0.0)
                                            green:(0x71 / 256.0)
                                             blue:(0xBC / 256.0)
                                            alpha:1.0];

static UIColor* leftColour = [UIColor colorWithRed:0.6
                                             green:0.0
                                              blue:0.0
                                             alpha:1.0]; // Colour for port

static UIColor* rightColour = [UIColor colorWithRed:0.0
                                              green:0.5
                                               blue:0.0
                                              alpha:1.0]; // Colour for starboard

@implementation LinkViewController

+ (UIColor *)posColour
{
    return posColour;
}

+ (UIColor *)negColour
{
    return negColour;
}

+ (UIColor *)leftColour
{
    return leftColour;
}

+ (UIColor *)rightColour
{
    return rightColour;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    [self setSelectedImages:@[@"Tab-Crossings-Bold",
                              [NSNull null] /* Polynomials */,
                              [NSNull null] /* Codes */,
                              @"Tab-Graph-Bold"]];
    [self registerDefaultKey:@"ViewLinkTab"];
}

- (void)updateHeader:(UILabel *)header
{
    NSAttributedString* signs = nil;
    if (self.packet->size() > 1) {
        size_t plus = 0, minus = 0;
        for (size_t i = 0; i < self.packet->size(); ++i)
            if (self.packet->crossing(i)->sign() > 0)
                ++plus;
            else
                ++minus;
        
        if (minus == 0)
            signs = [[NSAttributedString alloc] initWithString:@"all +ve"
                                                    attributes:@{NSForegroundColorAttributeName: posColour}];
        else if (plus == 0)
            signs = [[NSAttributedString alloc] initWithString:@"all −ve"
                                                    attributes:@{NSForegroundColorAttributeName: negColour}];
        else {
            NSMutableAttributedString* s = [[NSMutableAttributedString alloc]
                                            initWithString:[NSString stringWithFormat:@"%zu +ve", plus]
                                                attributes:@{NSForegroundColorAttributeName: posColour}];
            [s appendAttributedString:[[NSAttributedString alloc] initWithString:@", "]];
            [s appendAttributedString:[[NSAttributedString alloc]
                                       initWithString:[NSString stringWithFormat:@"%zu −ve", minus]
                                       attributes:@{NSForegroundColorAttributeName: negColour}]];
            signs = s;
        }
    } else if (self.packet->size() == 1) {
        if (self.packet->crossing(0)->sign() > 0)
            signs = [[NSAttributedString alloc] initWithString:@"+ve"
                                                    attributes:@{NSForegroundColorAttributeName: posColour}];
        else
            signs = [[NSAttributedString alloc] initWithString:@"−ve"
                                                    attributes:@{NSForegroundColorAttributeName: negColour}];
    }
    
    if (self.packet->isEmpty())
        header.attributedText = [[NSAttributedString alloc] initWithString:@"Empty"];
    else if (self.packet->countComponents() == 1) {
        // Knot:
        if (self.packet->size() == 0) {
            header.attributedText = [[NSAttributedString alloc] initWithString:@"Unknot with no crossings"];
        } else if (self.packet->size() == 1) {
            // Must be alternating.
            NSMutableAttributedString* s = [[NSMutableAttributedString alloc] initWithString:@"Alternating knot with 1 crossing ("];
            [s appendAttributedString:signs];
            [s appendAttributedString:[[NSAttributedString alloc] initWithString:@")"]];
            header.attributedText = s;
        } else {
            NSMutableAttributedString* s;
            if (self.packet->isAlternating())
                s = [[NSMutableAttributedString alloc]
                     initWithString:[NSString stringWithFormat:@"Alternating knot with %zu crossings (", self.packet->size()]];
            else
                s = [[NSMutableAttributedString alloc]
                     initWithString:[NSString stringWithFormat:@"Non-alternating knot with %zu crossings (", self.packet->size()]];
            [s appendAttributedString:signs];
            [s appendAttributedString:[[NSAttributedString alloc] initWithString:@")"]];
            header.attributedText = s;
        }
    } else {
        // Multiple component link:
        if (self.packet->size() == 0) {
            header.attributedText = [[NSAttributedString alloc]
                                     initWithString:[NSString stringWithFormat:@"Unlink with %zu components, no crossings", self.packet->countComponents()]];
        } else if (self.packet->size() == 1) {
            // Must be alternating.
            NSMutableAttributedString* s = [[NSMutableAttributedString alloc]
                                            initWithString:[NSString stringWithFormat:@"Alternating link with %zu components, 1 crossing (", self.packet->countComponents()]];
            [s appendAttributedString:signs];
            [s appendAttributedString:[[NSAttributedString alloc] initWithString:@")"]];
            header.attributedText = s;
        } else {
            NSMutableAttributedString* s;
            if (self.packet->isAlternating())
                s = [[NSMutableAttributedString alloc]
                     initWithString:[NSString stringWithFormat:@"Alternating link with %zu components, %zu crossings (", self.packet->countComponents(), self.packet->size()]];
            else
                s = [[NSMutableAttributedString alloc]
                     initWithString:[NSString stringWithFormat:@"Non-alternating link with %zu components, %zu crossings (", self.packet->countComponents(), self.packet->size()]];
            [s appendAttributedString:signs];
            [s appendAttributedString:[[NSAttributedString alloc] initWithString:@")"]];
            header.attributedText = s;
        }
    }
}

@end
