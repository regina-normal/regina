
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

Script& Script::operator = (const Script& src) {
    ChangeEventSpan span(*this);

    text_ = src.text_;
    variables_ = src.variables_;

    PacketListener::operator = (src);

    return *this;
}

void Script::swap(Script& other) {
    ChangeEventSpan span1(*this);
    ChangeEventSpan span2(other);

    text_.swap(other.text_);
    variables_.swap(other.variables_);

    swapListeners(other);
}

const std::string& Script::variableName(size_t index) const {
    std::map<std::string, Packet*>::const_iterator it = variables_.begin();
    advance(it, index);
    return (*it).first;
}

std::shared_ptr<Packet> Script::variableValue(size_t index) const {
    std::map<std::string, Packet*>::const_iterator it = variables_.begin();
    advance(it, index);
    return (*it).second->shared_from_this();
}

std::shared_ptr<Packet> Script::variableValue(const std::string& name) const {
    std::map<std::string, Packet*>::const_iterator it = variables_.find(name);
    if (it == variables_.end())
        return 0;
    return (*it).second->shared_from_this();
}

long Script::variableIndex(const std::string& name) const {
    std::map<std::string, Packet*>::const_iterator it = variables_.find(name);
    if (it == variables_.end())
        return -1;
    return distance(variables_.begin(), it);
}

void Script::setVariableName(size_t index, const std::string& name) {
    std::map<std::string, Packet*>::iterator it = variables_.begin();
    advance(it, index);

    if (name == it->first)
        return;

    ChangeEventSpan span(*this);

    Packet* value = it->second;
    variables_.erase(it);
    variables_.insert(std::make_pair(name, value));
}

void Script::setVariableValue(size_t index, std::shared_ptr<Packet> value) {
    std::map<std::string, Packet*>::iterator it = variables_.begin();
    advance(it, index);

    if (it->second == value.get())
        return;

    ChangeEventSpan span(*this);

    if (it->second)
        it->second->unlisten(this);
    it->second = value.get();
    if (it->second)
        it->second->listen(this);
}

const std::string& Script::addVariableName(const std::string& name,
        std::shared_ptr<Packet> value) {
    ChangeEventSpan span(*this);

    auto result = variables_.insert(std::make_pair(name, value.get()));
    int which = 2;
    while (! result.second) {
        std::ostringstream s;
        s << name << ' ' << which;
        result = variables_.insert(std::make_pair(s.str(), value.get()));

        ++which;
    }

    if (value)
        value->listen(this);
    return result.first->first;
}

void Script::removeVariable(const std::string& name) {
    std::map<std::string, Packet*>::iterator it = variables_.find(name);
    if (it == variables_.end())
        return;

    if (it->second)
        it->second->unlisten(this);

    ChangeEventSpan span(*this);
    variables_.erase(it);
}

void Script::removeVariable(size_t index) {
    std::map<std::string, Packet*>::iterator it = variables_.begin();
    advance(it, index);

    if (it->second)
        it->second->unlisten(this);

    ChangeEventSpan span(*this);
    variables_.erase(it);
}

void Script::writeTextLong(std::ostream& o) const {
    if (variables_.empty())
        o << "No variables.\n";
    else {
        for (const auto& v : variables_) {
            o << "Variable: " << v.first << " = ";
            if (v.second)
                o << v.second->label() << '\n';
            else
                o << "(null)" << '\n';
        }
    }
    o << '\n' << text_;
}

std::shared_ptr<Packet> Script::internalClonePacket(std::shared_ptr<Packet>)
        const {
    auto ans = std::make_shared<Script>();
    ans->text_ = text_;
    ans->variables_ = variables_;
    return ans;
}

void Script::writeXMLPacketData(std::ostream& out, FileFormat format,
        bool anon, PacketRefs& refs) const {
    using regina::xml::xmlEncodeSpecialChars;

    writeXMLHeader(out, "script", format, anon, refs);

    for (const auto& v : variables_) {
        out << "  <var name=\"" << xmlEncodeSpecialChars(v.first)
            << "\" valueid=\"";
        if (v.second)
            out << v.second->internalID();
        out << "\" value=\"";
        if (v.second)
            out << xmlEncodeSpecialChars(v.second->label());
        out << "\"/>\n";
    }

    if (format == REGINA_XML_GEN_2)
        out << "  <text>" << xmlEncodeSpecialChars(text_) << "</text>\n";
    else
        out << "  <code>" << xmlEncodeSpecialChars(text_) << "</code>\n";

    if (! anon)
        writeXMLTreeData(out, format, refs);
    writeXMLFooter(out, "script", format);
}

void Script::packetWasRenamed(Packet*) {
    // We assume that the packet that was renamed is one of the
    // variables for this packet.
    // There is nothing to update here; just fire the update.
    ChangeEventSpan span(*this);
}

void Script::packetToBeDestroyed(PacketShell packet) {
    // We know the script will change, because one of our variables is
    // listening on this packet.
    ChangeEventSpan span(*this);
    for (auto& v : variables_)
        if (v.second == packet)
            v.second = nullptr;
}

} // namespace regina

