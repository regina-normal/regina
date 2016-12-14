
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

#include "syntax/definition.h"
#include "syntax/definition_p.h"
#include "syntax/definitionref.h"

#include "syntax/context.h"
#include "syntax/format.h"
#include "syntax/format_p.h"
#include "syntax/repository_p.h"
#include "syntax/rule.h"

#include <algorithm>
#include <cassert>
#include <iostream>

#include "utilities/stringutils.h"
#include "utilities/xmlutils.h"

namespace regina {
namespace syntax {

KeywordList nullList;

DefinitionData::DefinitionData() :
    repo(nullptr),
    delimiters("\t !%&()*+,-./:;<=>?[\\]^{|}~"), // must be sorted!
    caseSensitive(true),
    version(0.0f)
{
}

DefinitionData::~DefinitionData()
{
    for (auto i : contexts)
        delete i;
}

DefinitionData* DefinitionData::get(const Definition &def)
{
    return def.d.get();
}

Definition::Definition() :
    d(new DefinitionData)
{
}

Definition::Definition(const Definition &other) :
    d(other.d)
{
    d->q = *this;
}

Definition::Definition(const std::shared_ptr<DefinitionData> &dd) :
    d(dd)
{
}

Definition::~Definition()
{
}

Definition& Definition::operator=(const Definition &rhs)
{
    d = rhs.d;
    return *this;
}

bool Definition::operator==(const Definition &other) const
{
    return d->fileName == other.d->fileName;
}

bool Definition::operator!=(const Definition& other) const
{
    return d->fileName != other.d->fileName;
}

bool Definition::isValid() const
{
    return d->repo && !d->fileName.empty() && !d->name.empty();
}

const std::string& Definition::filePath() const
{
    return d->fileName;
}

const std::string& Definition::name() const
{
    return d->name;
}

const std::string& Definition::section() const
{
    return d->section;
}

int Definition::version() const
{
    return d->version;
}

const std::string& Definition::style() const
{
    return d->style;
}

const std::string& Definition::indenter() const
{
    return d->indenter;
}

const std::string& Definition::author() const
{
    return d->author;
}

const std::string& Definition::license() const
{
    return d->license;
}

Context* DefinitionData::initialContext() const
{
    assert(!contexts.empty());
    return contexts.front();
}

Context* DefinitionData::contextByName(const std::string& name) const
{
    for (auto context : contexts) {
        if (context->name() == name)
            return context;
    }
    return nullptr;
}

const KeywordList& DefinitionData::keywordList(const std::string& name) const
{
    const auto it = keywordLists.find(name);
    if (it != keywordLists.end())
        return *(it->second);

    return nullList;
}

bool DefinitionData::isDelimiter(char c) const
{
    return std::binary_search(delimiters.begin(), delimiters.end(), c);
}

Format DefinitionData::formatByName(const std::string& name) const
{
    const auto it = formats.find(name);
    if (it != formats.end())
        return it->second;

    return Format();
}

bool DefinitionData::isLoaded() const
{
    return !contexts.empty();
}

bool DefinitionData::load()
{
    if (isLoaded())
        return true;

    assert(!fileName.empty());
    xmlTextReaderPtr reader = xmlNewTextReaderFilename(fileName.c_str());
    if (! reader)
        return false; // could not open

    std::string name;
    while (xmlTextReaderRead(reader) == 1) {
        if (xmlTextReaderNodeType(reader) != 1 /* start element */)
            continue;

        name = regina::xml::xmlString(xmlTextReaderName(reader));
        if (name == "highlighting")
            loadHighlighting(reader);
        else if (name == "general")
            loadGeneral(reader);
    }
    xmlFreeTextReader(reader);

    for (auto it = keywordLists.begin(); it != keywordLists.end(); ++it)
        it->second->setCaseSensitivity(caseSensitive);

    for (auto context : contexts) {
        context->resolveContexts();
        context->resolveIncludes();
    }

    assert(std::is_sorted(delimiters.begin(), delimiters.end()));
    return true;
}

void DefinitionData::clear()
{
    // keep only name and repo, so we can re-lookup to make references persist over repo reloads
    keywordLists.clear();
    for (auto i : contexts)
        delete i;
    contexts.clear();
    formats.clear();

    fileName.clear();
    section.clear();
    style.clear();
    indenter.clear();
    author.clear();
    license.clear();
    delimiters = "\t !%&()*+,-./:;<=>?[\\]^{|}~"; // must be sorted!
    caseSensitive = true,
    version = 0.0f;
}

bool DefinitionData::loadMetaData(const std::string& definitionFileName)
{
    fileName = definitionFileName;

    xmlTextReaderPtr reader = xmlNewTextReaderFilename(fileName.c_str());
    if (! reader)
        return false; // could not open

    bool success = false;
    while (xmlTextReaderRead(reader) == 1) {
        if (xmlTextReaderNodeType(reader) == 1 /* start element */) {
            if (regina::xml::xmlString(xmlTextReaderName(reader)) == "language") {
                success = loadLanguage(reader);
                break;
            }
        }
    }
    // Either could not parse, or end of file.
    xmlFreeTextReader(reader);
    return success;
}

bool DefinitionData::loadLanguage(xmlTextReaderPtr reader)
{
    // These attributes default to empty strings.
    name = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"name"));
    section = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"section"));
    style = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"style"));
    indenter = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"indenter"));
    author = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"author"));
    license = regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"license"));

    // Since valueOf() does not have defaults, we must explicitly test validity.
    double ver;
    if (regina::valueOf(regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"version")), ver))
        version = ver;

    bool cs;
    if (regina::valueOf(regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"casesensitive")), cs))
        caseSensitive = cs;

    return true;
}

void DefinitionData::loadHighlighting(xmlTextReaderPtr reader)
{
    if (xmlTextReaderIsEmptyElement(reader))
        return;
    if (xmlTextReaderRead(reader) != 1)
        return;

    std::string name;
    while (true) {
        switch (xmlTextReaderNodeType(reader)) {
            case 1 /* start element */:
                name = regina::xml::xmlString(xmlTextReaderName(reader));
                if (name == "list") {
                    KeywordList* keywords = new KeywordList;
                    keywords->load(reader);
                    keywordLists.insert(std::make_pair(keywords->name(), std::shared_ptr<KeywordList>(keywords)));
                } else if (name == "contexts") {
                    loadContexts(reader);
                } else if (name == "itemDatas") {
                    loadItemData(reader);
                }
                if (xmlTextReaderRead(reader) != 1)
                    return;
                break;
            case 15 /* end element */:
                return;
            default:
                if (xmlTextReaderRead(reader) != 1)
                    return;
                break;
        }
    }
}

void DefinitionData::loadContexts(xmlTextReaderPtr reader)
{
    if (xmlTextReaderIsEmptyElement(reader))
        return;
    if (xmlTextReaderRead(reader) != 1)
        return;
    while (true) {
        switch (xmlTextReaderNodeType(reader)) {
            case 1 /* start element */:
                if (regina::xml::xmlString(xmlTextReaderName(reader)) == "context") {
                    auto context = new Context;
                    context->setDefinition(q);
                    context->load(reader);
                    contexts.push_back(context);
                }
                if (xmlTextReaderRead(reader) != 1)
                    return;
                break;
            case 15 /* end element */:
                return;
            default:
                if (xmlTextReaderRead(reader) != 1)
                    return;
                break;
        }
    }
}

void DefinitionData::loadItemData(xmlTextReaderPtr reader)
{
    if (xmlTextReaderIsEmptyElement(reader))
        return;
    if (xmlTextReaderRead(reader) != 1)
        return;

    while (true) {
        switch (xmlTextReaderNodeType(reader)) {
            case 1 /* start element */:
                if (regina::xml::xmlString(xmlTextReaderName(reader)) == "itemData") {
                    Format f;
                    auto formatData = FormatPrivate::get(f);
                    formatData->definition = q;
                    formatData->load(reader);
                    formatData->id = RepositoryPrivate::get(repo)->nextFormatId();
                    formats.insert(std::make_pair(f.name(), f));
                }
                if (xmlTextReaderNext(reader) != 1)
                    return;
                if (xmlTextReaderRead(reader) != 1)
                    return;
                break;
            case 15 /* end element */:
                return;
            default:
                if (xmlTextReaderRead(reader) != 1)
                    return;
                break;
        }
    }
}

void DefinitionData::loadGeneral(xmlTextReaderPtr reader)
{
    if (xmlTextReaderIsEmptyElement(reader))
        return;
    if (xmlTextReaderRead(reader) != 1)
        return;

    // reference counter to count XML child elements, to not return too early
    int elementRefCounter = 1;

    bool cs;
    while (true) {
        switch (xmlTextReaderNodeType(reader)) {
            case 1 /* start element */:
                ++elementRefCounter;

                if (regina::xml::xmlString(xmlTextReaderName(reader)) == "keywords") {
                    if (regina::valueOf(regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"casesensitive")), cs))
                        caseSensitive = cs;
                    delimiters += regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"additionalDeliminator"));
                    std::sort(delimiters.begin(), delimiters.end());
                    auto it = std::unique(delimiters.begin(), delimiters.end());
                    delimiters.resize(std::distance(delimiters.begin(), it));
                    for (char c : regina::xml::xmlString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"weakDeliminator")))
                        std::remove(delimiters.begin(), delimiters.end(), c);
                } else {
                    // Skip current element.
                    if (xmlTextReaderNext(reader) != 1)
                        return;
                }
                if (xmlTextReaderRead(reader) != 1)
                    return;
                break;
            case 15 /* end element */:
                --elementRefCounter;
                if (elementRefCounter == 0)
                    return;
            default:
                if (xmlTextReaderRead(reader) != 1)
                    return;
                break;
        }
    }
}

DefinitionRef::DefinitionRef()
{
}

DefinitionRef::DefinitionRef(const Definition &def) :
    d(def.d)
{
}

DefinitionRef::~DefinitionRef()
{
}

DefinitionRef& DefinitionRef::operator=(const Definition &def)
{
    d = def.d;
    return *this;
}

Definition DefinitionRef::definition() const
{
    if (!d.expired())
        return Definition(d.lock());
    return Definition();
}

} } // namespace regina::syntax
