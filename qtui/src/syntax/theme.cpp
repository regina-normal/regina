/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

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

#include "theme.h"
#include "themedata_p.h"

using namespace KSyntaxHighlighting;

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
