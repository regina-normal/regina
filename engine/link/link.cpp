
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

#include "link.h"
#include <sstream>

namespace regina {

Link::Link(const Link& cloneMe) {
    for (Crossing* c : cloneMe.crossings_)
        crossings_.push_back(new Crossing(c->sign()));

    auto it = cloneMe.crossings_.begin();
    for (Crossing* c : crossings_) {
        for (int i = 0; i < 2; ++i) {
            c->next_[i] = translate((*it)->next_[i]);
            c->prev_[i] = translate((*it)->prev_[i]);
        }
        ++it;
    }

    for (const StrandRef& comp : cloneMe.components_)
        components_.push_back(translate(comp));
}

bool Link::connected(const Crossing* a, const Crossing* b) const {
    if (components_.size() <= 1)
        return true;

    // Do a depth-first search.
    size_t n = crossings_.size();

    bool* visited = new bool[n];
    Crossing const** stack = new Crossing const*[n];

    std::fill(visited, visited + n, false);

    size_t stackSize = 1;
    stack[0] = a;
    visited[a->index()] = true;

    const Crossing *curr, *adj;
    int i;
    while (stackSize > 0 && ! visited[b->index()]) {
        curr = stack[--stackSize];

        for (i = 0; i < 2; ++i) {
            adj = curr->next_[i].crossing();
            if (! visited[adj->index()]) {
                visited[adj->index()] = true;
                stack[stackSize++] = adj;
            }

            adj = curr->prev_[i].crossing();
            if (! visited[adj->index()]) {
                visited[adj->index()] = true;
                stack[stackSize++] = adj;
            }
        }
    }

    bool ans = visited[b->index()];

    delete[] stack;
    delete[] visited;
    return ans;
}

void Link::writeTextShort(std::ostream& out) const {
    if (components_.empty())
        out << "empty link";
    else if (components_.size() == 1)
        out << crossings_.size() << "-crossing knot";
    else
        out << crossings_.size() << "-crossing, "
            << components_.size() << "-component link";
}

void Link::writeTextLong(std::ostream& out) const {
    if (components_.empty()) {
        out << "Empty link" << std::endl;
        return;
    }

    if (components_.size() == 1)
        out << crossings_.size() << "-crossing knot";
    else
        out << crossings_.size() << "-crossing, "
            << components_.size() << "-component link";
    out << "\n\n";

    int comp = 0;
    StrandRef s;
    for (StrandRef start : components_) {
        out << "Component " << comp++ << ": ";
        if (! start) {
            out << "no crossings (separate unknot)" << std::endl;
            continue;
        }
        s = start;
        out << s;
        for (++s; s != start; ++s)
            out << ' ' << s;
        out << '\n';
    }

    out << "\nCrossings:";
    for (Crossing* c : crossings_)
        out << ' ' << (c->sign() > 0 ? '+' : '-') << c->index();
    out << std::endl;
}

void Link::reflect() {
    ChangeEventSpan span(this);
    for (Crossing* cross : crossings_)
        cross->sign_ = -cross->sign_;
}

void Link::rotate() {
    ChangeEventSpan span(this);

    for (StrandRef& s : components_)
        s.strand_ ^= 1;

    for (Crossing* cross : crossings_) {
        std::swap(cross->next_[0], cross->next_[1]);
        std::swap(cross->prev_[0], cross->prev_[1]);
        cross->next_[0].strand_ ^= 1;
        cross->next_[1].strand_ ^= 1;
        cross->prev_[0].strand_ ^= 1;
        cross->prev_[1].strand_ ^= 1;
    }
}

std::string Link::brief() const {
    if (components_.empty())
        return std::string();

    if (crossings_.empty()) {
        std::ostringstream out;
        out << "( )";
        for (size_t i = 1; i < components_.size(); ++i)
            out << " ( )";
        return out.str();
    }

    std::ostringstream out;
    for (Crossing* c : crossings_)
        out << (c->sign() > 0 ? '+' : '-');

    StrandRef s;
    for (StrandRef start : components_) {
        out << " ( ";
        if (! start) {
            out << ')';
        } else {
            s = start;
            out << s;
            for (++s; s != start; ++s)
                out << ' ' << s;
            out << " )";
        }
    }

    return out.str();
}

std::string Link::dumpConstruction() const {
    std::ostringstream out;

    out <<
"/**\n";
    if (! label().empty()) {
        out <<
" * Link: " << label() << "\n";
    } else {
        out <<
" * Link:\n";
    }
    out <<
" * Code automatically generated by dumpConstruction().\n"
" */\n"
"\n"
"Link* link = Link::fromData(";

    out << "{ ";
    if (crossings_.empty())
        out << '}';
    else {
        auto it = crossings_.begin();
        out << ((*it)->sign() > 0 ? "+1" : "-1");

        for (++it; it != crossings_.end(); ++it)
            out << ", " << ((*it)->sign() > 0 ? "+1" : "-1");
        out << " }";
    }

    StrandRef s;
    for (StrandRef start : components_) {
        out << ", { ";
        if (! start)
            out << '}';
        else {
            s = start;
            if (s.strand() == 0)
                out << '-';
            out << (s.crossing()->index() + 1);

            for (++s; s != start; ++s) {
                out << ", ";
                if (s.strand() == 0)
                    out << '-';
                out << (s.crossing()->index() + 1);
            }
            out << " }";
        }
    }

    out << ");\n";
    return out.str();
}

void Link::writeXMLPacketData(std::ostream& out) const {
    StrandRef s;

    out << "  <crossings size=\"" << crossings_.size() << "\">\n ";
    for (const Crossing* c : crossings_)
        out << ' ' << (c->sign() == 1 ? '+' : '-');
    out << "\n  </crossings>\n";
    out << "  <connections>\n";
    for (const Crossing* c : crossings_)
        out << "  " << c->next_[1] << ' ' << c->next_[0] << '\n';
    out << "  </connections>\n";
    out << "  <components size=\"" << components_.size() << "\">\n ";
    for (const StrandRef& s : components_)
        out << ' ' << s;
    out << "\n  </components>\n";
}

} // namespace regina

