/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>
    Copyright (C) 2016 Dominik Haumann <dhaumann@kde.org>

    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KSYNTAXHIGHLIGHTING_THEMEDATA_P_H
#define KSYNTAXHIGHLIGHTING_THEMEDATA_P_H

#include "syntax/theme.h"
#include "syntax/textstyledata_p.h"

#include <string>
#include <boost/noncopyable.hpp>

namespace KSyntaxHighlighting {

/**
 * Data container for a Theme.
 */
class ThemeData : boost::noncopyable
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

}

#endif // KSYNTAXHIGHLIGHTING_THEMEDATA_P_H
