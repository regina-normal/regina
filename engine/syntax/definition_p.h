
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
 * \file syntax/definition_p.h
 * \brief Part of the private syntax highlighting framework that is shared
 * between Regina's different graphical user interfaces.
 *
 * The syntax highlighting headers are private in the following sense: they are
 * not installed along with Regina's other headers, they do not appear in the
 * API documentation, and the syntax highlighting API is subject to change
 * without notice between different versions of Regina.
 */

#ifndef __SYNTAX_DEFINITION_P_H
#ifndef __DOXYGEN
#define __SYNTAX_DEFINITION_P_H
#endif

#include "regina-core.h"
#include "syntax/definitionref.h"
#include "syntax/keywordlist.h"

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <libxml/xmlreader.h>

namespace regina {
namespace syntax {

class Definition;
class Repository;

class REGINA_API DefinitionData : boost::noncopyable
{
public:
    DefinitionData();
    ~DefinitionData();

    static DefinitionData* get(const Definition &def);

    bool isLoaded() const;
    bool loadMetaData(const std::string& definitionFileName);

    void clear();

    bool load();
    bool loadLanguage(xmlTextReaderPtr reader);
    void loadHighlighting(xmlTextReaderPtr reader);
    void loadContexts(xmlTextReaderPtr reader);
    void loadItemData(xmlTextReaderPtr reader);
    void loadGeneral(xmlTextReaderPtr reader);

    const KeywordList& keywordList(const std::string& name) const;
    bool isDelimiter(char c) const;

    Context* initialContext() const;
    Context* contextByName(const std::string& name) const;

    Format formatByName(const std::string& name) const;

    DefinitionRef q;

    Repository *repo;
    std::map<std::string, std::shared_ptr<KeywordList>> keywordLists;
    std::vector<Context*> contexts;
    std::map<std::string, Format> formats;
    std::string delimiters;

    std::string fileName;
    std::string name;
    std::string section;
    std::string style;
    std::string indenter;
    std::string author;
    std::string license;
    bool caseSensitive;
    int version;
};

} } // namespace regina::syntax

#endif
