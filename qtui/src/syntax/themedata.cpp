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

#include "themedata_p.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <cassert>
#include <iostream>

using namespace KSyntaxHighlighting;

ThemeData* ThemeData::get(const Theme &theme)
{
    return theme.m_data.data();
}

ThemeData::ThemeData()
    : m_revision(0)
    , m_readOnly(true)
{
    memset(m_editorColors, 0, sizeof(m_editorColors));
}

/**
 * Convert QJsonValue @p val into a color, if possible. Valid colors are only
 * in hex format: #rrggbb. On error, returns 0x00000000.
 */
static inline QRgb readColor(const QJsonValue &val)
{
    const QRgb unsetColor = 0;
    if (!val.isString()) {
        return unsetColor;
    }
    const QString str = val.toString();
    if (str.isEmpty() || str[0] != QLatin1Char('#')) {
        return unsetColor;
    }
    const QColor color(str);
    return color.isValid() ? color.rgb() : unsetColor;
}

static inline TextStyleData readThemeData(const QJsonObject &obj)
{
    TextStyleData td;

    td.textColor = readColor(obj.value(QLatin1String("text-color")));
    td.backgroundColor = readColor(obj.value(QLatin1String("background-color")));
    td.selectedTextColor = readColor(obj.value(QLatin1String("selected-text-color")));
    td.selectedBackgroundColor = readColor(obj.value(QLatin1String("selected-background-color")));

    auto val = obj.value(QLatin1String("bold"));
    if (val.isBool()) {
        td.bold = val.toBool();
        td.hasBold = true;
    }
    val = obj.value(QLatin1String("italic"));
    if (val.isBool()) {
        td.italic = val.toBool();
        td.hasItalic = true;
    }
    val = obj.value(QLatin1String("underline"));
    if (val.isBool()) {
        td.underline = val.toBool();
        td.hasUnderline = true;
    }
    val = obj.value(QLatin1String("strike-through"));
    if (val.isBool()) {
        td.strikeThrough = val.toBool();
        td.hasStrikeThrough = true;
    }

    return td;
}

bool ThemeData::load(const std::string& filePath)
{
    QFile loadFile(filePath.c_str());
    if (!loadFile.open(QIODevice::ReadOnly)) {
        return false;
    }
    const QByteArray jsonData = loadFile.readAll();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        std::cerr << "Failed to parse theme file" << filePath << ":" << parseError.errorString().toUtf8().constData() << std::endl;
        return false;
    }

    m_filePath = filePath.c_str();

    QJsonObject obj = jsonDoc.object();

    // read metadata
    const QJsonObject metadata = obj.value(QLatin1String("metadata")).toObject();
    m_name = metadata.value(QLatin1String("name")).toString().toUtf8().constData();
    m_revision = metadata.value(QLatin1String("revision")).toInt();
    m_author = metadata.value(QLatin1String("author")).toString();
    m_license = metadata.value(QLatin1String("license")).toString();
    m_readOnly = metadata.value(QLatin1String("read-only")).toBool();

    // read text styles
    const QJsonObject textStyles = obj.value(QLatin1String("text-styles")).toObject();
    m_textStyles[Theme::Normal] = readThemeData(textStyles.value(QLatin1String("Normal")).toObject());
    m_textStyles[Theme::Keyword] = readThemeData(textStyles.value(QLatin1String("Keyword")).toObject());
    m_textStyles[Theme::Function] = readThemeData(textStyles.value(QLatin1String("Function")).toObject());
    m_textStyles[Theme::Variable] = readThemeData(textStyles.value(QLatin1String("Variable")).toObject());
    m_textStyles[Theme::ControlFlow] = readThemeData(textStyles.value(QLatin1String("ControlFlow")).toObject());
    m_textStyles[Theme::Operator] = readThemeData(textStyles.value(QLatin1String("Operator")).toObject());
    m_textStyles[Theme::BuiltIn] = readThemeData(textStyles.value(QLatin1String("BuiltIn")).toObject());
    m_textStyles[Theme::Extension] = readThemeData(textStyles.value(QLatin1String("Extension")).toObject());
    m_textStyles[Theme::Preprocessor] = readThemeData(textStyles.value(QLatin1String("Preprocessor")).toObject());
    m_textStyles[Theme::Attribute] = readThemeData(textStyles.value(QLatin1String("Attribute")).toObject());
    m_textStyles[Theme::Char] = readThemeData(textStyles.value(QLatin1String("Char")).toObject());
    m_textStyles[Theme::SpecialChar] = readThemeData(textStyles.value(QLatin1String("SpecialChar")).toObject());
    m_textStyles[Theme::String] = readThemeData(textStyles.value(QLatin1String("String")).toObject());
    m_textStyles[Theme::VerbatimString] = readThemeData(textStyles.value(QLatin1String("VerbatimString")).toObject());
    m_textStyles[Theme::SpecialString] = readThemeData(textStyles.value(QLatin1String("SpecialString")).toObject());
    m_textStyles[Theme::Import] = readThemeData(textStyles.value(QLatin1String("Import")).toObject());
    m_textStyles[Theme::DataType] = readThemeData(textStyles.value(QLatin1String("DataType")).toObject());
    m_textStyles[Theme::DecVal] = readThemeData(textStyles.value(QLatin1String("DecVal")).toObject());
    m_textStyles[Theme::BaseN] = readThemeData(textStyles.value(QLatin1String("BaseN")).toObject());
    m_textStyles[Theme::Float] = readThemeData(textStyles.value(QLatin1String("Float")).toObject());
    m_textStyles[Theme::Constant] = readThemeData(textStyles.value(QLatin1String("Constant")).toObject());
    m_textStyles[Theme::Comment] = readThemeData(textStyles.value(QLatin1String("Comment")).toObject());

    // read editor area colors
    const QJsonObject editorColors = obj.value(QLatin1String("editor-colors")).toObject();
    m_editorColors[Theme::BackgroundColor] = readColor(editorColors.value(QLatin1String("background-color")));
    m_editorColors[Theme::TextSelection] = readColor(editorColors.value(QLatin1String("selection")));
    m_editorColors[Theme::CurrentLine] = readColor(editorColors.value(QLatin1String("current-line")));
    m_editorColors[Theme::SearchHighlight] = readColor(editorColors.value(QLatin1String("search-highlight")));
    m_editorColors[Theme::ReplaceHighlight] = readColor(editorColors.value(QLatin1String("replace-highlight")));
    m_editorColors[Theme::BracketMatching] = readColor(editorColors.value(QLatin1String("bracket-matching")));
    m_editorColors[Theme::TabMarker] = readColor(editorColors.value(QLatin1String("tab-marker")));
    m_editorColors[Theme::SpellChecking] = readColor(editorColors.value(QLatin1String("spell-checking")));
    m_editorColors[Theme::IndentationLine] = readColor(editorColors.value(QLatin1String("indentation-line")));
    m_editorColors[Theme::IconBorder] = readColor(editorColors.value(QLatin1String("icon-border")));
    m_editorColors[Theme::LineNumbers] = readColor(editorColors.value(QLatin1String("line-numbers")));
    m_editorColors[Theme::CurrentLineNumber] = readColor(editorColors.value(QLatin1String("current-line-number")));
    m_editorColors[Theme::WordWrapMarker] = readColor(editorColors.value(QLatin1String("word-wrap-marker")));
    m_editorColors[Theme::ModifiedLines] = readColor(editorColors.value(QLatin1String("modified-lines")));
    m_editorColors[Theme::SavedLines] = readColor(editorColors.value(QLatin1String("saved-lines")));
    m_editorColors[Theme::Separator] = readColor(editorColors.value(QLatin1String("separator")));
    m_editorColors[Theme::MarkBookmark] = readColor(editorColors.value(QLatin1String("mark-bookmark")));
    m_editorColors[Theme::MarkBreakpointActive] = readColor(editorColors.value(QLatin1String("mark-breakpoint-active")));
    m_editorColors[Theme::MarkBreakpointReached] = readColor(editorColors.value(QLatin1String("mark-breakpoint-reached")));
    m_editorColors[Theme::MarkBreakpointDisabled] = readColor(editorColors.value(QLatin1String("mark-breakpoint-disabled")));
    m_editorColors[Theme::MarkExecution] = readColor(editorColors.value(QLatin1String("mark-execution")));
    m_editorColors[Theme::MarkWarning] = readColor(editorColors.value(QLatin1String("mark-warning")));
    m_editorColors[Theme::MarkError] = readColor(editorColors.value(QLatin1String("mark-error")));
    m_editorColors[Theme::TemplateBackground] = readColor(editorColors.value(QLatin1String("template-background")));
    m_editorColors[Theme::TemplatePlaceholder] = readColor(editorColors.value(QLatin1String("template-placeholder")));
    m_editorColors[Theme::TemplateFocusedPlaceholder] = readColor(editorColors.value(QLatin1String("template-focused-placeholder")));
    m_editorColors[Theme::TemplateReadOnlyPlaceholder] = readColor(editorColors.value(QLatin1String("template-read-only-placeholder")));

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

QString ThemeData::filePath() const
{
    return m_filePath;
}

QRgb ThemeData::textColor(Theme::TextStyle style) const
{
    assert(static_cast<int>(style) >= 0 && static_cast<int>(style) <= static_cast<int>(Theme::Others));
    return m_textStyles[style].textColor;
}

QRgb ThemeData::selectedTextColor(Theme::TextStyle style) const
{
    assert(static_cast<int>(style) >= 0 && static_cast<int>(style) <= static_cast<int>(Theme::Others));
    return m_textStyles[style].selectedTextColor;
}

QRgb ThemeData::backgroundColor(Theme::TextStyle style) const
{
    assert(static_cast<int>(style) >= 0 && static_cast<int>(style) <= static_cast<int>(Theme::Others));
    return m_textStyles[style].backgroundColor;
}

QRgb ThemeData::selectedBackgroundColor(Theme::TextStyle style) const
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

QRgb ThemeData::editorColor(Theme::EditorColorRole role) const
{
    assert(static_cast<int>(role) >= 0 && static_cast<int>(role) <= static_cast<int>(Theme::TemplateReadOnlyPlaceholder));
    return m_editorColors[role];
}
