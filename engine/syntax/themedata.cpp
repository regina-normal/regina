
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

#include "syntax/themedata_p.h"

#include <jansson.h>

#include <cassert>
#include <cstring>
#include <iostream>

namespace regina {
namespace syntax {

ThemeData* ThemeData::get(const Theme &theme)
{
    return theme.m_data.get();
}

ThemeData::ThemeData()
    : m_revision(0)
    , m_readOnly(true)
{
    memset(m_editorColors, 0, sizeof(m_editorColors));
}

/**
 * Convert JSON object @p val into a color, if possible. Valid colors are only
 * in hex format: #rrggbb or #aarrggbb. On error, returns 0x00000000.
 */
static inline unsigned readColor(json_t* val)
{
    const char* str = json_string_value(val);
    if (! (str && *str == '#'))
        return 0;
    return static_cast<unsigned>(strtoul(str + 1, 0, 16));
}

static inline void readString(json_t* val, std::string& result)
{
    const char* str = json_string_value(val);
    if (str)
        result = str;
    else
        result.clear();
}

static void readThemeData(json_t* obj, TextStyleData& dest)
{
    dest.textColor = readColor(json_object_get(obj, "text-color"));
    dest.backgroundColor = readColor(json_object_get(obj, "background-color"));
    dest.selectedTextColor = readColor(json_object_get(obj, "selected-text-color"));
    dest.selectedBackgroundColor = readColor(json_object_get(obj, "selected-background-color"));

    json_t* val = json_object_get(obj, "bold");
    if (json_is_boolean(val)) {
        dest.bold = json_boolean_value(val);
        dest.hasBold = true;
    }
    val = json_object_get(obj, "italic");
    if (json_is_boolean(val)) {
        dest.italic = json_boolean_value(val);
        dest.hasItalic = true;
    }
    val = json_object_get(obj, "underline");
    if (json_is_boolean(val)) {
        dest.underline = json_boolean_value(val);
        dest.hasUnderline = true;
    }
    val = json_object_get(obj, "strike-through");
    if (json_is_boolean(val)) {
        dest.strikeThrough = json_boolean_value(val);
        dest.hasStrikeThrough = true;
    }
}

bool ThemeData::load(const std::string& filePath)
{
    m_filePath = filePath.c_str();

    json_error_t error;
    json_t* obj = json_load_file(filePath.c_str(), 0, &error);
    if (! obj) {
        std::cerr << "Failed to parse theme file " << filePath << std::endl;
        std::cerr << "Error on line " << error.line << ": " << error.text << std::endl;
        return false;
    }

    // read metadata
    json_t* metadata = json_object_get(obj, "metadata");
    readString(json_object_get(metadata, "name"), m_name);
    m_revision = json_integer_value(json_object_get(metadata, "revision"));
    readString(json_object_get(metadata, "author"), m_author);
    readString(json_object_get(metadata, "license"), m_license);
    m_readOnly = json_boolean_value(json_object_get(metadata, "read-only"));

    // read text styles
    json_t* textStyles = json_object_get(obj, "text-styles");
    readThemeData(json_object_get(textStyles, "Normal"), m_textStyles[Theme::Normal]);
    readThemeData(json_object_get(textStyles, "Keyword"), m_textStyles[Theme::Keyword]);
    readThemeData(json_object_get(textStyles, "Function"), m_textStyles[Theme::Function]);
    readThemeData(json_object_get(textStyles, "Variable"), m_textStyles[Theme::Variable]);
    readThemeData(json_object_get(textStyles, "ControlFlow"), m_textStyles[Theme::ControlFlow]);
    readThemeData(json_object_get(textStyles, "Operator"), m_textStyles[Theme::Operator]);
    readThemeData(json_object_get(textStyles, "BuiltIn"), m_textStyles[Theme::BuiltIn]);
    readThemeData(json_object_get(textStyles, "Extension"), m_textStyles[Theme::Extension]);
    readThemeData(json_object_get(textStyles, "Preprocessor"), m_textStyles[Theme::Preprocessor]);
    readThemeData(json_object_get(textStyles, "Attribute"), m_textStyles[Theme::Attribute]);
    readThemeData(json_object_get(textStyles, "Char"), m_textStyles[Theme::Char]);
    readThemeData(json_object_get(textStyles, "SpecialChar"), m_textStyles[Theme::SpecialChar]);
    readThemeData(json_object_get(textStyles, "String"), m_textStyles[Theme::String]);
    readThemeData(json_object_get(textStyles, "VerbatimString"), m_textStyles[Theme::VerbatimString]);
    readThemeData(json_object_get(textStyles, "SpecialString"), m_textStyles[Theme::SpecialString]);
    readThemeData(json_object_get(textStyles, "Import"), m_textStyles[Theme::Import]);
    readThemeData(json_object_get(textStyles, "DataType"), m_textStyles[Theme::DataType]);
    readThemeData(json_object_get(textStyles, "DecVal"), m_textStyles[Theme::DecVal]);
    readThemeData(json_object_get(textStyles, "BaseN"), m_textStyles[Theme::BaseN]);
    readThemeData(json_object_get(textStyles, "Float"), m_textStyles[Theme::Float]);
    readThemeData(json_object_get(textStyles, "Constant"), m_textStyles[Theme::Constant]);
    readThemeData(json_object_get(textStyles, "Comment"), m_textStyles[Theme::Comment]);

    // read editor area colors
    json_t* editorColors = json_object_get(obj, "editor-colors");
    m_editorColors[Theme::BackgroundColor] = readColor(json_object_get(editorColors, "background-color"));
    m_editorColors[Theme::TextSelection] = readColor(json_object_get(editorColors, "selection"));
    m_editorColors[Theme::CurrentLine] = readColor(json_object_get(editorColors, "current-line"));
    m_editorColors[Theme::SearchHighlight] = readColor(json_object_get(editorColors, "search-highlight"));
    m_editorColors[Theme::ReplaceHighlight] = readColor(json_object_get(editorColors, "replace-highlight"));
    m_editorColors[Theme::BracketMatching] = readColor(json_object_get(editorColors, "bracket-matching"));
    m_editorColors[Theme::TabMarker] = readColor(json_object_get(editorColors, "tab-marker"));
    m_editorColors[Theme::SpellChecking] = readColor(json_object_get(editorColors, "spell-checking"));
    m_editorColors[Theme::IndentationLine] = readColor(json_object_get(editorColors, "indentation-line"));
    m_editorColors[Theme::IconBorder] = readColor(json_object_get(editorColors, "icon-border"));
    m_editorColors[Theme::LineNumbers] = readColor(json_object_get(editorColors, "line-numbers"));
    m_editorColors[Theme::CurrentLineNumber] = readColor(json_object_get(editorColors, "current-line-number"));
    m_editorColors[Theme::WordWrapMarker] = readColor(json_object_get(editorColors, "word-wrap-marker"));
    m_editorColors[Theme::ModifiedLines] = readColor(json_object_get(editorColors, "modified-lines"));
    m_editorColors[Theme::SavedLines] = readColor(json_object_get(editorColors, "saved-lines"));
    m_editorColors[Theme::Separator] = readColor(json_object_get(editorColors, "separator"));
    m_editorColors[Theme::MarkBookmark] = readColor(json_object_get(editorColors, "mark-bookmark"));
    m_editorColors[Theme::MarkBreakpointActive] = readColor(json_object_get(editorColors, "mark-breakpoint-active"));
    m_editorColors[Theme::MarkBreakpointReached] = readColor(json_object_get(editorColors, "mark-breakpoint-reached"));
    m_editorColors[Theme::MarkBreakpointDisabled] = readColor(json_object_get(editorColors, "mark-breakpoint-disabled"));
    m_editorColors[Theme::MarkExecution] = readColor(json_object_get(editorColors, "mark-execution"));
    m_editorColors[Theme::MarkWarning] = readColor(json_object_get(editorColors, "mark-warning"));
    m_editorColors[Theme::MarkError] = readColor(json_object_get(editorColors, "mark-error"));
    m_editorColors[Theme::TemplateBackground] = readColor(json_object_get(editorColors, "template-background"));
    m_editorColors[Theme::TemplatePlaceholder] = readColor(json_object_get(editorColors, "template-placeholder"));
    m_editorColors[Theme::TemplateFocusedPlaceholder] = readColor(json_object_get(editorColors, "template-focused-placeholder"));
    m_editorColors[Theme::TemplateReadOnlyPlaceholder] = readColor(json_object_get(editorColors, "template-read-only-placeholder"));

    json_decref(obj);
    return true;
}

const std::string& ThemeData::name() const
{
    return m_name;
}

int ThemeData::revision() const
{
    return m_revision;
}

bool ThemeData::isReadOnly() const
{
    return m_readOnly;
}

const std::string& ThemeData::filePath() const
{
    return m_filePath;
}

unsigned ThemeData::textColor(Theme::TextStyle style) const
{
    assert(static_cast<int>(style) >= 0 && static_cast<int>(style) <= static_cast<int>(Theme::Others));
    return m_textStyles[style].textColor;
}

unsigned ThemeData::selectedTextColor(Theme::TextStyle style) const
{
    assert(static_cast<int>(style) >= 0 && static_cast<int>(style) <= static_cast<int>(Theme::Others));
    return m_textStyles[style].selectedTextColor;
}

unsigned ThemeData::backgroundColor(Theme::TextStyle style) const
{
    assert(static_cast<int>(style) >= 0 && static_cast<int>(style) <= static_cast<int>(Theme::Others));
    return m_textStyles[style].backgroundColor;
}

unsigned ThemeData::selectedBackgroundColor(Theme::TextStyle style) const
{
    assert(static_cast<int>(style) >= 0 && static_cast<int>(style) <= static_cast<int>(Theme::Others));
    return m_textStyles[style].selectedBackgroundColor;
}

bool ThemeData::isBold(Theme::TextStyle style) const
{
    assert(static_cast<int>(style) >= 0 && static_cast<int>(style) <= static_cast<int>(Theme::Others));
    return m_textStyles[style].bold;
}

bool ThemeData::isItalic(Theme::TextStyle style) const
{
    assert(static_cast<int>(style) >= 0 && static_cast<int>(style) <= static_cast<int>(Theme::Others));
    return m_textStyles[style].italic;
}

bool ThemeData::isUnderline(Theme::TextStyle style) const
{
    assert(static_cast<int>(style) >= 0 && static_cast<int>(style) <= static_cast<int>(Theme::Others));
    return m_textStyles[style].underline;
}

bool ThemeData::isStrikeThrough(Theme::TextStyle style) const
{
    assert(static_cast<int>(style) >= 0 && static_cast<int>(style) <= static_cast<int>(Theme::Others));
    return m_textStyles[style].strikeThrough;
}

unsigned ThemeData::editorColor(Theme::EditorColorRole role) const
{
    assert(static_cast<int>(role) >= 0 && static_cast<int>(role) <= static_cast<int>(Theme::TemplateReadOnlyPlaceholder));
    return m_editorColors[role];
}

} } // namespace regina::syntax
