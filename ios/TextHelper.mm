
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

#import "TextHelper.h"

static UIColor* darkGoldenrod = [UIColor colorWithRed:(0xB8 / 256.0) green:(0x86 / 256.0) blue:(0x0B / 256.0) alpha:1.0];
static UIColor* darkerGoldenrod = [UIColor colorWithRed:(0x80 / 256.0) green:(0x5E / 256.0) blue:(0x08 / 256.0) alpha:1.0];
static UIColor* yesColour = [UIColor colorWithRed:0.0 green:0.5 blue:0.0 alpha:1.0];
static UIColor* noColour = [UIColor colorWithRed:0.6 green:0.0 blue:0.0 alpha:1.0];
static UIColor* badColour = [UIColor colorWithRed:0.6 green:0.0 blue:0.0 alpha:1.0];
static UIColor* dimColour = [UIColor colorWithRed:0.5 green:0.5 blue:0.5 alpha:1.0];
static UIColor* altColour1 = [UIColor colorWithRed:(0x2B / 256.0) green:(0x54 / 256.0) blue:(0x7E / 256.0) alpha:1.0]; // Blue jay: 2B547E
// static UIColor* altColour1 = [UIColor colorWithRed:(0x15 / 256.0) green:(0x1B / 256.0) blue:(0x54 / 256.0) alpha:1.0]; // Midnight blue: 151B54
static UIColor* altColour2 = [UIColor colorWithRed:(0x73 / 256.0) green:(0x7C / 256.0) blue:(0xA1 / 256.0) alpha:1.0]; // Slate blue: 737CA1
// static UIColor* altColour2 = [UIColor colorWithRed:(0x48 / 256.0) green:(0x63 / 256.0) blue:(0xA0 / 256.0) alpha:1.0]; // Steel blue: 4863A0

@implementation TextHelper

+ (NSAttributedString *)plainString:(NSString *)text
{
    return [[NSAttributedString alloc] initWithString:text attributes:@{NSForegroundColorAttributeName: [UIColor blackColor]}];
}

+ (NSAttributedString *)yesNoString:(NSString *)text yesNo:(BOOL)yesNo
{
    return [[NSAttributedString alloc] initWithString:text attributes:@{NSForegroundColorAttributeName: (yesNo ? yesColour : noColour)}];
}

+ (NSAttributedString *)yesNoString:(BOOL)yesNo yes:(NSString *)yes no:(NSString *)no
{
    return (yesNo ?
            [[NSAttributedString alloc] initWithString:yes attributes:@{NSForegroundColorAttributeName: yesColour}] :
            [[NSAttributedString alloc] initWithString:no attributes:@{NSForegroundColorAttributeName: noColour}]);
}

+ (NSAttributedString *)markedString:(NSString *)text
{
    return [[NSAttributedString alloc] initWithString:text attributes:@{NSForegroundColorAttributeName: darkGoldenrod}];
}

+ (NSAttributedString *)badString:(NSString *)text
{
    return [[NSAttributedString alloc] initWithString:text attributes:@{NSForegroundColorAttributeName: badColour}];
}

+ (NSAttributedString *)dimString:(NSString *)text
{
    return [[NSAttributedString alloc] initWithString:text attributes:@{NSForegroundColorAttributeName: dimColour}];
}

+ (NSAttributedString*)altString:(NSString*)text parity:(BOOL)parity
{
    return [[NSAttributedString alloc] initWithString:text attributes:@{NSForegroundColorAttributeName: (parity ? altColour1 : altColour2)}];
}

+ (void)appendToList:(NSMutableString*)str item:(NSString*)item
{
    if (str.length == 0)
        [str appendString:item];
    else
        [str appendFormat:@", %@", item];
}

+ (NSString*)countString:(long)count singular:(const char*)singular plural:(const char*)plural
{
    if (count == 1)
        return [NSString stringWithFormat:@"1 %s", singular];
    else
        return [NSString stringWithFormat:@"%ld %s", count, plural];
}

@end
