
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

#import "PythonHighlighter.h"

static NSArray* keywords = @[
    @"and", @"assert", @"break", @"class", @"continue", @"def", @"del", @"elif",
    @"else", @"except", @"exec", @"finally", @"for", @"from", @"global", @"if",
    @"import", @"in", @"is", @"lambda", @"not", @"or", @"pass", @"print",
    @"raise", @"return", @"try", @"while", @"yield", @"None", @"True", @"False"];

static NSArray* operators = @[
    @"=", @"==", @"!=", @"<", @"<=", @">", @">=",
    @"\\+", @"-", @"\\*", @"/", @"//", @"%", @"\\*\\*",
    @"\\+=", @"-=", @"\\*=", @"/=", @"%=",
    @"\\^", @"\\|", @"&", @"~", @">>", @"<<"];

static NSArray* braces = @[
    @"{", @"}", @"\\(", @"\\)", @"\\[", @"]"];

static NSDictionary* basicStyles = @{
                                     @"keyword" : @[],
                                     @"operator" : @[],
                                     @"brace" : @[],
                                     @"defclass" : @[],
                                     @"string" : @[],
                                     @"string2" : @[],
                                     @"comment" : @[],
                                     @"self" : @[],
                                     @"numbers" : @[]
    };


static UIColor* darkerGoldenrod = [UIColor colorWithRed:(0x80 / 256.0) green:(0x5E / 256.0) blue:(0x08 / 256.0) alpha:1.0];



@implementation PythonHighlighter {
    UIFont* regular;
    UIFont* bold;
    UIFont* italic;
}

- (id)init
{
    self = [super init];
    if (self) {
        // TODO: Pass the UITextView as an argument, and extract the font size from that.
        regular = [UIFont fontWithName:@"Menlo" size:14];
        bold = [UIFont fontWithName:@"Menlo-Bold" size:14];
        italic = [UIFont fontWithName:@"Menlo-Italic" size:14];
    }
    return self;
}

- (void)textStorage:(NSTextStorage *)textStorage willProcessEditing:(NSTextStorageEditActions)editedMask range:(NSRange)editedRange changeInLength:(NSInteger)delta
{
    NSLog(@"willProcessEditing: range %d(%d), delta %d", editedRange.location, editedRange.length, delta);

    // By default, everything should be in a fixed-width font.
    [textStorage addAttribute:NSFontAttributeName value:regular range:editedRange];

    // Highlight what needs to be highlighted.
    NSString* text = textStorage.string;

    [text enumerateSubstringsInRange:NSMakeRange(0, text.length)
                             options:NSStringEnumerationByWords
                          usingBlock:^(NSString* substring, NSRange substringRange, NSRange, BOOL*) {
                              if ([substring isEqual:@"homology"])
                                  [textStorage addAttribute:NSForegroundColorAttributeName value:darkerGoldenrod range:substringRange];
                          }];
}

@end
