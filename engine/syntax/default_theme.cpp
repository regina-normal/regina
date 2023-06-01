/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

namespace regina::syntax {

#ifndef USE_JANSSON
void ThemeData::loadDefault() {
    // The code below basically hard-codes the contents of default.theme.

    // Metadata:
    m_name = "Default";
    m_revision = 1;
    m_author = "Kate Authors";
    m_license = "MIT";
    m_readOnly = true;

    // Text styles:
    m_textStyles[Theme::Normal].textColor = 0x1f1c1b;
    m_textStyles[Theme::Normal].selectedTextColor = 0xffffff;
    m_textStyles[Theme::Normal].bold = false;
    m_textStyles[Theme::Normal].hasBold = true;
    m_textStyles[Theme::Normal].italic = false;
    m_textStyles[Theme::Normal].hasItalic = true;
    m_textStyles[Theme::Normal].underline = false;
    m_textStyles[Theme::Normal].hasUnderline = true;
    m_textStyles[Theme::Normal].strikeThrough = false;
    m_textStyles[Theme::Normal].hasStrikeThrough = true;

    m_textStyles[Theme::Keyword].textColor = 0x1f1c1b;
    m_textStyles[Theme::Keyword].selectedTextColor = 0xffffff;
    m_textStyles[Theme::Keyword].bold = true;
    m_textStyles[Theme::Keyword].hasBold = true;

    m_textStyles[Theme::Function].textColor = 0x644a9b;
    m_textStyles[Theme::Function].selectedTextColor = 0x452886;

    m_textStyles[Theme::Variable].textColor = 0x0057ae;
    m_textStyles[Theme::Variable].selectedTextColor = 0x00316e;

    m_textStyles[Theme::ControlFlow].textColor = 0x1f1c1b;
    m_textStyles[Theme::ControlFlow].selectedTextColor = 0xffffff;
    m_textStyles[Theme::ControlFlow].bold = true;
    m_textStyles[Theme::ControlFlow].hasBold = true;

    m_textStyles[Theme::Operator].textColor = 0x1f1c1b;
    m_textStyles[Theme::Operator].selectedTextColor = 0xffffff;

    m_textStyles[Theme::BuiltIn].textColor = 0x644a9b;
    m_textStyles[Theme::BuiltIn].selectedTextColor = 0x452886;
    m_textStyles[Theme::BuiltIn].bold = true;
    m_textStyles[Theme::BuiltIn].hasBold = true;

    m_textStyles[Theme::Extension].textColor = 0x0095ff;
    m_textStyles[Theme::Extension].selectedTextColor = 0xffffff;
    m_textStyles[Theme::Extension].bold = true;
    m_textStyles[Theme::Extension].hasBold = true;

    m_textStyles[Theme::Preprocessor].textColor = 0x006e28;
    m_textStyles[Theme::Preprocessor].selectedTextColor = 0x006e28;

    m_textStyles[Theme::Attribute].textColor = 0x0057ae;
    m_textStyles[Theme::Attribute].selectedTextColor = 0x00316e;

    m_textStyles[Theme::Char].textColor = 0x924c9d;
    m_textStyles[Theme::Char].selectedTextColor = 0x6c2477;

    m_textStyles[Theme::SpecialChar].textColor = 0x3daee9;
    m_textStyles[Theme::SpecialChar].selectedTextColor = 0xfcfcfc;

    m_textStyles[Theme::String].textColor = 0xbf0303;
    m_textStyles[Theme::String].selectedTextColor = 0x9c0e0e;

    m_textStyles[Theme::VerbatimString].textColor = 0xbf0303;
    m_textStyles[Theme::VerbatimString].selectedTextColor = 0x9c0e0e;

    m_textStyles[Theme::SpecialString].textColor = 0xff5500;
    m_textStyles[Theme::SpecialString].selectedTextColor = 0xff5500;

    m_textStyles[Theme::Import].textColor = 0xff5500;
    m_textStyles[Theme::Import].selectedTextColor = 0xff5500;

    m_textStyles[Theme::DataType].textColor = 0x0057ae;
    m_textStyles[Theme::DataType].selectedTextColor = 0x00316e;

    m_textStyles[Theme::DecVal].textColor = 0xb08000;
    m_textStyles[Theme::DecVal].selectedTextColor = 0x805c00;

    m_textStyles[Theme::BaseN].textColor = 0xb08000;
    m_textStyles[Theme::BaseN].selectedTextColor = 0x805c00;

    m_textStyles[Theme::Float].textColor = 0xb08000;
    m_textStyles[Theme::Float].selectedTextColor = 0x805c00;

    m_textStyles[Theme::Constant].textColor = 0xaa5500;
    m_textStyles[Theme::Constant].selectedTextColor = 0x5e2f00;

    m_textStyles[Theme::Comment].textColor = 0x898887;
    m_textStyles[Theme::Comment].selectedTextColor = 0x5e5d5d;

    m_textStyles[Theme::Documentation].textColor = 0x607880;
    m_textStyles[Theme::Documentation].selectedTextColor = 0x46585e;

    m_textStyles[Theme::Annotation].textColor = 0xca60ca;
    m_textStyles[Theme::Annotation].selectedTextColor = 0xa44ea4;

    m_textStyles[Theme::CommentVar].textColor = 0x0095ff;
    m_textStyles[Theme::CommentVar].selectedTextColor = 0xffffff;

    m_textStyles[Theme::RegionMarker].textColor = 0x0057ae;
    m_textStyles[Theme::RegionMarker].selectedTextColor = 0x00316e;
    m_textStyles[Theme::RegionMarker].backgroundColor = 0xe0e9f8;

    m_textStyles[Theme::Information].textColor = 0xb08000;
    m_textStyles[Theme::Information].selectedTextColor = 0x805c00;

    m_textStyles[Theme::Warning].textColor = 0xbf0303;
    m_textStyles[Theme::Warning].selectedTextColor = 0x9c0e0e;

    m_textStyles[Theme::Alert].textColor = 0xbf0303;
    m_textStyles[Theme::Alert].selectedTextColor = 0x9c0e0e;
    m_textStyles[Theme::Alert].backgroundColor = 0xf7e6e6;
    m_textStyles[Theme::Alert].bold = true;
    m_textStyles[Theme::Alert].hasBold = true;

    m_textStyles[Theme::Error].textColor = 0xbf0303;
    m_textStyles[Theme::Error].selectedTextColor = 0x9c0e0e;
    m_textStyles[Theme::Error].underline = true;
    m_textStyles[Theme::Error].hasUnderline = true;

    m_textStyles[Theme::Others].textColor = 0x006e28;
    m_textStyles[Theme::Others].selectedTextColor = 0x006e28;

    // Editor colours:
    m_editorColors[Theme::BackgroundColor] = 0xffffff;
    m_editorColors[Theme::TextSelection] = 0x94caef;
    m_editorColors[Theme::CurrentLine] = 0xf8f7f6;
    m_editorColors[Theme::SearchHighlight] = 0xffff00;
    m_editorColors[Theme::ReplaceHighlight] = 0x00ff00;
    m_editorColors[Theme::BracketMatching] = 0xffff00;
    m_editorColors[Theme::TabMarker] = 0xd2d2d2;
    m_editorColors[Theme::SpellChecking] = 0xbf0303;
    m_editorColors[Theme::IndentationLine] = 0xd2d2d2;
    m_editorColors[Theme::IconBorder] = 0xf0f0f0;
    m_editorColors[Theme::LineNumbers] = 0xa0a0a0;
    m_editorColors[Theme::CurrentLineNumber] = 0x1e1e1e;
    m_editorColors[Theme::WordWrapMarker] = 0xededed;
    m_editorColors[Theme::ModifiedLines] = 0xfdbc4b;
    m_editorColors[Theme::SavedLines] = 0x2ecc71;
    m_editorColors[Theme::Separator] = 0x898887;
    m_editorColors[Theme::MarkBookmark] = 0x0000ff;
    m_editorColors[Theme::MarkBreakpointActive] = 0xff0000;
    m_editorColors[Theme::MarkBreakpointReached] = 0xffff00;
    m_editorColors[Theme::MarkBreakpointDisabled] = 0xff00ff;
    m_editorColors[Theme::MarkExecution] = 0xa0a0a4;
    m_editorColors[Theme::MarkWarning] = 0x00ff00;
    m_editorColors[Theme::MarkError] = 0xff0000;
    m_editorColors[Theme::TemplateBackground] = 0xd6d2d0;
    m_editorColors[Theme::TemplatePlaceholder] = 0xbaf8ce;
    m_editorColors[Theme::TemplateFocusedPlaceholder] = 0x76da98;
    m_editorColors[Theme::TemplateReadOnlyPlaceholder] = 0xf6e6e6;
    // Unused: "code-folding" : "#94caef"
}

#endif

} // namespace regina::syntax
