
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "packet/nscript.h"
#include "file/nfile.h"

#define PROP_VARIABLE 1

static const NString emptyString;

const NString& NScript::getVariableName(unsigned long index) const {
    std::map<NString, NString>::const_iterator it = variables.begin();
    advance(it, index);
    return (*it).first;
}

const NString& NScript::getVariableValue(unsigned long index) const {
    std::map<NString, NString>::const_iterator it = variables.begin();
    advance(it, index);
    return (*it).second;
}

const NString& NScript::getVariableValue(const NString& name) const {
    std::map<NString, NString>::const_iterator it = variables.find(name);
    if (it == variables.end())
        return emptyString;
    return (*it).second;
}

void NScript::writeTextLong(std::ostream& o) const {
    if (variables.empty())
        o << "No variables.\n";
    else {
        for (std::map<NString, NString>::const_iterator vit =
                variables.begin(); vit != variables.end(); vit++)
            o << "Variable: " << (*vit).first << " = " << (*vit).second << '\n';
    }
    o << '\n';
    copy(lines.begin(), lines.end(), std::ostream_iterator<NString>(o, "\n"));
}

NPacket* NScript::internalClonePacket(NPacket*) const {
    NScript* ans = new NScript();
    ans->lines = lines;
    ans->variables = variables;
    return ans;
}

void NScript::writePacket(NFile& out) const {
    out.writeULong(lines.size());
    for (std::vector<NString>::const_iterator it = lines.begin();
            it != lines.end(); it++)
        out.writeString(*it);

    // Write the properties.
    streampos bookmark(0);

    // The variables will be written as properties to allow for changing
    // of their representation in future file formats.

    for (std::map<NString, NString>::const_iterator vit = variables.begin();
            vit != variables.end(); vit++) {
        bookmark = writePropertyHeader(out, PROP_VARIABLE);
        out.writeString((*vit).first);
        out.writeString((*vit).second);
        writePropertyFooter(out, bookmark);
    }

    // At the moment there are no properties to write!
    writeAllPropertiesFooter(out);
}

NScript* NScript::readPacket(NFile& in, NPacket*) {
    NScript* ans = new NScript();
    unsigned long size = in.readULong();
    for (unsigned long i=0; i<size; i++)
        ans->lines.push_back(in.readString());

    ans->readProperties(in);

    return ans;
}

void NScript::initialiseAllProperties() {
}

void NScript::readIndividualProperty(NFile& infile, unsigned propType) {
    if (propType == PROP_VARIABLE)
        variables.insert(make_pair(infile.readString(), infile.readString()));
}

