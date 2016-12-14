
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

#include "syntax/format.h"
#include "syntax/format_p.h"
#include "syntax/definition.h"
#include "syntax/definitionref.h"
#include "syntax/textstyledata_p.h"
#include "syntax/theme.h"
#include "syntax/themedata_p.h"

#include <cassert>

#include "utilities/stringutils.h"
#include "utilities/xmlutils.h"

namespace regina {
namespace syntax {

static inline unsigned readColour(const std::string& str) {
    if (str.empty() || str.front() != '#')
        return 0;
    return static_cast<unsigned>(strtoul(str.c_str() + 1, 0, 16));
}

static Theme::TextStyle stringToDefaultFormat(const std::string& str)
{
    if (!regina::startsWith(str, "ds"))
        return Theme::Normal;

    bool ok = false;

    // TODO: Make this log-time in the enumerator size.
    if (str == "dsNormal") return Theme::Normal;
    if (str == "dsKeyword") return Theme::Keyword;
    if (str == "dsFunction") return Theme::Function;
    if (str == "dsVariable") return Theme::Variable;
    if (str == "dsControlFlow") return Theme::ControlFlow;
    if (str == "dsOperator") return Theme::Operator;
    if (str == "dsBuiltIn") return Theme::BuiltIn;
    if (str == "dsExtension") return Theme::Extension;
    if (str == "dsPreprocessor") return Theme::Preprocessor;
    if (str == "dsAttribute") return Theme::Attribute;
    if (str == "dsChar") return Theme::Char;
    if (str == "dsSpecialChar") return Theme::SpecialChar;
    if (str == "dsString") return Theme::String;
    if (str == "dsVerbatimString") return Theme::VerbatimString;
    if (str == "dsSpecialString") return Theme::SpecialString;
    if (str == "dsImport") return Theme::Import;
    if (str == "dsDataType") return Theme::DataType;
    if (str == "dsDecVal") return Theme::DecVal;
    if (str == "dsBaseN") return Theme::BaseN;
    if (str == "dsFloat") return Theme::Float;
    if (str == "dsConstant") return Theme::Constant;
    if (str == "dsComment") return Theme::Comment;

    return Theme::Normal;
}

FormatPrivate::FormatPrivate()
    : defaultStyle(Theme::Normal)
    , id(0)
    , spellCheck(true)
{
}

FormatPrivate* FormatPrivate::get(const Format &format)
{
    return format.d.get();
}

Format::Format() : d(new FormatPrivate)
{
}

Format::Format(const Format &other) :
    d(other.d)
{
}

Format::~Format()
{
}

Format& Format::operator=(const Format& other)
{
    d = other.d;
    return *this;
}

bool Format::isValid() const
{
    return !d->name.empty();
}

const std::string& Format::name() const
{
    return d->name;
}

uint16_t Format::id() const
{
    return d->id;
}

bool Format::isDefaultTextStyle(const Theme &theme) const
{
    return (!hasTextColor(theme))
        && (!hasBackgroundColor(theme))
        && (selectedTextColor(theme) == theme.selectedTextColor(Theme::Normal))
        && (selectedBackgroundColor(theme) == theme.selectedBackgroundColor(Theme::Normal))
        && (isBold(theme) == theme.isBold(Theme::Normal))
        && (isItalic(theme) == theme.isItalic(Theme::Normal))
        && (isUnderline(theme) == theme.isUnderline(Theme::Normal))
        && (isStrikeThrough(theme) == theme.isStrikeThrough(Theme::Normal));
}

bool Format::hasTextColor(const Theme &theme) const
{
    return textColor(theme) != theme.textColor(Theme::Normal)
        && (d->style.textColor || theme.textColor(d->defaultStyle));
}

unsigned Format::textColor(const Theme &theme) const
{
    return d->style.textColor ? d->style.textColor : theme.textColor(d->defaultStyle);
}

unsigned Format::selectedTextColor(const Theme &theme) const
{
    return d->style.selectedTextColor ? d->style.selectedTextColor : theme.selectedTextColor(d->defaultStyle);
}

bool Format::hasBackgroundColor(const Theme &theme) const
{
    return backgroundColor(theme) != theme.backgroundColor(Theme::Normal)
         && (d->style.backgroundColor || theme.backgroundColor(d->defaultStyle));
}

unsigned Format::backgroundColor(const Theme &theme) const
{
    return d->style.backgroundColor ? d->style.backgroundColor : theme.backgroundColor(d->defaultStyle);
}

unsigned Format::selectedBackgroundColor(const Theme &theme) const
{
    return d->style.selectedBackgroundColor ? d->style.selectedBackgroundColor
                            : theme.selectedBackgroundColor(d->defaultStyle);
}

bool Format::isBold(const Theme &theme) const
{
    return d->style.hasBold ? d->style.bold : theme.isBold(d->defaultStyle);
}

bool Format::isItalic(const Theme &theme) const
{
    return d->style.hasItalic ? d->style.italic : theme.isItalic(d->defaultStyle);
}

bool Format::isUnderline(const Theme &theme) const
{
    return d->style.hasUnderline ? d->style.underline : theme.isUnderline(d->defaultStyle);
}

bool Format::isStrikeThrough(const Theme &theme) const
{
    return d->style.hasStrikeThrough ? d->style.strikeThrough : theme.isStrikeThrough(d->defaultStyle);
}

bool Format::spellCheck() const
{
    return d->spellCheck;
}


void FormatPrivate::load(xmlTextReaderPtr reader)
{
    name = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"name"));
    defaultStyle = stringToDefaultFormat(
        regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"defStyleNum")));

    std::string ref = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"color"));
    if (!ref.empty()) {
        style.textColor = readColour(ref);
    }

    ref = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"selColor"));
    if (!ref.empty()) {
        style.selectedTextColor = readColour(ref);
    }

    ref = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"backgroundColor"));
    if (!ref.empty()) {
        style.backgroundColor = readColour(ref);
    }

    ref = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"selBackgroundColor"));
    if (!ref.empty()) {
        style.selectedBackgroundColor = readColour(ref);
    }

    bool b;
    ref = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"italic"));
    if (!ref.empty()) {
        regina::valueOf(ref, b);
        style.hasItalic = true;
        style.italic = b;
    }

    ref = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"bold"));
    if (!ref.empty()) {
        regina::valueOf(ref, b);
        style.hasBold = true;
        style.bold = b;
    }

    ref = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"underline"));
    if (!ref.empty()) {
        regina::valueOf(ref, b);
        style.hasUnderline = true;
        style.underline = b;
    }

    ref = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"strikeOut"));
    if (!ref.empty()) {
        regina::valueOf(ref, b);
        style.hasStrikeThrough = true;
        style.strikeThrough = b;
    }

    ref = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"spellChecking"));
    if (!ref.empty()) {
        regina::valueOf(ref, spellCheck);
    }
}

} } // namespace regina::syntax
