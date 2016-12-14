
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
 * \file syntax/themedata_p.h
 * \brief Part of the private syntax highlighting framework that is shared
 * between Regina's different graphical user interfaces.
 *
 * The syntax highlighting headers are private in the following sense: they are
 * not installed along with Regina's other headers, they do not appear in the
 * API documentation, and the syntax highlighting API is subject to change
 * without notice between different versions of Regina.
 */

#ifndef __SYNTAX_THEMEDATA_P_H
#ifndef __DOXYGEN
#define __SYNTAX_THEMEDATA_P_H
#endif

#include "regina-core.h"
#include "syntax/theme.h"
#include "syntax/textstyledata_p.h"

#include <string>
#include <boost/noncopyable.hpp>

namespace regina {
namespace syntax {

/**
 * Data container for a Theme.
 */
class REGINA_API ThemeData : boost::noncopyable
{
public:
    static ThemeData* get(const Theme &theme);

    /**
     * Default constructor, creating an uninitialized ThemeData instance.
     */
    ThemeData();

    /**
     * Load the Theme data from the file @p filePath.
     * Note, that @p filePath either is a local file, or a qt resource location.
     */
    bool load(const std::string& filePath);

    /**
     * Returns the unique name of this Theme.
     */
    const std::string& name() const;

    int revision() const;

    /**
     * Returns @c true if this Theme is read-only.
     * Typically, themes that are shipped by default are read-only.
     */
    bool isReadOnly() const;

    /**
     * Returns the full path and filename to this Theme.
     *
     * If the theme is invalid (isValid()), an empty string is returned.
     */
    const std::string& filePath() const;

    /**
     * Returns the text color to be used for @p style.
     * The color will be returned as an integer of the form #AARRGGBB.
     * @c 0 is returned for styles that do not specify a text color,
     * use the default text color in that case.
     */
    unsigned textColor(Theme::TextStyle style) const;

    /**
     * Returns the text color for selected to be used for @p style.
     * The color will be returned as an integer of the form #AARRGGBB.
     * @c 0 is returned for styles that do not specify a selected text color,
     * use the textColor() in that case.
     */
    unsigned selectedTextColor(Theme::TextStyle style) const;

    /**
     * Returns the background color to be used for @p style.
     * The color will be returned as an integer of the form #AARRGGBB.
     * @c 0 is returned for styles that do not specify a background color,
     * use the default background color in that case.
     */
    unsigned backgroundColor(Theme::TextStyle style) const;

    /**
     * Returns the background color for selected text to be used for @p style.
     * The color will be returned as an integer of the form #AARRGGBB.
     * @c 0 is returned for styles that do not specify a selected background
     * color, use the default backgroundColor() in that case.
     */
    unsigned selectedBackgroundColor(Theme::TextStyle style) const;

    /**
     * Returns whether the given style should be shown in bold.
     */
    bool isBold(Theme::TextStyle style) const;

    /**
     * Returns whether the given style should be shown in italic.
     */
    bool isItalic(Theme::TextStyle style) const;

    /**
     * Returns whether the given style should be shown underlined.
     */
    bool isUnderline(Theme::TextStyle style) const;

    /**
     * Returns whether the given style should be shown struck through.
     */
    bool isStrikeThrough(Theme::TextStyle style) const;

public:
    /**
     * Returns the editor color for the requested @p role.
     * The color will be returned as an integer of the form #AARRGGBB.
     */
    unsigned editorColor(Theme::EditorColorRole role) const;

private:
    int m_revision;
    std::string m_name;
    std::string m_author;
    std::string m_license;
    std::string m_filePath;
    bool m_readOnly;

    //! TextStyles
    TextStyleData m_textStyles[Theme::Others + 1];

    //! Editor area colors
    unsigned m_editorColors[Theme::TemplateReadOnlyPlaceholder + 1];
};

} } // namespace regina::syntax

#endif
