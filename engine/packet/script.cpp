
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <algorithm>
#include <iterator>
#include <sstream>
#include "packet/script.h"
#include "utilities/xmlutils.h"

#define PROP_VARIABLE 1

namespace regina {

Script& Script::operator = (const Script& src) {
    PacketChangeSpan span(*this);

    text_ = src.text_;
    variables_ = src.variables_;

    return *this;
}

void Script::swap(Script& other) {
    PacketChangeSpan span1(*this);
    PacketChangeSpan span2(other);

    text_.swap(other.text_);
    variables_.swap(other.variables_);
}

const std::string& Script::variableName(size_t index) const {
    auto it = variables_.begin();
    advance(it, index);
    return (*it).first;
}

std::shared_ptr<Packet> Script::variableValue(size_t index) const {
    auto it = variables_.begin();
    advance(it, index);
    return it->second.lock();
}

std::shared_ptr<Packet> Script::variableValue(const std::string& name) const {
    auto it = variables_.find(name);
    if (it == variables_.end())
        return nullptr;
    return it->second.lock();
}

long Script::variableIndex(const std::string& name) const {
    auto it = variables_.find(name);
    if (it == variables_.end())
        return -1;
    return distance(variables_.begin(), it);
}

void Script::setVariableName(size_t index, const std::string& name) {
    auto it = variables_.begin();
    advance(it, index);

    if (name == it->first)
        return;

    PacketChangeSpan span(*this);

    std::weak_ptr<Packet> value = std::move(it->second);
    variables_.erase(it);
    variables_.emplace(name, std::move(value));
}

void Script::setVariableValue(size_t index, std::weak_ptr<Packet> value) {
    auto it = variables_.begin();
    advance(it, index);

    PacketChangeSpan span(*this);
    it->second = std::move(value);
}

const std::string& Script::addVariableName(const std::string& name,
        std::weak_ptr<Packet> value) {
    PacketChangeSpan span(*this);

    auto result = variables_.emplace(name, value);
    int which = 2;
    while (! result.second) {
        std::ostringstream s;
        s << name << ' ' << which;
        result = variables_.emplace(s.str(), value);

        ++which;
    }

    return result.first->first;
}

void Script::removeVariable(const std::string& name) {
    auto it = variables_.find(name);
    if (it == variables_.end())
        return;

    PacketChangeSpan span(*this);
    variables_.erase(it);
}

void Script::removeVariable(size_t index) {
    auto it = variables_.begin();
    advance(it, index);

    PacketChangeSpan span(*this);
    variables_.erase(it);
}

bool Script::operator == (const Script& other) const {
    if (text_ != other.text_)
        return false;
    if (variables_.size() != other.variables_.size())
        return false;

    auto a = variables_.begin();
    auto b = other.variables_.begin();
    for ( ; a != variables_.end(); ++a, ++b) {
        if (a->first != b->first)
            return false;
        if (a->second.lock() != b->second.lock())
            return false;
    }

    return true;
}

void Script::writeTextShort(std::ostream& o) const {
    if (text_.empty())
        o << "Empty script";
    else {
        size_t lines = std::count(text_.begin(), text_.end(), '\n');
        if (lines > 0 && text_.back() != '\n')
            ++lines; // there is an extra unfinished line
        o << lines << "-line script";
    }
    if (variables_.empty())
        o << ", no variables";
    else {
        for (const auto& v : variables_) {
            o << ", " << v.first << " = ";
            if (auto shared = v.second.lock())
                o << '<' << shared->label() << '>';
            else
                o << "(null)";
        }
    }
}

void Script::writeTextLong(std::ostream& o) const {
    if (variables_.empty())
        o << "No variables.\n";
    else {
        for (const auto& v : variables_) {
            o << "Variable: " << v.first << " = ";
            if (auto shared = v.second.lock())
                o << shared->label() << '\n';
            else
                o << "(null)" << '\n';
        }
    }
    o << '\n' << text_;
}

std::shared_ptr<Packet> Script::internalClonePacket() const {
    auto ans = std::make_shared<Script>();
    ans->text_ = text_;
    ans->variables_ = variables_;
    return ans;
}

void Script::writeXMLPacketData(std::ostream& out, FileFormat format,
        bool anon, PacketRefs& refs) const {
    using regina::xml::xmlEncodeSpecialChars;

    writeXMLHeader(out, "script", format, anon, refs, true);

    for (const auto& v : variables_) {
        auto shared = v.second.lock();
        out << "  <var name=\"" << xmlEncodeSpecialChars(v.first)
            << "\" valueid=\"";
        if (shared)
            out << shared->internalID();
        out << "\" value=\"";
        if (shared)
            out << xmlEncodeSpecialChars(shared->label());
        out << "\"/>\n";
    }

    if (format == FileFormat::XmlGen2)
        out << "  <text>" << xmlEncodeSpecialChars(text_) << "</text>\n";
    else
        out << "  <code>" << xmlEncodeSpecialChars(text_) << "</code>\n";

    if (! anon)
        writeXMLTreeData(out, format, refs);
    writeXMLFooter(out, "script", format);
}

} // namespace regina

