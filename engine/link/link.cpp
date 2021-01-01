
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

#include "link.h"
#include "maths/numbertheory.h"
#include <algorithm>
#include <sstream>

namespace regina {

Link::Link(const Link& cloneMe, bool cloneProps) {
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

    if (! cloneProps)
        return;

    // Clone properties:
    if (cloneMe.jones_.known())
        jones_ = new Laurent<Integer>(*(cloneMe.jones_.value()));
    if (cloneMe.homflyAZ_.known())
        homflyAZ_ = new Laurent2<Integer>(*(cloneMe.homflyAZ_.value()));
    if (cloneMe.homflyLM_.known())
        homflyLM_ = new Laurent2<Integer>(*(cloneMe.homflyLM_.value()));
    if (cloneMe.bracket_.known())
        bracket_ = new Laurent<Integer>(*(cloneMe.bracket_.value()));
}

Link::Link(const std::string& description) {
    Link* attempt;

    if ((attempt = fromKnotSig(description))) {
        swapContents(*attempt);
        setLabel(description);
    } else if ((attempt = fromOrientedGauss(description))) {
        swapContents(*attempt);
        setLabel(description);
    } else if ((attempt = fromGauss(description))) {
        swapContents(*attempt);
        setLabel(description);
    } else if ((attempt = fromDT(description))) {
        swapContents(*attempt);
        setLabel(description);
    }

    delete attempt;
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

bool Link::isAlternating() const {
    StrandRef s;
    int prev;

    for (StrandRef start : components_) {
        // 0-crossing components are considered alternating.
        if (! start)
            continue;

        // Follow each non-empty component around.
        s = start;
        do {
            prev = s.strand();
            ++s;
            if (s.strand() == prev)
                return false;
        } while (s != start);
    }

    return true;
}

long Link::linking() const {
    if (crossings_.empty())
        return 0;

    // This algorithm is linear time.

    // Firat sum the signs of all crossings.
    long ans = 0;
    for (const Crossing* c : crossings_)
        ans += c->sign();

    // Now work through each component, and subtract off crossings that
    // involve that component twice.
    bool* seen = new bool[crossings_.size()];
    std::fill(seen, seen + crossings_.size(), false);

    StrandRef s;
    for (StrandRef start : components_) {
        if (! start)
            continue;

        s = start;

        do {
            if (seen[s.crossing()->index()])
                ans -= s.crossing()->sign();
            else
                seen[s.crossing()->index()] = true;
            ++s;
        } while (s != start);

        // Reset the seen[] array to all false.
        do {
            seen[s.crossing()->index()] = false;
            ++s;
        } while (s != start);
    }
    delete[] seen;

    return ans / 2;
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

void Link::swapContents(Link& other) {
    if (&other == this)
        return;

    ChangeEventSpan span1(this);
    ChangeEventSpan span2(&other);

    // Swap core data:
    crossings_.swap(other.crossings_);
    components_.swap(other.components_);

    // Swap properties:
    jones_.swap(other.jones_);
    homflyAZ_.swap(other.homflyAZ_);
    homflyLM_.swap(other.homflyLM_);
    bracket_.swap(other.bracket_);
    niceTreeDecomposition_.swap(other.niceTreeDecomposition_);
}

void Link::reflect() {
    ChangeEventSpan span(this);
    for (Crossing* cross : crossings_)
        cross->sign_ = -cross->sign_;

    clearAllProperties();
}

void Link::reverse() {
    ChangeEventSpan span(this);
    for (Crossing* cross : crossings_) {
        std::swap(cross->next_[0], cross->prev_[0]);
        std::swap(cross->next_[1], cross->prev_[1]);
    }

    clearAllProperties();
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

    clearAllProperties();
}

void Link::change(Crossing* c) {
    ChangeEventSpan span(this);

    for (StrandRef& s : components_)
        if (s.crossing_ == c)
            s.strand_ ^= 1;

    StrandRef s;

    // We need to ensure that the following code works in the case of
    // loop(s) at the given crossing.

    // 1. Flip next[...].strand bits from previous crossings.
    // In this code:
    // - s.strand is from a prev[] array, and has not been flipped;
    // - the next[...] array has not been reordered.
    s = c->prev_[0];
    s.crossing_->next_[s.strand_].strand_ ^= 1;
    s = c->prev_[1];
    s.crossing_->next_[s.strand_].strand_ ^= 1;

    // 2. Reorder next[] and prev[] arrays.
    std::swap(c->next_[0], c->next_[1]);
    std::swap(c->prev_[0], c->prev_[1]);

    // 3. Flip prev[...].strand bits from next crossings.
    // In this code:
    // - s.strand is from a next[] array, and has been flipped if necessary;
    // - the prev[...] array has been reordered if necessary.
    s = c->next_[0];
    s.crossing_->prev_[s.strand_].strand_ ^= 1;
    s = c->next_[1];
    s.crossing_->prev_[s.strand_].strand_ ^= 1;

    // Finally: the crossing sign will change.
    c->sign_ = -c->sign_;

    clearAllProperties();
}

void Link::changeAll() {
    ChangeEventSpan span(this);

    for (StrandRef& s : components_)
        s.strand_ ^= 1;

    int i;
    for (Crossing* c : crossings_) {
        std::swap(c->next_[0], c->next_[1]);
        std::swap(c->prev_[0], c->prev_[1]);
        for (i = 0; i < 2; ++i) {
            c->next_[i].strand_ ^= 1;
            c->prev_[i].strand_ ^= 1;
        }
        c->sign_ = - c->sign_;
    }

    clearAllProperties();
}

void Link::resolve(Crossing* c) {
    ChangeEventSpan span(this);

    if (c->next_[0].crossing() == c) {
        if (c->prev_[0].crossing() == c) {
            // This is a 1-crossing unknot component, and it resolves
            // into two 0-crossing unknot component.
            for (StrandRef& s : components_)
                if (s.crossing_ == c) {
                    // 0-crossing component #1:
                    s.crossing_ = nullptr;
                    s.strand_ = 0;
                    break;
                }
            // 0-crossing component #2:
            components_.push_back(StrandRef());

            crossings_.erase(crossings_.begin() + c->index());
            delete c;
        } else {
            // This is a twist: prev_[0] should connect to next_[1], and
            // we spin off a new 0-crossing unknot component.
            StrandRef from = c->prev_[0];
            StrandRef to = c->next_[1];
            from.crossing()->next_[from.strand()] = to;
            to.crossing()->prev_[to.strand()] = from;

            // Ensure that no component uses c as its starting point.
            for (StrandRef& s : components_)
                if (s.crossing_ == c) {
                    s = to;
                    break;
                }

            components_.push_back(StrandRef());
            crossings_.erase(crossings_.begin() + c->index());
            delete c;
        }
    } else if (c->prev_[0].crossing() == c) {
        // This is again a twist: prev_[1] should connect to next_[0], and
        // we spin off a new 0-crossing unknot component.
        StrandRef from = c->prev_[1];
        StrandRef to = c->next_[0];
        from.crossing()->next_[from.strand()] = to;
        to.crossing()->prev_[to.strand()] = from;

        // Ensure that no component uses c as its starting point.
        for (StrandRef& s : components_)
            if (s.crossing_ == c) {
                s = to;
                break;
            }

        components_.push_back(StrandRef());
        crossings_.erase(crossings_.begin() + c->index());
        delete c;
    } else {
        // This crossing does not connect to itself at all.
        // Depending on whether next_[0] is connected with next_[1], we either
        // break one component into two, or merge two components into one.

        // Ensure that no component uses c as its starting point.
        // Note that this could potentially happen twice.
        for (StrandRef& s : components_)
            if (s.crossing_ == c)
                ++s;

        // See whether c belongs to one or two components.
        std::vector<StrandRef>::iterator comp = components_.end();
        StrandRef s = c->next_[1];
        while (s.crossing_ != c) {
            if (comp == components_.end())
                comp = std::find(components_.begin(), components_.end(), s);
            ++s;
        }
        if (s.strand_ == 1) {
            // We walked all the way back to the same strand of c
            // without seeing c again in between - this means that c
            // belongs to two components.
            // Since we traversed one of these components entirely, it
            // must be stored in comp.
            // The two components will be merged as a result of this
            // operation, so we delete comp and keep the other (unknown)
            // component reference.
            components_.erase(comp);
        } else {
            // We returned to the other strand of c.
            // This means that c belongs entirely to a single component,
            // and as a result of this operation it will split into two
            // components.
            if (comp == components_.end()) {
                // The existing component marker must be between c->next_[0]
                // and c->prev_[1].
                components_.push_back(c->next_[1]);
            } else {
                // The existing component marker was found between c->next_[1]
                // and c->prev_[0].
                components_.push_back(c->next_[0]);
            }
        }

        // Merge the strands that need to be merged.
        StrandRef from = c->prev_[0];
        StrandRef to = c->next_[1];
        from.crossing()->next_[from.strand()] = to;
        to.crossing()->prev_[to.strand()] = from;

        from = c->prev_[1];
        to = c->next_[0];
        from.crossing()->next_[from.strand()] = to;
        to.crossing()->prev_[to.strand()] = from;

        // Finally, remove and delete the crossing.
        crossings_.erase(crossings_.begin() + c->index());
        delete c;
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

void Link::composeWith(const Link& other) {
    if (other.isEmpty())
        return;

    ChangeEventSpan span(this);

    // From here we can assume other is non-empty.
    // Clone its crossings, and transfer them directly into this link.
    // This abuses the MarkedVector API slightly (since an object must
    // not belong to more than one MarkedVector at a time), but the
    // implementation of MarkedVector does make it correct.
    Link clone(other);
    for (Crossing* c : clone.crossings_)
        crossings_.push_back(c);
    clone.crossings_.clear();

    if (isEmpty()) {
        // This link simply acquires all of clone's components.
        clone.components_.swap(components_);

        clearAllProperties();
        return;
    }

    // From here we assume that *both* links are non-empty.
    // We copy all compoments of clone except for the first.
    auto it = clone.components_.begin();
    for (++it; it != clone.components_.end(); ++it)
        components_.push_back(*it);

    // All that is left is to graft the first components of the two links.
    StrandRef graft = clone.components_.front();
    StrandRef src = components_.front();

    if (! graft) {
        // We are grafting in a 0-crossing unknot component: nothing to do.
    } else if (! src) {
        // We are grafting the other component into a 0-crossing unknot
        // component of *this* link.  Just replace the component entirely.
        *components_.begin() = graft;
    } else {
        // We are grafting two non-trivial components together.
        StrandRef graftEnd = graft.prev();
        join(src.prev(), graft); // changes graft.prev()
        join(graftEnd, src);
    }

    // All done!
    clearAllProperties();
}

Link* Link::parallel(int k, Framing framing) const {
    // Get the special cases out of the way.
    if (k == 0 || components_.empty())
        return new Link;
    if (k == 1)
        return new Link(*this);
    if (crossings_.empty())
        return new Link(components_.size() * k);

    Link* ans = new Link;
    Crossing** tmp = new Crossing*[k*k]; // Used to build grids of crossings

    // Crossing i of knot:
    //
    // +ve:    |                 -ve:    ^
    //     --- | --->                --- | --->
    //         v                         |
    //
    // Crossings (k^2 i, ..., k^2 (i+1) - 1) of this tangle:
    //
    //  k^2 i       | ... | k^2 (i+1) - k     k^2 i + k-1 ^ ... ^ k^2 (i+1) - 1
    //          --- | --- | --->                      --- | --- | --->
    //          ... | ... | ...                       ... | ... | ...
    //          --- | --- | --->                      --- | --- | --->
    //  k^2 i + k-1 v ... v k^2 (i+1) - 1     k^2 i       | ... | k^2 (i+1) - k

    // Create the k^2 crossings for each original, and join them
    // together internally.
    int i, j;
    for (Crossing* c : crossings_) {
        for (i = 0; i < k * k; ++i)
            ans->crossings_.push_back(tmp[i] = new Crossing(c->sign()));

        for (i = 0; i < k; ++i)
            for (j = 0; j < k - 1; ++j)
                Link::join(tmp[k*i + j]->upper(), tmp[k*i + j+1]->upper());
        for (i = 0; i < k - 1; ++i)
            for (j = 0; j < k; ++j)
                Link::join(tmp[k*i + j]->lower(), tmp[k*(i+1) + j]->lower());
    }

    // Walk around the original knot, and keep track of the left-hand
    // and right-hand crossings of the new tangle where we (i) enter the
    // grid configuration, and (ii) leave this configuration.

    StrandRef s;
    int idx;
    int enterL, exitL;
    int enterStrand, exitStrand;
    int enterDelta, exitDelta;
    int startL;
    int startStrand;
    int startDelta;

    int writhe;
    bool* seen = new bool[crossings_.size()];
    std::fill(seen, seen + crossings_.size(), false);

    for (const StrandRef& start : components_) {
        if (! start) {
            // This component is a 0-crossing unknot.
            for (i = 0; i < k; ++i)
                ans->components_.push_back(StrandRef());
            continue;
        }

        writhe = 0;
        s = start;
        exitL = -1;
        do {
            idx = s.crossing()->index();
            if (s.crossing()->sign() > 0) {
                if (s.strand() == 1) {
                    enterL = k*k * (idx+1) - k;
                    enterDelta = -k;
                } else {
                    enterL = k*k * idx;
                    enterDelta = 1;
                }
            } else {
                if (s.strand() == 1) {
                    enterL = k*k * idx;
                    enterDelta = k;
                } else {
                    enterL = k*k * idx + k-1;
                    enterDelta = -1;
                }
            }
            enterStrand = s.strand();

            // Connect the previous grid to this.
            if (exitL >= 0) {
                for (i = 0; i < k; ++i)
                    Link::join(
                        ans->crossings_[exitL + i * exitDelta]->
                            strand(exitStrand),
                        ans->crossings_[enterL + i * enterDelta]->
                            strand(enterStrand));
            } else {
                startL = enterL;
                startDelta = enterDelta;
                startStrand = enterStrand;
            }

            exitL = enterL + (k-1) * (s.strand() == 1 ? 1 : k);
            exitDelta = enterDelta;
            exitStrand = enterStrand;

            if (seen[s.crossing()->index()])
                writhe += s.crossing()->sign();
            else
                seen[s.crossing()->index()] = true;

            ++s;
        } while (s != start);

        // Reset the seen[] array to all false.
        do {
            seen[s.crossing()->index()] = false;
            ++s;
        } while (s != start);

        if (writhe == 0 || framing == FRAMING_BLACKBOARD) {
            // Close up the k new parallel link components.
            for (i = 0; i < k; ++i)
                Link::join(
                    ans->crossings_[exitL + i * exitDelta]->
                        strand(exitStrand),
                    ans->crossings_[startL + i * startDelta]->
                        strand(startStrand));
        } else if (writhe > 0) {
            // We want the Seifert framing, and the writhe is positive.
            // Insert the requisite number of negative twists
            // before closing off the k parallel link components.
            for (i = 0; i < writhe * k; ++i) {
                for (j = 0; j < k - 1; ++j)
                    ans->crossings_.push_back(tmp[j] = new Crossing(-1));

                for (j = 0; j < k - 2; ++j)
                    Link::join(tmp[j]->lower(), tmp[j + 1]->lower());

                if (i == 0) {
                    Link::join(
                        ans->crossings_[exitL]->strand(exitStrand),
                        tmp[0]->lower());
                    for (j = 1; j < k; ++j)
                        Link::join(
                            ans->crossings_[exitL + j * exitDelta]->
                                strand(exitStrand),
                            tmp[j - 1]->upper());
                } else {
                    Link::join(
                        ans->crossings_[exitL]->upper(), tmp[0]->lower());
                    for (j = 1; j < k - 1; ++j)
                        Link::join(
                            ans->crossings_[exitL + j]->upper(),
                            tmp[j - 1]->upper());
                    Link::join(
                        ans->crossings_[exitL + (k - 2)]->lower(),
                        tmp[k - 2]->upper());
                }

                exitL = tmp[0]->index();
            }

            for (j = 0; j < k - 1; ++j)
                Link::join(
                    ans->crossings_[exitL + j]->upper(),
                    ans->crossings_[startL + j * startDelta]->
                        strand(startStrand));
            Link::join(
                ans->crossings_[exitL + (k - 2)]->lower(),
                ans->crossings_[startL + (k - 1) * startDelta]->
                    strand(startStrand));
        } else {
            // We want the Seifert framing, and the writhe is negative.
            // Insert the requisite number of positive twists
            // before closing off the k parallel link components.
            for (i = 0; i < (-writhe) * k; ++i) {
                for (j = 0; j < k - 1; ++j)
                    ans->crossings_.push_back(tmp[j] = new Crossing(1));

                for (j = 0; j < k - 2; ++j)
                    Link::join(tmp[j]->upper(), tmp[j + 1]->upper());

                if (i == 0) {
                    Link::join(
                        ans->crossings_[exitL]->strand(exitStrand),
                        tmp[0]->upper());
                    for (j = 1; j < k; ++j)
                        Link::join(
                            ans->crossings_[exitL + j * exitDelta]->
                                strand(exitStrand),
                            tmp[j - 1]->lower());
                } else {
                    Link::join(
                        ans->crossings_[exitL]->lower(), tmp[0]->upper());
                    for (j = 1; j < k - 1; ++j)
                        Link::join(
                            ans->crossings_[exitL + j]->lower(),
                            tmp[j - 1]->lower());
                    Link::join(
                        ans->crossings_[exitL + (k - 2)]->upper(),
                        tmp[k - 2]->lower());
                }

                exitL = tmp[0]->index();
            }

            for (j = 0; j < k - 1; ++j)
                Link::join(
                    ans->crossings_[exitL + j]->lower(),
                    ans->crossings_[startL + j * startDelta]->
                        strand(startStrand));
            Link::join(
                ans->crossings_[exitL + (k - 2)]->upper(),
                ans->crossings_[startL + (k - 1) * startDelta]->
                    strand(startStrand));
        }

        // Take note of the k new link components.
        for (i = 0; i < k; ++i)
            ans->components_.push_back(
                ans->crossings_[startL + i * startDelta]->
                    strand(startStrand));
    }

    delete[] seen;
    delete[] tmp;
    return ans;
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

std::string Link::pace() const {
    std::ostringstream out;
    writePACE(out);
    return out.str();
}

void Link::writePACE(std::ostream& out) const {
    if (! label().empty())
        out << "c " << label() << std::endl;
    out << "p tw " << size() << ' ' << (size() * 2) << std::endl;

    int i;
    const Crossing* adj;
    for (const Crossing* c : crossings_) {
        for (i = 0; i < 2; ++i) {
            adj = c->next(i).crossing();
            if (adj->index() >= c->index())
                out << (c->index() + 1) << ' ' << (adj->index() + 1);
            else
                out << (adj->index() + 1) << ' ' << (c->index() + 1);
            out << std::endl;
        }
    }
}

void Link::prepareTreeDecomposition(TreeDecomposition& td) const {
    optimiseForJones(td);

    // For each crossing, we compute how many steps forward we take from
    // its upper strand before we enter some crossing at the lower strand.
    //
    // For those crossings with more such steps, we will try to forget
    // them closer to the root bag of our nice tree decomposition.
    int* upperSteps = new int[size()];
    StrandRef s;
    int steps;
    for (StrandRef start : components_) {
        if (! start)
            continue;

        // Find a lower strand to traverse this component from.
        // If the component has no lower strand at all, then we will
        // just come back around to the original starting point.
        s = start;
        do {
            if (s.strand() == 0)
                break;
            ++s;
        } while (s != start);

        // We now traverse the component backwards from here.
        start = s;
        do {
            if (s.strand() == 0)
                steps = 0;
            else
                upperSteps[s.crossing()->index()] = ++steps;
            --s;
        } while (s != start);
    }

    td.makeNice(upperSteps);

    delete[] upperSteps;
}

void Link::insertTorusLink(int p, int q, bool positive) {
    if (p < q)
        std::swap(p, q);

    // We have p >= q.
    if (q == 0) {
        ChangeEventSpan span(this);
        if (p == 0) {
            // Insert a single unknot.
            components_.push_back({});
        } else {
            // Insert p disjoint unknots.
            for (int i = 0; i < p; ++i)
                components_.push_back({});
        }
        clearAllProperties();
        return;
    }
    if (q == 1) {
        // Insert a single unknot.
        ChangeEventSpan span(this);
        components_.push_back({});
        clearAllProperties();
        return;
    }

    // We now have 1 < q <= p.
    // Use the standard diagram with p(q-1) crossings.

    int n = p * (q - 1);
    int nComp = regina::gcd(p, q);

    ChangeEventSpan span(this);

    Crossing** c = new Crossing*[n];
    int i;
    for (i = 0; i < n; ++i) {
        c[i] = new Crossing(positive ? 1 : -1);
        crossings_.push_back(c[i]);
    }

    int sliceIdx, nextIdx;
    for (int slice = 0; slice < p; ++slice) {
        sliceIdx = slice * (q - 1);
        nextIdx = ((slice == p - 1) ? 0 : (sliceIdx + q - 1));
        join({c[sliceIdx], 0}, {c[nextIdx], 1});
        for (i = 0; i < q - 2; ++i) {
            join({c[sliceIdx + i], 1}, {c[sliceIdx + i + 1], 1});
            join({c[sliceIdx + i + 1], 0}, {c[nextIdx + i], 0});
        }
        join({c[sliceIdx + q - 2], 1}, {c[nextIdx + q - 2], 0});

        if (slice < nComp)
            components_.push_back({c[sliceIdx], 1});
    }

    delete[] c;
    clearAllProperties();
}

} // namespace regina

