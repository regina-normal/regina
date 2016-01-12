
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
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

/* end stub */

#include <iterator>
#include "packet/nscript.h"
#include "utilities/xmlutils.h"

#define PROP_VARIABLE 1

namespace regina {

const std::string& NScript::getVariableName(size_t index) const {
    std::map<std::string, NPacket*>::const_iterator it = variables.begin();
    advance(it, index);
    return (*it).first;
}

NPacket* NScript::getVariableValue(size_t index) const {
    std::map<std::string, NPacket*>::const_iterator it = variables.begin();
    advance(it, index);
    return (*it).second;
}

NPacket* NScript::getVariableValue(const std::string& name) const {
    std::map<std::string, NPacket*>::const_iterator it = variables.find(name);
    if (it == variables.end())
        return 0;
    return (*it).second;
}

long NScript::getVariableIndex(const std::string& name) const {
    std::map<std::string, NPacket*>::const_iterator it = variables.find(name);
    if (it == variables.end())
        return -1;
    return distance(variables.begin(), it);
}

void NScript::setVariableName(size_t index, const std::string& name) {
    std::map<std::string, NPacket*>::iterator it = variables.begin();
    advance(it, index);

    if (name == it->first)
        return;

    ChangeEventSpan span(this);

    NPacket* value = it->second;
    variables.erase(it);
    variables.insert(std::make_pair(name, value));
}

void NScript::setVariableValue(size_t index, NPacket* value) {
    std::map<std::string, NPacket*>::iterator it = variables.begin();
    advance(it, index);

    if (it->second == value)
        return;

    ChangeEventSpan span(this);

    if (it->second)
        it->second->unlisten(this);
    it->second = value;
    if (it->second)
        it->second->listen(this);
}

void NScript::removeVariable(const std::string& name) {
    std::map<std::string, NPacket*>::iterator it = variables.find(name);
    if (it == variables.end())
        return;

    if (it->second)
        it->second->unlisten(this);

    ChangeEventSpan span(this);
    variables.erase(it);
}

void NScript::removeVariable(size_t index) {
    std::map<std::string, NPacket*>::iterator it = variables.begin();
    advance(it, index);

    if (it->second)
        it->second->unlisten(this);

    ChangeEventSpan span(this);
    variables.erase(it);
}

void NScript::writeTextLong(std::ostream& o) const {
    if (variables.empty())
        o << "No variables.\n";
    else {
        for (std::map<std::string, NPacket*>::const_iterator vit =
                variables.begin(); vit != variables.end(); vit++) {
            o << "Variable: " << vit->first << " = ";
            if (vit->second)
                o << vit->second->label() << '\n';
            else
                o << "(null)" << '\n';
        }
    }
    o << '\n' << text;
}

NPacket* NScript::internalClonePacket(NPacket*) const {
    NScript* ans = new NScript();
    ans->text = text;
    ans->variables = variables;
    return ans;
}

void NScript::writeXMLPacketData(std::ostream& out) const {
    using regina::xml::xmlEncodeSpecialChars;

    for (std::map<std::string, NPacket*>::const_iterator vit =
            variables.begin(); vit != variables.end(); vit++) {
        out << "  <var name=\"" << xmlEncodeSpecialChars((*vit).first)
            << "\" valueid=\"";
        if (vit->second)
            out << vit->second->internalID();
        out << "\" value=\"";
        if (vit->second)
            out << xmlEncodeSpecialChars(vit->second->label());
        out << "\"/>\n";
    }

    out << "  <text>" << xmlEncodeSpecialChars(text) << "</text>\n";
}

void NScript::packetWasRenamed(NPacket*) {
    // We assume that the packet that was renamed is one of the
    // variables for this packet.
    // There is nothing to update here; just fire the update.
    ChangeEventSpan span(this);
}

void NScript::packetToBeDestroyed(NPacket* packet) {
    // We know the script will change, because one of our variables is
    // listening on this packet.
    ChangeEventSpan span(this);
    for (std::map<std::string, NPacket*>::iterator vit =
            variables.begin(); vit != variables.end(); vit++)
        if (vit->second == packet)
            vit->second = 0;
}

} // namespace regina

