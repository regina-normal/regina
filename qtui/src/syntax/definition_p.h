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

#ifndef KSYNTAXHIGHLIGHTING_DEFINITION_P_H
#define KSYNTAXHIGHLIGHTING_DEFINITION_P_H

#include "definitionref_p.h"

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <libxml/xmlreader.h>

namespace KSyntaxHighlighting {

class Definition;
class Repository;

class DefinitionData : boost::noncopyable
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
}

#endif
