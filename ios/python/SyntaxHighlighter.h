
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

#import <Foundation/Foundation.h>
#import "syntax/definition.h"
#import "syntax/theme.h"

/**
 * A delegate for use with text kit classes (such as UITextView) that
 * performs syntax highlighting.
 *
 * To use this, create a new SyntaxHighlighter in your view controller,
 * set its \a definition and \a theme properties, and then set the
 * SyntaxHighlighter as <tt>textView.textStorage.delegate</tt>, where
 * \a textView is the corresponding text view (e.g., the relevant UITextView).
 *
 * The view controller should hold this syntax highlighter strongly.
 */
@interface SyntaxHighlighter : NSObject <NSTextStorageDelegate>

/**
 * The syntax highlighting rules for the particular language to be recognised.
 * For example, when used with script packets, this should contain the
 * rules for highlighting Python.
 */
@property (assign, nonatomic) regina::syntax::Definition definition;

/**
 * The theme that describes which visual elements (fonts, colours, etc.)
 * should be used to display which elements of the underlying language.
 */
@property (assign, nonatomic) regina::syntax::Theme theme;

/**
 * Initialises a new syntax highlighter.
 */
- (id)init;

/**
 * Performs the syntax highlighting through this delegate method.
 */
- (void)textStorage:(NSTextStorage *)textStorage willProcessEditing:(NSTextStorageEditActions)editedMask range:(NSRange)editedRange changeInLength:(NSInteger)delta;

@end
