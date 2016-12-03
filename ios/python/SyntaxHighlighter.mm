
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  Parts of this file are modified from the KDE syntax-highlighting      *
 *  framework, which is copyright (C) 2016 Volker Krause <vkrause@kde.org>*
 *  and is distributed under the GNU Library General Public License as    *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *
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

#import "NSMatcher.h"
#import "SyntaxHighlighter.h"
#import "syntax/context.h"
#import "syntax/definition_p.h"
#import "syntax/format.h"
#import "syntax/repository.h"
#import "syntax/rule.h"
#import "syntax/state.h"
#import "syntax/state_p.h"
#import "syntax/theme.h"

@implementation SyntaxHighlighter {
    UIFont* regular;
    UIFont* bold;
    UIFont* italic;
}

- (id)init
{
    self = [super init];
    if (self) {
        // TODO: Pass the UITextView as an argument, and extract the font size from that.
        regular = [UIFont fontWithName:@"Menlo" size:12];
        bold = [UIFont fontWithName:@"Menlo-Bold" size:12];
        italic = [UIFont fontWithName:@"Menlo-Italic" size:12];
    }
    return self;
}

- (void)ensureDefinitionLoaded
{
    auto defData = regina::syntax::DefinitionData::get(self.definition);
    if (!self.definition.isValid() && defData->repo && !self.definition.name().empty()) {
        NSLog(@"Definition became invalid, trying re-lookup.");
        self.definition = defData->repo->definitionForName(self.definition.name());
        defData = regina::syntax::DefinitionData::get(self.definition);
    }

    if (!defData->repo && !defData->name.empty())
        NSLog(@"Repository got deleted while a highlighter is still active!");

    if (self.definition.isValid())
        defData->load();
}

/**
 * Returns the index of the first non-space character. If the line is empty
 * or only contains whitespace, toOffset is returned.
 */
+ (int)firstNonSpaceChar:(NSString*)text fromOffset:(NSInteger)fromOffset toOffset:(NSInteger)toOffset
{
    for (NSInteger i = fromOffset; i < toOffset; ++i)
        if (! [[NSCharacterSet whitespaceAndNewlineCharacterSet] characterIsMember:[text characterAtIndex:i]])
            return i;

    return toOffset;
}

+ (bool)switchContext:(const regina::syntax::ContextSwitch&)contextSwitch state:(regina::syntax::StateData*)data
{
    for (int i = 0; i < contextSwitch.popCount(); ++i) {
        // don't pop the last context if we can't push one
        if (data->size() == 1 && !contextSwitch.context())
            return false;
        if (data->size() == 0)
            break;
        data->pop();
    }

    if (contextSwitch.context())
        data->push(contextSwitch.context());

    assert(!data->isEmpty());
    return true;
}

- (regina::syntax::State)highlightLine:(NSTextStorage*)textStorage fromOffset:(NSInteger)fromOffset toOffset:(NSInteger)toOffset state:(regina::syntax::State)state
{
    // verify/initialize state
    auto defData = regina::syntax::DefinitionData::get(self.definition);
    regina::syntax::State newState = state.copy();
    auto stateData = regina::syntax::StateData::get(newState);
    if (stateData->m_defData && defData != stateData->m_defData) {
        NSLog(@"Got invalid state, resetting.");
        stateData->clear();
    }
    if (stateData->isEmpty()) {
        stateData->push(defData->initialContext());
        stateData->m_defData = defData;
    }

    NSString* text = [textStorage.string substringToIndex:toOffset];

    // process empty lines
    if (fromOffset == toOffset) {
        while (!stateData->topContext()->lineEmptyContext().isStay())
            [SyntaxHighlighter switchContext:stateData->topContext()->lineEmptyContext() state:stateData];
        return newState;
    }

    assert(!stateData->isEmpty());
    int firstNonSpace = [SyntaxHighlighter firstNonSpaceChar:text fromOffset:fromOffset toOffset:toOffset];
    int offset = fromOffset, beginOffset = fromOffset;
    auto currentLookupContext = stateData->topContext();
    auto currentFormat = currentLookupContext->attribute();
    bool lineContinuation = false;
    std::map<regina::syntax::Rule*, int> skipOffsets;

    do {
        bool isLookAhead = false;
        int newOffset = fromOffset;
        std::string newFormat;
        auto newLookupContext = currentLookupContext;
        for (const auto &rule : stateData->topContext()->rules()) {
            auto it = skipOffsets.find(rule.get());
            if (it != skipOffsets.end() && it->second > offset)
                continue;

            // filter out rules that only match for leading whitespace
            if (rule->firstNonSpace() && (offset > firstNonSpace)) {
                continue;
            }

            // filter out rules that require a specific column
            if ((rule->requiredColumn() >= 0) && (rule->requiredColumn() != offset - fromOffset)) {
                continue;
            }

            NSMatcher m(text);
            const auto newResult = rule->match(m, offset);
            newOffset = newResult.offset();
            if (newResult.skipOffset() > newOffset)
                skipOffsets.insert(std::make_pair(rule.get(), newResult.skipOffset()));
            if (newOffset <= offset)
                continue;

            if (rule->isLookAhead()) {
                assert(!rule->context().isStay());
                [SyntaxHighlighter switchContext:rule->context() state:stateData];
                isLookAhead = true;
                break;
            }

            newLookupContext = stateData->topContext();
            [SyntaxHighlighter switchContext:rule->context() state:stateData];
            newFormat = rule->attribute().empty() ? stateData->topContext()->attribute() : rule->attribute();
            if (newOffset == toOffset && std::dynamic_pointer_cast<regina::syntax::LineContinue>(rule))
                lineContinuation = true;
            break;
        }
        if (isLookAhead)
            continue;

        if (newOffset <= offset) { // no matching rule
            if (stateData->topContext()->fallthrough()) {
                [SyntaxHighlighter switchContext:stateData->topContext()->fallthroughContext() state:stateData];
                continue;
            }

            newOffset = offset + 1;
            newLookupContext = stateData->topContext();
            newFormat = newLookupContext->attribute();
        }

        if (newFormat != currentFormat /*|| currentLookupDef != newLookupDef*/) {
            if (offset > fromOffset)
                [self applyFormat:currentLookupContext->formatByName(currentFormat)
                      textStorage:textStorage
                           offset:beginOffset
                           length:(offset - beginOffset)];
            beginOffset = offset;
            currentFormat = newFormat;
            currentLookupContext = newLookupContext;
        }
        assert(newOffset > offset);
        offset = newOffset;

    } while (offset < toOffset);

    if (beginOffset < offset)
        [self applyFormat:currentLookupContext->formatByName(currentFormat)
              textStorage:textStorage
                   offset:beginOffset
                   length:(toOffset - beginOffset)];

    while (!stateData->topContext()->lineEndContext().isStay() && !lineContinuation) {
        if (! [SyntaxHighlighter switchContext:stateData->topContext()->lineEndContext() state:stateData])
            break;
    }

    return newState;
}

- (void)textStorage:(NSTextStorage *)textStorage willProcessEditing:(NSTextStorageEditActions)editedMask range:(NSRange)editedRange changeInLength:(NSInteger)delta
{
    NSLog(@"willProcessEditing: range %d(%d), delta %d", editedRange.location, editedRange.length, delta);

    // Currently, we highlight the *entire* text all at once.
    // This is fine for now, where syntax highlighting is only used for read-only script packets.
    // Once we start allowing users to edit scripts, this may need to change...

    // verify definition, deal with no highlighting being enabled
    [self ensureDefinitionLoaded];
    if (!self.definition.isValid()) {
        [self applyFormat:regina::syntax::Format() textStorage:textStorage offset:0 length:textStorage.length];
        return;
    }

    // Highlight each line separately.
    regina::syntax::State state;

    NSCharacterSet* newlines = [NSCharacterSet newlineCharacterSet];
    NSInteger from = 0;
    NSInteger to;
    while (true) {
        while (from < textStorage.length && [newlines characterIsMember:[textStorage.string characterAtIndex:from]])                                             ++from;
        if (from >= textStorage.length)
            break;

        to = from + 1;
        while (to < textStorage.length && ! [newlines characterIsMember:[textStorage.string characterAtIndex:to]])
            ++to;

        state = [self highlightLine:textStorage fromOffset:from toOffset:to state:state];

        from = to + 1;
    }
}

- (void)applyFormat:(const regina::syntax::Format&)format textStorage:(NSTextStorage*)textStorage offset:(NSInteger)offset length:(NSInteger)length
{
    if (length == 0)
        return;

    NSRange r = NSMakeRange(offset, length);

    // TODO: Allow both bold and italic.
    // First set the font and clear all other attributes.
    if (format.isBold(self.theme))
        [textStorage setAttributes:@{NSFontAttributeName: bold}
                             range:r];
    else if (format.isItalic(self.theme))
        [textStorage setAttributes:@{NSFontAttributeName: italic}
                             range:r];
    else
        [textStorage setAttributes:@{NSFontAttributeName: regular}
                             range:r];

    if (format.hasTextColor(self.theme)) {
        unsigned rgb = format.textColor(self.theme);
        [textStorage addAttribute:NSForegroundColorAttributeName
                            value:[UIColor colorWithRed:((rgb >> 16) & 0xff) / 256.0
                                                  green:((rgb >> 8) & 0xff) / 256.0
                                                   blue:(rgb & 0xff) / 256.0
                                                  alpha:1.0]
                            range:r];
    }

    if (format.hasBackgroundColor(self.theme)) {
        unsigned rgb = format.backgroundColor(self.theme);
        [textStorage addAttribute:NSBackgroundColorAttributeName
                            value:[UIColor colorWithRed:((rgb >> 16) & 0xff) / 256.0
                                                  green:((rgb >> 8) & 0xff) / 256.0
                                                   blue:(rgb & 0xff) / 256.0
                                                  alpha:1.0]
                            range:r];
    }
    
    if (format.isUnderline(self.theme))
        [textStorage addAttribute:NSUnderlineStyleAttributeName
                            value:[NSNumber numberWithInteger:NSUnderlineStyleSingle]
                            range:r];

    if (format.isStrikeThrough(self.theme))
        [textStorage addAttribute:NSStrikethroughStyleAttributeName
                            value:[NSNumber numberWithInteger:NSUnderlineStyleSingle]
                            range:r];
}

@end
