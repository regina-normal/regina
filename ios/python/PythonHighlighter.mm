
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

static UIColor* darkMagenta = [UIColor colorWithRed:(0x8b / 256.0) green:(0x00 / 256.0) blue:(0x8b / 256.0) alpha:1.0];
static UIColor* darkGreen = [UIColor colorWithRed:(0x00 / 256.0) green:(0x64 / 256.0) blue:(0x00 / 256.0) alpha:1.0];

@interface HighlightingRule : NSObject

@property (strong, nonatomic) NSRegularExpression* pattern;
@property (assign, nonatomic) NSInteger match;
@property (weak, nonatomic) UIColor* colour;
@property (weak, nonatomic) UIFont* font;

- (id)initWithPattern:(NSString*)p match:(NSInteger)m colour:(UIColor*)c font:(UIFont*)f;
+ (id)ruleWithPattern:(NSString*)p match:(NSInteger)m colour:(UIColor*)c font:(UIFont*)f;

@end

@implementation HighlightingRule

- (id)initWithPattern:(NSString *)p match:(NSInteger)m colour:(UIColor *)c font:(UIFont *)f
{
    self = [super init];
    if (self) {
        _pattern = [NSRegularExpression regularExpressionWithPattern:p options:0 error:nil];
        _match = m;
        _colour = c;
        _font = f;
    }
    return self;
}

+ (id)ruleWithPattern:(NSString *)p match:(NSInteger)m colour:(UIColor *)c font:(UIFont *)f
{
    return [[HighlightingRule alloc] initWithPattern:p match:m colour:c font:f];
}

@end

@implementation PythonHighlighter {
    UIFont* regular;
    UIFont* bold;
    UIFont* italic;
    NSMutableArray* rules;
}

- (void)initRules
{
    rules = [[NSMutableArray alloc] init];

    // Keywords:
    // (Can be overridden by strings and/or comments)
    for (NSString* s in keywords) {
        [rules addObject:[HighlightingRule ruleWithPattern:[NSString stringWithFormat:@"\\b%@\\b", s]
                                                     match:0
                                                    colour:[UIColor blueColor]
                                                      font:nil]];
    }

    // Operators:
    // (Can be overridden by strings and/or comments)
    for (NSString* s in operators) {
        [rules addObject:[HighlightingRule ruleWithPattern:s
                                                     match:0
                                                    colour:[UIColor redColor]
                                                      font:nil]];
    }

    // Braces:
    // (Can be overridden by strings and/or comments)
    for (NSString* s in braces) {
        [rules addObject:[HighlightingRule ruleWithPattern:s
                                                     match:0
                                                    colour:[UIColor darkGrayColor]
                                                      font:nil]];
    }

    // Self:
    // (Can be overridden by strings and/or comments)
    [rules addObject:[HighlightingRule ruleWithPattern:@"\\bself\\b"
                                                 match:0
                                                colour:[UIColor blackColor]
                                                  font:italic]];

    // "def" or "class" followed by an identifier:
    // (Can be overridden by strings and/or comments)
    [rules addObject:[HighlightingRule ruleWithPattern:@"\\bdef\\b\\s*(\\w+)"
                                                 match:1
                                                colour:[UIColor blackColor]
                                                  font:bold]];
    [rules addObject:[HighlightingRule ruleWithPattern:@"\\bclass\\b\\s*(\\w+)"
                                                 match:1
                                                colour:[UIColor blackColor]
                                                  font:bold]];

    // Numeric literals:
    // (Can be overridden by strings and/or comments)
    [rules addObject:[HighlightingRule ruleWithPattern:@"\\b[+-]?[0-9]+[lL]?\\b"
                                                 match:0
                                                colour:[UIColor brownColor]
                                                  font:nil]];
    [rules addObject:[HighlightingRule ruleWithPattern:@"\\b[+-]?0[xX][0-9A-Fa-f]+[lL]?\\b"
                                                 match:0
                                                colour:[UIColor brownColor]
                                                  font:nil]];
    [rules addObject:[HighlightingRule ruleWithPattern:@"\\b[+-]?[0-9]+(?:\\.[0-9]+)?(?:[eE][+-]?[0-9]+)?\\b"
                                                 match:0
                                                colour:[UIColor brownColor]
                                                  font:nil]];

    // Strings, possibly including escape sequences:
    // TODO: I'm not convinced by these regexes (both for iOS and Qt)...
    // TODO: Make sure this is not inside a comment.
    [rules addObject:[HighlightingRule ruleWithPattern:@"\"[^\"\\\\]*(\\\\.[^\"\\\\]*)*\""
                                                 match:0
                                                colour:[UIColor magentaColor]
                                                  font:regular]];
    [rules addObject:[HighlightingRule ruleWithPattern:@"'[^'\\\\]*(\\\\.[^'\\\\]*)*'"
                                                 match:0
                                                colour:[UIColor magentaColor]
                                                  font:regular]];
    [rules addObject:[HighlightingRule ruleWithPattern:@"\"\"\"[^\"\\\\]*(\\\\.[^\"\\\\]*)*\"\"\""
                                                 match:0
                                                colour:darkMagenta
                                                  font:regular]];
    [rules addObject:[HighlightingRule ruleWithPattern:@"'''[^'\\\\]*(\\\\.[^'\\\\]*)*'''"
                                                 match:0
                                                colour:darkMagenta
                                                  font:regular]];

    // Single-line comments:
    // TODO: Make sure this is not inside a string.
    [rules addObject:[HighlightingRule ruleWithPattern:@"#[^\\n]*"
                                                 match:0
                                                colour:darkGreen
                                                  font:italic]];
}

- (id)init
{
    self = [super init];
    if (self) {
        // TODO: Pass the UITextView as an argument, and extract the font size from that.
        regular = [UIFont fontWithName:@"Menlo" size:12];
        bold = [UIFont fontWithName:@"Menlo-Bold" size:12];
        italic = [UIFont fontWithName:@"Menlo-Italic" size:12];

        [self initRules];
    }
    return self;
}

- (void)textStorage:(NSTextStorage *)textStorage willProcessEditing:(NSTextStorageEditActions)editedMask range:(NSRange)editedRange changeInLength:(NSInteger)delta
{
    NSLog(@"willProcessEditing: range %d(%d), delta %d", editedRange.location, editedRange.length, delta);

    // Currently, we highlight the *entire* text all at once.
    // This is fine for now, where syntax highlighting is only used for read-only script packets.
    // Once we start allowing users to edit scripts, this may need to change...

    // By default, everything should be in a fixed-width font.
    [textStorage addAttribute:NSFontAttributeName value:regular range:editedRange];

    // Highlight what needs to be highlighted.
    NSString* text = textStorage.string;
    NSArray<NSTextCheckingResult*>* matches;
    for (HighlightingRule* rule in rules) {
        matches = [rule.pattern matchesInString:text options:0 range:NSMakeRange(0, text.length)];
        for (NSTextCheckingResult* match in matches) {
            NSRange r = [match rangeAtIndex:rule.match];
            if (rule.colour)
                [textStorage addAttribute:NSForegroundColorAttributeName value:rule.colour range:r];
            if (rule.font)
                [textStorage addAttribute:NSFontAttributeName value:rule.font range:r];
        }
    }
}

@end
