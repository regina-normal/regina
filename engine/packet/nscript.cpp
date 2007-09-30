
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

#include <iterator>
#include "packet/nscript.h"
#include "file/nfile.h"
#include "utilities/xmlutils.h"

#define PROP_VARIABLE 1

namespace regina {

namespace {
    const std::string emptyString;
}

const std::string& NScript::getVariableName(unsigned long index) const {
    std::map<std::string, std::string>::const_iterator it = variables.begin();
    advance(it, index);
    return (*it).first;
}

const std::string& NScript::getVariableValue(unsigned long index) const {
    std::map<std::string, std::string>::const_iterator it = variables.begin();
    advance(it, index);
    return (*it).second;
}

const std::string& NScript::getVariableValue(const std::string& name) const {
    std::map<std::string, std::string>::const_iterator it =
        variables.find(name);
    if (it == variables.end())
        return emptyString;
    return (*it).second;
}

void NScript::writeTextLong(std::ostream& o) const {
    if (variables.empty())
        o << "No variables.\n";
    else {
        for (std::map<std::string, std::string>::const_iterator vit =
                variables.begin(); vit != variables.end(); vit++)
            o << "Variable: " << (*vit).first << " = " << (*vit).second << '\n';
    }
    o << '\n';
    copy(lines.begin(), lines.end(),
        std::ostream_iterator<std::string>(o, "\n"));
}

NPacket* NScript::internalClonePacket(NPacket*) const {
    NScript* ans = new NScript();
    ans->lines = lines;
    ans->variables = variables;
    return ans;
}

void NScript::writePacket(NFile& out) const {
    out.writeULong(lines.size());
    for (std::vector<std::string>::const_iterator it = lines.begin();
            it != lines.end(); it++)
        out.writeString(*it);

    // Write the properties.
    std::streampos bookmark(0);

    // The variables will be written as properties to allow for changing
    // of their representation in future file formats.

    for (std::map<std::string, std::string>::const_iterator vit =
            variables.begin(); vit != variables.end(); vit++) {
        bookmark = out.writePropertyHeader(PROP_VARIABLE);
        out.writeString((*vit).first);
        out.writeString((*vit).second);
        out.writePropertyFooter(bookmark);
    }

    // At the moment there are no properties to write!
    out.writeAllPropertiesFooter();
}

NScript* NScript::readPacket(NFile& in, NPacket*) {
    NScript* ans = new NScript();
    unsigned long size = in.readULong();
    for (unsigned long i=0; i<size; i++)
        ans->lines.push_back(in.readString());

    in.readProperties(ans);

    return ans;
}

void NScript::writeXMLPacketData(std::ostream& out) const {
    using regina::xml::xmlEncodeSpecialChars;

    for (std::map<std::string, std::string>::const_iterator vit =
            variables.begin(); vit != variables.end(); vit++)
        out << "  <var name=\"" << xmlEncodeSpecialChars((*vit).first)
            << "\" value=\"" << xmlEncodeSpecialChars((*vit).second)
            << "\"/>\n";

    for (std::vector<std::string>::const_iterator it = lines.begin();
            it != lines.end(); it++)
        out << "  <line>" << xmlEncodeSpecialChars(*it) << "</line>\n";
}

void NScript::readIndividualProperty(NFile& infile, unsigned propType) {
    if (propType == PROP_VARIABLE)
        variables.insert(std::make_pair(infile.readString(),
            infile.readString()));
}

} // namespace regina

