
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This file is modified from the KDE syntax-highlighting framework,     *
 *  which is copyright (C) 2016 Volker Krause <vkrause@kde.org>           *
 *  and is distributed under the GNU Library General Public License as    *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/**
 * \file syntax/format.h
 * \brief Part of the private syntax highlighting framework that is shared
 * between Regina's different graphical user interfaces.
 *
 * The syntax highlighting headers are private in the following sense: they are
 * not installed along with Regina's other headers, they do not appear in the
 * API documentation, and the syntax highlighting API is subject to change
 * without notice between different versions of Regina.
 */

#ifndef __SYNTAX_FORMAT_H
#ifndef __DOXYGEN
#define __SYNTAX_FORMAT_H
#endif

#include "regina-core.h"
#include <memory>

namespace regina {
namespace syntax {

class DefinitionRef;
class FormatPrivate;
class Theme;

/** Describes the format to be used for a specific text fragment.
 *  The actual format used for displaying is merged from the format information
 *  in the syntax definition file, and a theme.
 *
 *  @see Theme
 */
class REGINA_API Format
{
public:
    /** Creates an empty/invalid format. */
    Format();
    Format(const Format &other);
    ~Format();

    Format& operator=(const Format &other);

    /** Returns @c true if this is a valid format, ie. one that
     *  was read from a syntax definition file.
     */
    bool isValid() const;

    /** The name of this format as used in the syntax definition file. */
    const std::string& name() const;

    /** Returns a unique identifier of this format.
     *  This is useful for efficient storing of formats in a text line. The
     *  identifier is unique per Repository instance, but will change when
     *  the repository is reloaded (which also invalidatess the corresponding
     *  Definition anyway).
     */
    uint16_t id() const;

    /** Returns @c true if the combination of this format and the theme @p theme
     *  do not change the default text format in any way.
     *  This is useful for output formats where changing formatting implies cost,
     *  and thus benefit from optimizing the default case of not having any format
     *  applied. If you make use of this, make sure to set the default text style
     *  to what the corresponding theme sets for Theme::Normal.
     */
    bool isDefaultTextStyle(const Theme &theme) const;

    /** Returns @c true if the combination of this format and the theme @p theme
     *  change the foreground color compared to the default format.
     */
    bool hasTextColor(const Theme &theme) const;
    /** Returns the foreground color of the combination of this format and the
     *  given theme.
     *  The color will be returned as an integer of the form #AARRGGBB.
     */
    unsigned textColor(const Theme &theme) const;
    /** Returns the foreground color for selected text of the combination of
     *  this format and the given theme.
     *  The color will be returned as an integer of the form #AARRGGBB.
     */
    unsigned selectedTextColor(const Theme &theme) const;
    /** Returns @c true if the combination of this format and the theme @p theme
     *  change the background color compared to the default format.
     */
    bool hasBackgroundColor(const Theme &theme) const;
    /** Returns the background color of the combination of this format and the
     *  given theme.
     *  The color will be returned as an integer of the form #AARRGGBB.
     */
    unsigned backgroundColor(const Theme &theme) const;
    /** Returns the background color of selected text of the combination of
     *  this format and the given theme.
     *  The color will be returned as an integer of the form #AARRGGBB.
     */
    unsigned selectedBackgroundColor(const Theme &theme) const;

    /** Returns @c true if the combination of this format and the given theme
     *  results in bold text formatting.
     */
    bool isBold(const Theme &theme) const;
    /** Returns @c true if the combination of this format and the given theme
     *  results in italic text formatting.
     */
    bool isItalic(const Theme &theme) const;
    /** Returns @c true if the combination of this format and the given theme
     *  results in underlined text.
     */
    bool isUnderline(const Theme &theme) const;
    /** Returns @c true if the combination of this format and the given theme
     *  results in struck through text.
     */
    bool isStrikeThrough(const Theme &theme) const;

    /**
     * Returns whether characters with this format should be spell checked.
     */
    bool spellCheck() const;

private:
    friend class FormatPrivate;
    std::shared_ptr<FormatPrivate> d;
};

} } // namespace regina::syntax

#endif
