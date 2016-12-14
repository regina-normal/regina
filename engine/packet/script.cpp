
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <iterator>
#include <sstream>
#include "packet/script.h"
#include "utilities/xmlutils.h"

#define PROP_VARIABLE 1

namespace regina {

const std::string& Script::variableName(size_t index) const {
    std::map<std::string, Packet*>::const_iterator it = variables.begin();
    advance(it, index);
    return (*it).first;
}

Packet* Script::variableValue(size_t index) const {
    std::map<std::string, Packet*>::const_iterator it = variables.begin();
    advance(it, index);
    return (*it).second;
}

Packet* Script::variableValue(const std::string& name) const {
    std::map<std::string, Packet*>::const_iterator it = variables.find(name);
    if (it == variables.end())
        return 0;
    return (*it).second;
}

long Script::variableIndex(const std::string& name) const {
    std::map<std::string, Packet*>::const_iterator it = variables.find(name);
    if (it == variables.end())
        return -1;
    return distance(variables.begin(), it);
}

void Script::setVariableName(size_t index, const std::string& name) {
    std::map<std::string, Packet*>::iterator it = variables.begin();
    advance(it, index);

    if (name == it->first)
        return;

    ChangeEventSpan span(this);

    Packet* value = it->second;
    variables.erase(it);
    variables.insert(std::make_pair(name, value));
}

void Script::setVariableValue(size_t index, Packet* value) {
    std::map<std::string, Packet*>::iterator it = variables.begin();
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

const std::string& Script::addVariableName(const std::string& name,
        Packet* value) {
    ChangeEventSpan span(this);

    auto result = variables.insert(std::make_pair(name, value));
    int which = 2;
    while (! result.second) {
        std::ostringstream s;
        s << name << ' ' << which;
        result = variables.insert(std::make_pair(s.str(), value));

        ++which;
    }

    if (value)
        value->listen(this);
    return result.first->first;
}

void Script::removeVariable(const std::string& name) {
    std::map<std::string, Packet*>::iterator it = variables.find(name);
    if (it == variables.end())
        return;

    if (it->second)
        it->second->unlisten(this);

    ChangeEventSpan span(this);
    variables.erase(it);
}

void Script::removeVariable(size_t index) {
    std::map<std::string, Packet*>::iterator it = variables.begin();
    advance(it, index);

    if (it->second)
        it->second->unlisten(this);

    ChangeEventSpan span(this);
    variables.erase(it);
}

void Script::writeTextLong(std::ostream& o) const {
    if (variables.empty())
        o << "No variables.\n";
    else {
        for (std::map<std::string, Packet*>::const_iterator vit =
                variables.begin(); vit != variables.end(); vit++) {
            o << "Variable: " << vit->first << " = ";
            if (vit->second)
                o << vit->second->label() << '\n';
            else
                o << "(null)" << '\n';
        }
    }
    o << '\n' << text_;
}

Packet* Script::internalClonePacket(Packet*) const {
    Script* ans = new Script();
    ans->text_ = text_;
    ans->variables = variables;
    return ans;
}

void Script::writeXMLPacketData(std::ostream& out) const {
    using regina::xml::xmlEncodeSpecialChars;

    for (std::map<std::string, Packet*>::const_iterator vit =
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

    out << "  <text>" << xmlEncodeSpecialChars(text_) << "</text>\n";
}

void Script::packetWasRenamed(Packet*) {
    // We assume that the packet that was renamed is one of the
    // variables for this packet.
    // There is nothing to update here; just fire the update.
    ChangeEventSpan span(this);
}

void Script::packetToBeDestroyed(Packet* packet) {
    // We know the script will change, because one of our variables is
    // listening on this packet.
    ChangeEventSpan span(this);
    for (std::map<std::string, Packet*>::iterator vit =
            variables.begin(); vit != variables.end(); vit++)
        if (vit->second == packet)
            vit->second = 0;
}

} // namespace regina

