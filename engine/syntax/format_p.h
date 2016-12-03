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

#ifndef __SYNTAX_FORMAT_P_H
#ifndef __DOXYGEN
#define __SYNTAX_FORMAT_P_H
#endif

#include "syntax/definitionref.h"
#include "syntax/textstyledata_p.h"
#include "syntax/theme.h"

#include <string>
#include <boost/noncopyable.hpp>
#include <libxml/xmlreader.h>

namespace regina {
namespace syntax {

class FormatPrivate : public boost::noncopyable
{
public:
    FormatPrivate();
    static FormatPrivate* get(const Format &format);

    void load(xmlTextReaderPtr reader);

    DefinitionRef definition;
    std::string name;
    TextStyleData style;
    Theme::TextStyle defaultStyle;
    uint16_t id;
    bool spellCheck;
};

} } // namespace regina::syntax

#endif
