
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
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

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "nscript.h"
    #include "nfile.h"
#else
    #include "engine/packet/nscript.h"
    #include "engine/file/nfile.h"
#endif

#define PROP_VARIABLE 1

const NString emptyString;

const NString& NScript::getVariableValue(const NString& name) const {
    NDynamicArrayIterator<NStringPair> it(variables);
    while (! it.done()) {
        if ((*it).first == name)
            return (*it).second;
        it++;
    }
    return emptyString;
}

long NScript::getVariableIndex(const NString& name) const {
    NDynamicArrayIterator<NStringPair> it(variables);
    while (! it.done()) {
        if ((*it).first == name)
            return it.getArrayIndex();
        it++;
    }
    return -1;
}

void NScript::removeVariable(const NString& name) {
    NDynamicArrayIterator<NStringPair> it(variables);
    while (! it.done()) {
        if ((*it).first == name)
            variables.remove(it);
        it++;
    }
}

void NScript::writeTextLong(ostream& o) const {
    if (variables.size() == 0)
        o << "No variables.\n";
    else {
        NDynamicArrayIterator<NStringPair> vit(variables);
        while (! vit.done()) {
            o << "Variable: " << (*vit).first << " = "
                << (*vit).second << '\n';
            vit++;
        }
    }

    o << '\n';

    NDynamicArrayIterator<NString> it(lines);
    while (! it.done()) {
        o << *it << '\n';
        it++;
    }
}

NPacket* NScript::internalClonePacket(NPacket*) const {
    NScript* ans = new NScript();

    NDynamicArrayIterator<NString> it(lines);
    while (! it.done()) {
        ans->lines.addLast(*it);
        it++;
    }

    NDynamicArrayIterator<NStringPair> vit(variables);
    while (! vit.done()) {
        ans->variables.addLast(*vit);
        vit++;
    }

    return ans;
}

void NScript::writePacket(NFile& out) const {
    out.writeULong(lines.size());
    NDynamicArrayIterator<NString> it(lines);
    while (! it.done()) {
        out.writeString(*it);
        it++;
    }

    // Write the properties.
    streampos bookmark(0);

    // The variables will be written as properties to allow for changing
    // of their representation in future file formats.

    NDynamicArrayIterator<NStringPair> vit(variables);
    while (! vit.done()) {
        bookmark = writePropertyHeader(out, PROP_VARIABLE);
        out.writeString((*vit).first);
        out.writeString((*vit).second);
        writePropertyFooter(out, bookmark);
        vit++;
    }

    // At the moment there are no properties to write!
    writeAllPropertiesFooter(out);
}

NScript* NScript::readPacket(NFile& in, NPacket*) {
    NScript* ans = new NScript();
    unsigned long size = in.readULong();
    for (unsigned long i=0; i<size; i++)
        ans->lines.addLast(in.readString());

    ans->readProperties(in);

    return ans;
}

void NScript::initialiseAllProperties() {
}

void NScript::readIndividualProperty(NFile& infile, unsigned propType) {
    if (propType == PROP_VARIABLE)
        variables.addLast(NStringPair(infile.readString(),
            infile.readString()));
}

