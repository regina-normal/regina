
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

#include "syntax/theme.h"
#include "syntax/themedata_p.h"

namespace regina {
namespace syntax {

std::string nullString;

Theme::Theme()
{
}

Theme::Theme(const Theme &copy)
{
    m_data = copy.m_data;
}

Theme::Theme(ThemeData* data)
    : m_data(data)
{
}

Theme::~Theme()
{
}

Theme &Theme::operator=(const Theme &other)
{
    m_data = other.m_data;
    return *this;
}

bool Theme::isValid() const
{
    return m_data.get();
}

const std::string& Theme::name() const
{
    return m_data.get() ? m_data->name() : nullString;
}

bool Theme::isReadOnly() const
{
    return m_data.get() ? m_data->isReadOnly() : false;
}

const std::string& Theme::filePath() const
{
    return m_data.get() ? m_data->filePath() : nullString;
}

unsigned Theme::textColor(TextStyle style) const
{
    return m_data.get() ? m_data->textColor(style) : 0;
}

unsigned Theme::selectedTextColor(TextStyle style) const
{
    return m_data.get() ? m_data->selectedTextColor(style) : 0;
}

unsigned Theme::backgroundColor(TextStyle style) const
{
    return m_data.get() ? m_data->backgroundColor(style) : 0;
}

unsigned Theme::selectedBackgroundColor(TextStyle style) const
{
    return m_data.get() ? m_data->selectedBackgroundColor(style) : 0;
}

bool Theme::isBold(TextStyle style) const
{
    return m_data.get() ? m_data->isBold(style) : false;
}

bool Theme::isItalic(TextStyle style) const
{
    return m_data.get() ? m_data->isItalic(style) : false;
}

bool Theme::isUnderline(TextStyle style) const
{
    return m_data.get() ? m_data->isUnderline(style) : false;
}

bool Theme::isStrikeThrough(TextStyle style) const
{
    return m_data.get() ? m_data->isStrikeThrough(style) : false;
}

unsigned Theme::editorColor(EditorColorRole role) const
{
    return m_data.get() ? m_data->editorColor(role) : 0;
}

} } // namespace regina::syntax
