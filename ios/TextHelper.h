
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

/**
 * Miscellaneous helper routines for manipulating strings.
 */
@interface TextHelper : NSObject

/**
 * Renders the given text in a plain black font.  The font colour is
 * explicitly specified in the resulting attributed string.
 */
+ (NSAttributedString*)plainString:(NSString*)text;

/**
 * Renders the given text in a green "yes" or a red "no" font.
 */
+ (NSAttributedString*)yesNoString:(NSString*)text yesNo:(BOOL)yesNo;

/**
 * According to the given boolean, renders either the text \a yes in a
 * green font, or the text \a no in a red font.
 */
+ (NSAttributedString*)yesNoString:(BOOL)yesNo yes:(NSString*)yes no:(NSString*)no;

/**
 * Renders the given text in a brown font.
 */
+ (NSAttributedString*)markedString:(NSString*)text;

/**
 * Renders the given text in a dark red colour.
 */
+ (NSAttributedString*)badString:(NSString*)text;

/**
 * Renders the given text in a dim grey colour.
 */
+ (NSAttributedString*)dimString:(NSString*)text;

/**
 * Renders the given text in one of two alternating colours, for use with long lists.
 */
+ (NSAttributedString*)altString:(NSString*)text parity:(BOOL)parity;

/**
 * Appends \a item to \a str.
 * If \a str is non-empty then the new text \a item will be
 * separated from the existing text by a comma and a space.
 */
+ (void)appendToList:(NSMutableString*)str item:(NSString*)item;

/**
 * Returns either "\a count \a singular" or "\a count \a plural"
 * according to whether or not \a count is 1.
 *
 * Example: <tt>[TextHelper countString:n singular:"tetrahedra" plural:"tetrahedra"]</tt>
 *
 * The arguments \a singular and \a plural must both be plain ASCII.
 */
+ (NSString*)countString:(long)count singular:(const char*)singular plural:(const char*)plural;

@end
