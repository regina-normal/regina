
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#include "link.h"
#include "maths/numbertheory.h"
#include "triangulation/dim3.h"
#include <algorithm>
#include <numeric> // for std::gcd()
#include <sstream>

namespace regina {

Link::Link(const Link& cloneMe, bool cloneProps) : virtualGenus_(-1) {
    crossings_.reserve(cloneMe.crossings_.size());
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

    components_.reserve(cloneMe.components_.size());
    for (const StrandRef& comp : cloneMe.components_)
        components_.push_back(translate(comp));

    if (! cloneProps)
        return;

    // Clone properties:
    virtualGenus_ = cloneMe.virtualGenus_;
    alexander_ = cloneMe.alexander_;
    jones_ = cloneMe.jones_;
    homflyAZ_ = cloneMe.homflyAZ_;
    homflyLM_ = cloneMe.homflyLM_;
    bracket_ = cloneMe.bracket_;
    niceTreeDecomposition_ = cloneMe.niceTreeDecomposition_;
}

Link::Link(Link&& src) noexcept :
        components_(std::move(src.components_)),
        virtualGenus_(src.virtualGenus_),
        alexander_(std::move(src.alexander_)),
        jones_(std::move(src.jones_)),
        homflyLM_(std::move(src.homflyLM_)),
        homflyAZ_(std::move(src.homflyAZ_)),
        bracket_(std::move(src.bracket_)),
        niceTreeDecomposition_(std::move(src.niceTreeDecomposition_)) {
    // We need src.crossings_ to be empty, so that src's destructor does not
    // do anything unexpected.  Ensure this by using a swap instead of a move.
    crossings_.swap(src.crossings_);
}

Link& Link::operator = (const Link& src) {
    if (std::addressof(src) == this)
        return *this;

    // We use a basic PacketChangeSpan here, not a richer ChangeAndClearSpan,
    // since we do not want to touch computed properties.  Our intention here
    // is to clone them, not clear them.
    PacketChangeSpan span(*this);

    for (Crossing* c : crossings_)
        delete c;

    crossings_.clear();
    components_.clear();

    crossings_.reserve(src.crossings_.size());
    for (Crossing* c : src.crossings_)
        crossings_.push_back(new Crossing(c->sign()));

    auto it = src.crossings_.begin();
    for (Crossing* c : crossings_) {
        for (int i = 0; i < 2; ++i) {
            c->next_[i] = translate((*it)->next_[i]);
            c->prev_[i] = translate((*it)->prev_[i]);
        }
        ++it;
    }

    components_.reserve(src.components_.size());
    for (const StrandRef& comp : src.components_)
        components_.push_back(translate(comp));

    // Do not touch TopologyLockable members.  (See TopologyLockable for why.)

    // Clone properties:
    virtualGenus_ = src.virtualGenus_;
    alexander_ = src.alexander_;
    jones_ = src.jones_;
    homflyAZ_ = src.homflyAZ_;
    homflyLM_ = src.homflyLM_;
    bracket_ = src.bracket_;
    niceTreeDecomposition_ = src.niceTreeDecomposition_;

    return *this;
}

bool Link::operator == (const Link& other) const {
    if (crossings_.size() != other.crossings_.size())
        return false;
    if (components_.size() != other.components_.size())
        return false;

    for (size_t i = 0; i < crossings_.size(); ++i) {
        Crossing* a = crossings_[i];
        Crossing* b = other.crossings_[i];

        if (a->sign() != b->sign())
            return false;
        if (a->next_[0] != translate(b->next_[0]))
            return false;
        if (a->next_[1] != translate(b->next_[1]))
            return false;

        // If everything is self-consistent then the prev strands should
        // match also; we don't need to test those.
    }

    for (size_t i = 0; i < components_.size(); ++i) {
        if (components_[i] != translate(other.components_[i]))
            return false;
    }

    return true;
}

Link& Link::operator = (Link&& src) {
    // We use a basic PacketChangeSpan here, not a richer ChangeAndClearSpan,
    // since we do not want to touch computed properties.  Our intention here
    // is to move them, not clear them.
    PacketChangeSpan span(*this);

    // MarkedVector, pointers must eventually be destroyed:
    crossings_.swap(src.crossings_);

    // std::vector, does not own its pointers:
    components_ = std::move(src.components_);

    // Do not touch TopologyLockable members.  (See TopologyLockable for why.)

    virtualGenus_ = src.virtualGenus_;
    alexander_ = std::move(src.alexander_);
    jones_ = std::move(src.jones_);
    homflyLM_ = std::move(src.homflyLM_);
    homflyAZ_ = std::move(src.homflyAZ_);
    bracket_ = std::move(src.bracket_);
    niceTreeDecomposition_ = std::move(src.niceTreeDecomposition_);

    // Let src dispose of the original crossings in its own destructor.
    return *this;
}

Link::Link(const std::string& description) {
    try {
        *this = fromSig(description);
        return;
    } catch (const InvalidArgument&) {
    }

    try {
        *this = fromOrientedGauss(description);
        return;
    } catch (const InvalidArgument&) {
    }

    try {
        *this = fromGauss(description);
        return;
    } catch (const InvalidArgument&) {
    }

    try {
        *this = fromDT(description);
        return;
    } catch (const InvalidArgument&) {
    }

    try {
        *this = fromPD(description);
        return;
    } catch (const InvalidArgument&) {
    }

    throw InvalidArgument("The given string could not be interpreted "
        "as representing a link");
}

bool Link::isConnected() const {
    if (components_.size() <= 1)
        return true;

    // Look for any zero-crossing components.
    for (auto c : components_)
        if (! c)
            return false; // since we already know there are other components

    // Every component contains at least one crossing.
    if (crossings_.size() <= 1)
        return true;

    // Run a depth-first search.
    // We know there are at least two crossings from the tests above.
    size_t n = crossings_.size();

    FixedArray<bool> visited(n, false);
    FixedArray<const Crossing*> stack(n);

    size_t stackSize = 1;
    stack[0] = crossings_.front();
    visited[0] = true;
    size_t nFound = 1;

    while (stackSize > 0) {
        auto curr = stack[--stackSize];

        for (int i = 0; i < 2; ++i) {
            // We only need to look at next, not prev, since anything we can
            // reach via prev can also be reached via a sequence of next steps.
            auto adj = curr->next_[i].crossing();
            if (! visited[adj->index()]) {
                ++nFound;
                if (nFound == n)
                    return true;

                visited[adj->index()] = true;
                stack[stackSize++] = adj;
            }
        }
    }

    return false;
}

std::vector<Link> Link::diagramComponents() const {
    if (components_.empty())
        return {};
    if (components_.size() == 1)
        return { Link(*this) };

    // We have multiple link components.
    // Work out how many of these are zero-crossing unknots.
    size_t nTrivial = 0;
    for (auto c : components_)
        if (! c)
            ++nTrivial;

    if (crossings_.empty()) {
        std::vector<Link> ans(nTrivial); // all empty links
        for (auto& link : ans)
            link.components_.emplace_back(); // make them 0-crossing unknots
        return ans;
    }

    // We have at least one crossing.
    // Run a depth-first search to work out which crossings belong to the same
    // components.

    size_t n = crossings_.size();

    FixedArray<ssize_t> comp(n, -1);
    FixedArray<const Crossing*> stack(n);

    size_t next = 0;
    size_t nComp = 0; // only incremented _after_ finishing the component
    size_t nFound = 0;

    while (nFound < n) {
        // Find a starting point to explore the next connected component.
        while (comp[next] >= 0)
            ++next;

        size_t stackSize = 1;
        stack[0] = crossings_[next];
        comp[next] = nComp;
        ++next;
        ++nFound;

        while (stackSize > 0) {
            auto curr = stack[--stackSize];

            for (int i = 0; i < 2; ++i) {
                // We only need to look at next, not prev, since anything we
                // can reach via prev can also be reached via a sequence of
                // next steps.
                auto adj = curr->next_[i].crossing();
                if (comp[adj->index()] < 0) {
                    stack[stackSize++] = adj;
                    comp[adj->index()] = nComp;
                    ++nFound;
                }
            }
        }

        ++nComp;
    }

    // Extract the components into individual links.
    //
    // Do this the easy way for now: clone this link so that we get copies of
    // all the crossings, hooked together and indexed correctly.
    //
    // Like moveContentsTo(), we abuse MarkedVector by having crossings
    // temporarily belong to two marked vectors at once; see moveContentsTo()
    // for why this is fine.

    Link clone(*this);
    std::vector<Link> ans(nComp + nTrivial);

    // We need to distribute link components first, while the new crossings
    // still have their old indices.
    for (auto c : clone.components_)
        if (c)
            ans[comp[c.crossing()->index()]].components_.push_back(c);
    clone.components_.clear();

    // Now distribute crossings, which will change their indices and make our
    // comp[] array useless.
    for (size_t i = 0; i < n; ++i)
        ans[comp[i]].crossings_.push_back(clone.crossings_[i]);
    clone.crossings_.clear();

    // Finally add in the trivial (0-crossing) diagram components.
    for (size_t i = 0; i < nTrivial; ++i)
        ans[nComp + i].components_.emplace_back();

    return ans;
}

bool Link::connected(const Crossing* a, const Crossing* b) const {
    if (components_.size() <= 1)
        return true;

    // Do a depth-first search.
    size_t n = crossings_.size();

    FixedArray<bool> visited(n, false);
    FixedArray<const Crossing*> stack(n);

    size_t stackSize = 1;
    stack[0] = a;
    visited[a->index()] = true;

    while (stackSize > 0 && ! visited[b->index()]) {
        auto curr = stack[--stackSize];

        for (int i = 0; i < 2; ++i) {
            // We only need to look at next, not prev, since anything we can
            // reach via prev can also be reached via a sequence of next steps.
            const Crossing* adj = curr->next_[i].crossing();
            if (! visited[adj->index()]) {
                visited[adj->index()] = true;
                stack[stackSize++] = adj;
            }
        }
    }

    return visited[b->index()];
}

StrandRef Link::overForComponent(StrandRef component) const {
    if (! component)
        return {};

    StrandRef start = component;
    if (start.strand() == 1)
        return start;

    // Our component begins with an under-crossing.

    if (components_.size() == 1) {
        // Our link is actually a _knot_, which means the corresponding
        // over-crossing is guaranteed to be part of the same component.
        start.jump();
        return start;
    }

    // We will need to follow the link component around.
    for (StrandRef s = start.next(); s != start; ++s)
        if (s.strand() == 1)
            return s;

    // The component is entirely under-crossings.
    return {};
}

StrandRef Link::underForComponent(StrandRef component) const {
    if (! component)
        return {};

    StrandRef start = component;
    if (start.strand() == 0)
        return start;

    // Our component begins with an over-crossing.

    if (components_.size() == 1) {
        // Our link is actually a _knot_, which means the corresponding
        // under-crossing is guaranteed to be part of the same component.
        start.jump();
        return start;
    }

    // We will need to follow the link component around.
    for (StrandRef s = start.next(); s != start; ++s)
        if (s.strand() == 0)
            return s;

    // The component is entirely over-crossings.
    return {};
}

size_t Link::countTrivialComponents() const {
    size_t ans = 0;
    for (StrandRef c : components_)
        if (! c)
            ++ans;
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

long Link::linking2() const {
    if (crossings_.empty())
        return 0;

    // This algorithm is linear time.

    // First sum the signs of all crossings.
    long ans = 0;
    for (const Crossing* c : crossings_)
        ans += c->sign();

    // Now work through each component, and subtract off crossings that
    // involve that component twice.
    FixedArray<bool> seen(crossings_.size(), false);

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

    return ans;
}

long Link::writheOfComponent(StrandRef strand) const {
    if (! strand)
        return 0;

    // This algorithm is linear time.

    long ans = 0;

    bool* seen = new bool[crossings_.size()];
    std::fill(seen, seen + crossings_.size(), false);

    StrandRef s = strand;
    do {
        if (seen[s.crossing()->index()])
            ans += s.crossing()->sign();
        else
            seen[s.crossing()->index()] = true;
        ++s;
    } while (s != strand);

    delete[] seen;
    return ans;
}

long Link::oddWrithe() const {
    if (components_.size() != 1)
        throw FailedPrecondition("Self-linking number requires the link "
            "to have exactly one component");

    if (crossings_.empty())
        return 0;

    FixedArray<ssize_t> firstSeen(crossings_.size(), -1);
    long ans = 0;

    StrandRef start = components_.front();
    StrandRef s = start;
    ssize_t pos = 0;
    do {
        size_t i = s.crossing()->index();
        if (firstSeen[i] < 0)
            firstSeen[i] = pos;
        else if (! ((pos ^ firstSeen[i]) & 1))
            ans += s.crossing()->sign();

        ++pos;
        ++s;
    } while (s != start);

    return ans;
}

size_t Link::seifertCircles() const {
    if (crossings_.empty())
        return components_.size();

    size_t ans = 0;
    FixedArray<bool> seen(crossings_.size() * 2, false);
    for (size_t i = 0; i < crossings_.size() * 2; ++i)
        if (! seen[i]) {
            // Follow the Seifert circle starting at the strand with this ID.
            ++ans;
            StrandRef start = strand(i);
            StrandRef s = start;
            do {
                seen[s.id()] = true;
                s.jump();
                ++s;
            } while (s != start);
        }

    // Finish by adding in any zero-crossing components.
    for (auto c : components_)
        if (! c)
            ++ans;
    return ans;
}

void Link::writeTextShort(std::ostream& out) const {
    if (components_.empty()) {
        out << "Empty link";
        return;
    }

    if (components_.size() == 1)
        out << crossings_.size() << "-crossing knot: ";
    else
        out << crossings_.size() << "-crossing, "
            << components_.size() << "-component link: ";

    brief(out);
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

void Link::swap(Link& other) {
    if (&other == this)
        return;

    // We use a basic PacketChangeSpan here, not a richer ChangeAndClearSpan,
    // since we do not want to touch computed properties.  Our intention here
    // is to swap them, not clear them.
    PacketChangeSpan span1(*this);
    PacketChangeSpan span2(other);

    // Swap core data:
    crossings_.swap(other.crossings_);
    components_.swap(other.components_);

    // Swap properties:
    std::swap(virtualGenus_, other.virtualGenus_);
    alexander_.swap(other.alexander_);
    jones_.swap(other.jones_);
    homflyAZ_.swap(other.homflyAZ_);
    homflyLM_.swap(other.homflyLM_);
    bracket_.swap(other.bracket_);
    niceTreeDecomposition_.swap(other.niceTreeDecomposition_);
}

void Link::insertLink(const Link& source) {
    if (source.isEmpty())
        return;
    if (isEmpty()) {
        *this = source;
        return;
    }

    ChangeAndClearSpan<> span(*this);

    // From here we can assume source is non-empty.
    // Clone its crossings, and transfer them directly into this link.
    // This abuses the MarkedVector API slightly (since an object must
    // not belong to more than one MarkedVector at a time), but the
    // implementation of MarkedVector does make it correct.
    Link clone(source);
    for (Crossing* c : clone.crossings_)
        crossings_.push_back(c);
    clone.crossings_.clear();

    // We can copy or move components from clone, whichever makes more sense.
    if (isEmpty()) {
        // Constant time, and correct since this link is empty and we do not
        // care what happens to clone.components_.
        clone.components_.swap(components_);
    } else {
        components_.insert(components_.end(),
            clone.components_.begin(), clone.components_.end());
    }
}

void Link::insertLink(Link&& source) {
    if (source.isEmpty())
        return;
    if (isEmpty()) {
        *this = std::move(source);
        return;
    }

    ChangeAndClearSpan<> span(*this);
    // Don't worry about change spans for source, since source is about to be
    // destroyed.

    // The following code abuses the MarkedVector API slightly, but it's fine;
    // see the comments in moveContentsTo() below.
    for (auto* c : source.crossings_)
        crossings_.push_back(c);
    source.crossings_.clear();

    components_.insert(components_.end(),
        source.components_.begin(), source.components_.end());
    // It should be harmless to leave junk in source.components_, but let's
    // not risk someone abusing what might become dangling crossing pointers.
    source.components_.clear();
}

void Link::moveContentsTo(Link& dest) {
    if (isEmpty())
        return;
    if (dest.isEmpty()) {
        swap(dest);
        return;
    }

    ChangeAndClearSpan<> span(*this);
    ChangeAndClearSpan<> span2(dest);

    // The following code abuses MarkedVector, since for a brief moment each
    // crossing belongs to both crossings_ and dest.crossings_.  However, the
    // subsequent clear() operation does not touch the markings (indices), and
    // so we end up with the correct result (i.e., markings correct for dest).
    for (auto* c : crossings_)
        dest.crossings_.push_back(c);
    crossings_.clear();

    dest.components_.insert(dest.components_.end(),
        components_.begin(), components_.end());
    components_.clear();
}

GroupPresentation Link::group(bool simplify) const {
    if (crossings_.empty()) {
        // This is a zero-crossing unlink.
        return GroupPresentation(components_.size());
    }

    // We have a non-zero number of crossings.
    // Build the Wirtinger presentation.
    //
    // We start with just the generators corresponding to sections of
    // the diagram that include crossings; we will pick up any additional
    // generators for zero-crossing unknot components when we traverse the
    // link shortly.
    GroupPresentation g(crossings_.size());

    // We will need to number the "over-segments" - coniguous sections
    // of the knot that consist entirely of over-crossings.
    // Construct a map from arc IDs to "over-segment" IDs, by traversing
    // each component one at a time.
    int* strandToSection = new int[2 * crossings_.size()];
    int currSegment = 0;

    for (StrandRef comp : components_) {
        if (! comp) {
            // This is a zero-crossing unknot component.
            g.addGenerator();
            continue;
        }

        // Start our traversal of each component from an under-crossing,
        // so we are guaranteed that this is the beginning of an over-segment.
        StrandRef start = comp;
        if (start.strand() > 0) {
            if (components_.size() == 1) {
                // Just jump immediately to the under-strand at this crossing.
                start.jump();
            } else {
                // There is no guarantee that the under-strand is part
                // of the same component.  Instead, walk along the component
                // until we find an under-strand.
                StrandRef s = start;
                do {
                    ++s;
                } while (s.strand() > 0 && s != start);

                start = s;

                // It is possible that we never found an under-strand.
                // This happens when the entire component is an unknot
                // with no self-crossings that is overlaid onto the diagram.
                //
                // How this affects us now is that the total number of
                // "over-segments" (i.e., the number of generators in our
                // group presentation) goes up by one.
                //
                // We will adjust this later.
            }
        }

        StrandRef s = start;
        do {
            strandToSection[s.id()] = currSegment;
            ++s;
            if (s.strand() == 0) {
                // We just passed under a crossing.
                ++currSegment;
            }
        } while (s != start);

        if (start.strand() > 0) {
            // This is the scenario noted above where some component
            // consists entirely of over-crossings.
            // We need to make two adjustments:
            //
            // - increment currSegment, since we are about to move to a new
            //   component but we did not increment it at the end of the
            //   loop just now; and
            //
            // - increment the total number of group generators, since we
            //   based our original count on the number of crossings, which
            //   only counts those over-segments with start and end points.

            ++currSegment;
            g.addGenerator();
        }
    }

    // Now build the presentation.
    for (Crossing* c : crossings_) {
        GroupExpression exp;
        if (c->sign() > 0) {
            exp.addTermLast(strandToSection[c->upper().id()], 1);
            exp.addTermLast(strandToSection[c->lower().id()], 1);
            exp.addTermLast(strandToSection[c->upper().id()], -1);
            exp.addTermLast(strandToSection[c->lower().prev().id()], -1);
        } else {
            exp.addTermLast(strandToSection[c->upper().id()], 1);
            exp.addTermLast(strandToSection[c->lower().prev().id()], 1);
            exp.addTermLast(strandToSection[c->upper().id()], -1);
            exp.addTermLast(strandToSection[c->lower().id()], -1);
        }
        g.addRelation(std::move(exp));
    }

    delete[] strandToSection;

    if (simplify)
        g.simplify();
    return g;
}

std::string Link::brief() const {
    if (components_.empty())
        return std::string();

    std::ostringstream out;
    brief(out);
    return out.str();
}

void Link::brief(std::ostream& out) const {
    if (components_.empty())
        return;

    if (crossings_.empty()) {
        out << "( )";
        for (size_t i = 1; i < components_.size(); ++i)
            out << " ( )";
        return;
    }

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
}

void Link::composeWith(const Link& other) {
    if (other.isEmpty())
        return;

    ChangeAndClearSpan<> span(*this);

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
}

namespace {
    // Data to support whiteheadDouble().
    // This array identifies the crossing numbers in the double's 2x2 grid
    // that correspond to various incoming/outgoing strands in the original
    // crossing.
    //
    // The four arguments:
    // - sign of the original crossing (negative, positive)
    // - original strand (lower, upper)
    // - direction of the new strand (forward, reverse)
    // - end of the new strand (in, out)
    //
    // This is most easily accessed via dblOffset().

    constexpr int dblGrid[2][2][2][2] = {
        {
            { // original: negative crossing, lower strand
                { 0, 2 /* forward */ }, { 3, 1 /* reverse */ }
            },
            { // original: negative crossing, upper strand
                { 1, 0 /* forward */ }, { 2, 3 /* reverse */ }
            }
        },
        {
            { // original: positive crossing, lower strand
                { 0, 2 /* forward */ }, { 3, 1 /* reverse */ }
            },
            { // original: positive crossing, upper strand
                { 2, 3 /* forward */ }, { 1, 0 /* reverse */ }
            }
        }
    };

    int dblOffset(StrandRef s, int dir, int end) {
        return dblGrid[s.crossing()->sign() < 0 ? 0 : 1][s.strand()][dir][end];
    }
}

Link Link::whiteheadDouble(bool positive) const {
    if (components_.size() != 1)
        throw FailedPrecondition("The Whitehead double requires the link "
            "to have exactly one component");
    if (size() == 0) {
        if (positive)
            return fromData({ 1, 1 }, { 1, -2, 2, -1 });
        else
            return fromData({ -1, -1 }, { -1, 2, -2, 1 });
    }

    Link ans;

    // Crossing i of the original knot:
    //
    // +ve:    |                 -ve:    ^
    //     --- | --->                --- | --->
    //         v                         |
    //
    // Crossings (4i, ..., 4i+3) of the Whitehead double:
    //
    //  4i   ^     | 4i+2
    //   --- | --- | -->
    //       |     |
    //   <-- | --- | ---
    //  4i+1 |     v 4i+3
    //
    // The signs of these crossings:
    //
    //       - +
    //       + -

    // Create the four crossings for each original, and join them
    // together internally.
    Crossing* tmp[4]; // Used to build grids of crossings
    for (Crossing* c : crossings_) {
        ans.crossings_.push_back(tmp[0] = new Crossing(-1));
        ans.crossings_.push_back(tmp[1] = new Crossing(1));
        ans.crossings_.push_back(tmp[2] = new Crossing(1));
        ans.crossings_.push_back(tmp[3] = new Crossing(-1));

        Link::join(tmp[1]->upper(), tmp[0]->upper());
        Link::join(tmp[2]->upper(), tmp[3]->upper());
        Link::join(tmp[0]->lower(), tmp[2]->lower());
        Link::join(tmp[3]->lower(), tmp[1]->lower());
    }

    // Connect the different grids together, building two parallel copies
    // (one oriented in reverse).  We will later break these open to add the
    // twists (if necessary) and clasp.
    long writhe = 0;
    for (Crossing* c : crossings_) {
        size_t base = 4 * c->index();

        for (int s = 0; s < 2; ++s) {
            StrandRef next = c->next(s);
            size_t nextBase = 4 * next.crossing()->index();

            Link::join(
                ans.crossings_[base + dblOffset(c->strand(s), 0, 1)]->
                    strand(s),
                ans.crossings_[nextBase + dblOffset(next, 0, 0)]->
                    strand(next.strand()));
            Link::join(
                ans.crossings_[nextBase + dblOffset(next, 1, 1)]->
                    strand(next.strand()),
                ans.crossings_[base + dblOffset(c->strand(s), 1, 0)]->
                    strand(s));
        }

        writhe += c->sign();
    }

    // Identify the endpoints where we need to break the link open and insert
    // the clasp.  The indices:
    // - start vs end of the traversal of the original knot;
    // - forward vs reverse direction.
    StrandRef start = components_.front();
    size_t startBase = 4 * start.crossing()->index();

    StrandRef breakpoint[2][2];
    breakpoint[0][0] = ans.crossing(startBase + dblOffset(start, 0, 0))->
        strand(start.strand());
    breakpoint[0][1] = ans.crossing(startBase + dblOffset(start, 1, 1))->
        strand(start.strand());
    breakpoint[1][0] = breakpoint[0][0].prev();
    breakpoint[1][1] = breakpoint[0][1].next();

    // Add twists to compensate for the writhe.
    if (writhe > 0) {
        for ( ; writhe; --writhe) {
            Crossing* twist[2];
            ans.crossings_.push_back(twist[0] = new Crossing(1));
            ans.crossings_.push_back(twist[1] = new Crossing(1));

            Link::join(breakpoint[1][0], twist[0]->lower());
            Link::join(twist[0]->lower(), twist[1]->upper());
            breakpoint[1][0] = twist[1]->upper();

            Link::join(twist[1]->lower(), twist[0]->upper());
            Link::join(twist[0]->upper(), breakpoint[1][1]);
            breakpoint[1][1] = twist[1]->lower();
        }
    } else if (writhe < 0) {
        for ( ; writhe; ++writhe) {
            Crossing* twist[2];
            ans.crossings_.push_back(twist[0] = new Crossing(-1));
            ans.crossings_.push_back(twist[1] = new Crossing(-1));

            Link::join(breakpoint[1][0], twist[0]->upper());
            Link::join(twist[0]->upper(), twist[1]->lower());
            breakpoint[1][0] = twist[1]->lower();

            Link::join(twist[1]->upper(), twist[0]->lower());
            Link::join(twist[0]->lower(), breakpoint[1][1]);
            breakpoint[1][1] = twist[1]->upper();
        }
    }

    // Add the clasp.
    Crossing* clasp[2];
    if (positive) {
        ans.crossings_.push_back(clasp[0] = new Crossing(1));
        ans.crossings_.push_back(clasp[1] = new Crossing(1));

        Link::join(breakpoint[1][0], clasp[0]->upper());
        Link::join(clasp[0]->upper(), clasp[1]->lower());
        Link::join(clasp[1]->lower(), breakpoint[1][1]);

        Link::join(breakpoint[0][1], clasp[1]->upper());
        Link::join(clasp[1]->upper(), clasp[0]->lower());
        Link::join(clasp[0]->lower(), breakpoint[0][0]);
    } else {
        ans.crossings_.push_back(clasp[0] = new Crossing(-1));
        ans.crossings_.push_back(clasp[1] = new Crossing(-1));

        Link::join(breakpoint[1][0], clasp[0]->lower());
        Link::join(clasp[0]->lower(), clasp[1]->upper());
        Link::join(clasp[1]->upper(), breakpoint[1][1]);

        Link::join(breakpoint[0][1], clasp[1]->lower());
        Link::join(clasp[1]->lower(), clasp[0]->upper());
        Link::join(clasp[0]->upper(), breakpoint[0][0]);
    }

    // And we're done.
    ans.components_.push_back(breakpoint[0][0]);
    return ans;
}

Link Link::parallel(int k, Framing framing) const {
    // Get the special cases out of the way.
    if (k == 0 || components_.empty())
        return Link();
    if (k == 1)
        return Link(*this);
    if (crossings_.empty())
        return Link(components_.size() * k);

    Link ans;
    auto* tmp = new Crossing*[k*k]; // Used to build grids of crossings

    // Crossing i of the original link:
    //
    // +ve:    |                 -ve:    ^
    //     --- | --->                --- | --->
    //         v                         |
    //
    // Crossings (k^2 i, ..., k^2 (i+1) - 1) of the new link:
    //
    //  k^2 i       | ... | k^2 (i+1) - k     k^2 i + k-1 ^ ... ^ k^2 (i+1) - 1
    //          --- | --- | --->                      --- | --- | --->
    //          ... | ... | ...                       ... | ... | ...
    //          --- | --- | --->                      --- | --- | --->
    //  k^2 i + k-1 v ... v k^2 (i+1) - 1     k^2 i       | ... | k^2 (i+1) - k

    // Create the k^2 crossings for each original, and join them
    // together internally.
    for (Crossing* c : crossings_) {
        for (int i = 0; i < k * k; ++i)
            ans.crossings_.push_back(tmp[i] = new Crossing(c->sign()));

        for (int i = 0; i < k; ++i)
            for (int j = 0; j < k - 1; ++j)
                Link::join(tmp[k*i + j]->upper(), tmp[k*i + j+1]->upper());
        for (int i = 0; i < k - 1; ++i)
            for (int j = 0; j < k; ++j)
                Link::join(tmp[k*i + j]->lower(), tmp[k*(i+1) + j]->lower());
    }

    // Walk around the original knot, and keep track of the left-hand
    // and right-hand crossings of the new tangle where we (i) enter the
    // grid configuration, and (ii) leave this configuration.

    StrandRef s;
    ssize_t enterL, exitL;
    int enterStrand, exitStrand;
    std::make_signed_t<size_t> enterDelta, exitDelta, startDelta;
    ssize_t startL;
    int startStrand;

    long writhe;
    bool* seen = new bool[crossings_.size()];
    std::fill(seen, seen + crossings_.size(), false);

    for (const StrandRef& start : components_) {
        if (! start) {
            // This component is a 0-crossing unknot.
            for (int i = 0; i < k; ++i)
                ans.components_.emplace_back();
            continue;
        }

        writhe = 0;
        s = start;
        exitL = -1;
        do {
            ssize_t idx = s.crossing()->index();
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
                for (int i = 0; i < k; ++i)
                    Link::join(
                        ans.crossings_[exitL + i * exitDelta]->
                            strand(exitStrand),
                        ans.crossings_[enterL + i * enterDelta]->
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

        if (writhe == 0 || framing == Framing::Blackboard) {
            // Close up the k new parallel link components.
            for (int i = 0; i < k; ++i)
                Link::join(
                    ans.crossings_[exitL + i * exitDelta]->
                        strand(exitStrand),
                    ans.crossings_[startL + i * startDelta]->
                        strand(startStrand));
        } else if (writhe > 0) {
            // We want the Seifert framing, and the writhe is positive.
            // Insert the requisite number of negative twists
            // before closing off the k parallel link components.
            for (long w = 0; w < writhe * k; ++w) {
                for (int j = 0; j < k - 1; ++j)
                    ans.crossings_.push_back(tmp[j] = new Crossing(-1));

                for (int j = 0; j < k - 2; ++j)
                    Link::join(tmp[j]->lower(), tmp[j + 1]->lower());

                if (w == 0) {
                    Link::join(
                        ans.crossings_[exitL]->strand(exitStrand),
                        tmp[0]->lower());
                    for (int j = 1; j < k; ++j)
                        Link::join(
                            ans.crossings_[exitL + j * exitDelta]->
                                strand(exitStrand),
                            tmp[j - 1]->upper());
                } else {
                    Link::join(
                        ans.crossings_[exitL]->upper(), tmp[0]->lower());
                    for (int j = 1; j < k - 1; ++j)
                        Link::join(
                            ans.crossings_[exitL + j]->upper(),
                            tmp[j - 1]->upper());
                    Link::join(
                        ans.crossings_[exitL + (k - 2)]->lower(),
                        tmp[k - 2]->upper());
                }

                exitL = tmp[0]->index();
            }

            for (int j = 0; j < k - 1; ++j)
                Link::join(
                    ans.crossings_[exitL + j]->upper(),
                    ans.crossings_[startL + j * startDelta]->
                        strand(startStrand));
            Link::join(
                ans.crossings_[exitL + (k - 2)]->lower(),
                ans.crossings_[startL + (k - 1) * startDelta]->
                    strand(startStrand));
        } else {
            // We want the Seifert framing, and the writhe is negative.
            // Insert the requisite number of positive twists
            // before closing off the k parallel link components.
            for (long w = 0; w < (-writhe) * k; ++w) {
                for (int j = 0; j < k - 1; ++j)
                    ans.crossings_.push_back(tmp[j] = new Crossing(1));

                for (int j = 0; j < k - 2; ++j)
                    Link::join(tmp[j]->upper(), tmp[j + 1]->upper());

                if (w == 0) {
                    Link::join(
                        ans.crossings_[exitL]->strand(exitStrand),
                        tmp[0]->upper());
                    for (int j = 1; j < k; ++j)
                        Link::join(
                            ans.crossings_[exitL + j * exitDelta]->
                                strand(exitStrand),
                            tmp[j - 1]->lower());
                } else {
                    Link::join(
                        ans.crossings_[exitL]->lower(), tmp[0]->upper());
                    for (int j = 1; j < k - 1; ++j)
                        Link::join(
                            ans.crossings_[exitL + j]->lower(),
                            tmp[j - 1]->lower());
                    Link::join(
                        ans.crossings_[exitL + (k - 2)]->upper(),
                        tmp[k - 2]->lower());
                }

                exitL = tmp[0]->index();
            }

            for (int j = 0; j < k - 1; ++j)
                Link::join(
                    ans.crossings_[exitL + j]->lower(),
                    ans.crossings_[startL + j * startDelta]->
                        strand(startStrand));
            Link::join(
                ans.crossings_[exitL + (k - 2)]->upper(),
                ans.crossings_[startL + (k - 1) * startDelta]->
                    strand(startStrand));
        }

        // Take note of the k new link components.
        for (int i = 0; i < k; ++i)
            ans.components_.push_back(
                ans.crossings_[startL + i * startDelta]->
                    strand(startStrand));
    }

    delete[] seen;
    delete[] tmp;
    return ans;
}

void Link::tightEncode(std::ostream& out) const {
    regina::detail::tightEncodeIndex(out, size());

    // Bit-pack the crossing signs, 6 per character.
    {
        char bits = 0;
        int pos = 0;
        for (auto* c : crossings_) {
            if (c->sign() > 0)
                bits |= (1 << pos);
            if (pos < 5)
                ++pos;
            else {
                out.put(bits + 33);
                bits = 0;
                pos = 0;
            }
        }
        if (pos != 0)
            out.put(bits + 33);
    }

    for (auto* c : crossings_)
        for (int side = 0; side < 2; ++side)
            regina::detail::tightEncodeIndex(out, c->next(side).id());

    regina::detail::tightEncodeIndex(out, countComponents());

    for (StrandRef s : components_)
        regina::detail::tightEncodeIndex(out, s.id());
}

Link Link::tightDecode(std::istream& input) {
    // Read size as a signed type, since this will make comparisons
    // with (signed) strand IDs simpler.
    auto size = regina::detail::tightDecodeIndex<ssize_t>(input);
    if (size < 0)
        throw InvalidInput("The tight encoding has a negative number "
            "of crossings");

    Link ans;
    int pos = 0;
    int bits;
    for (ssize_t i = 0; i < size; ++i) {
        if (pos == 0) {
            bits = input.get();
            if (bits == EOF)
                throw InvalidInput("The tight encoding is incomplete");
            bits -= 33;
        }
        ans.crossings_.push_back(new Crossing((bits & (1 << pos)) ? 1 : -1));
        if (pos < 5)
            ++pos;
        else
            pos = 0;
    }

    for (auto* c : ans.crossings_)
        for (int side = 0; side < 2; ++side) {
            auto id = regina::detail::tightDecodeIndex<ssize_t>(input);
            if (id < 0 || id >= 2 * size)
                throw InvalidInput("The tight encoding has invalid "
                    "connections");

            StrandRef dest = ans.strand(id);
            if (dest.prev())
                throw InvalidInput("The tight encoding has inconsistent "
                    "connections");
            join(StrandRef(c, side), dest);
        }

    auto cmpts = regina::detail::tightDecodeIndex<size_t>(input);
    if (cmpts < 0)
        throw InvalidInput("The tight encoding has a negative number "
            "of components");
    if (size > 0 && cmpts == 0)
        throw InvalidInput("The tight encoding has an invalid number "
            "of components");

    for (size_t i = 0; i < cmpts; ++i) {
        auto destID = regina::detail::tightDecodeIndex<ssize_t>(input);
        if (destID >= 2 * size)
            throw InvalidInput("The tight encoding contains an invalid "
                "component");
        ans.components_.push_back(ans.strand(destID));
    }

    return ans;
}

std::string Link::source(Language language) const {
    std::ostringstream out;

    char left, right;
    switch (language) {
        case Language::Cxx:
            out << "Link link = Link::fromData(";
            left = '{'; right = '}';
            break;
        case Language::Python:
            out << "link = Link.fromData(";
            left = '['; right = ']';
            break;
    }

    out << left << ' ';
    if (crossings_.empty())
        out << right;
    else {
        auto it = crossings_.begin();
        out << ((*it)->sign() > 0 ? "+1" : "-1");

        for (++it; it != crossings_.end(); ++it)
            out << ", " << ((*it)->sign() > 0 ? "+1" : "-1");
        out << ' ' << right;
    }

    if (components_.empty()) {
        if (language == Language::Python)
            out << ", [ ]";
    } else {
        // In Python, we express multiple components as [[...], ..., [...]].
        // However, for a _single_ component we can just use [...].
        // A problem: this makes [] ambiguous, since it could represent either
        // the empty link or the zero-crossing unknot, and so in the latter
        // case we must still keep the outer list.
        bool outerList = (language == Language::Python &&
            (components_.size() > 1 || crossings_.empty()));

        out << ", ";
        if (outerList)
            out << left;

        StrandRef s;
        bool first = true;
        for (StrandRef start : components_) {
            if (first)
                first = false;
            else
                out << ", ";
            out << left << ' ';

            if (! start) {
                if (language == Language::Cxx)
                    out << "0 ";
                out << right;
            } else {
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
                out << ' ' << right;
            }
        }

        if (outerList)
            out << right;
    }

    out << ')';
    if (language == Language::Cxx)
        out << ';';
    out << '\n';

    return out.str();
}

std::string Link::pace() const {
    std::ostringstream out;
    writePACE(out);
    return out.str();
}

void Link::writePACE(std::ostream& out) const {
    // if (! label().empty())
    //     out << "c " << label() << std::endl;
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
        ChangeAndClearSpan<> span(*this);
        if (p == 0) {
            // Insert a single unknot.
            components_.emplace_back();
        } else {
            // Insert p disjoint unknots.
            for (int i = 0; i < p; ++i)
                components_.emplace_back();
        }
        return;
    }
    if (q == 1) {
        // Insert a single unknot.
        ChangeAndClearSpan<> span(*this);
        components_.emplace_back();
        return;
    }

    // We now have 1 < q <= p.
    // Use the standard diagram with p(q-1) crossings.

    int n = p * (q - 1);
    int nComp = std::gcd(p, q);

    ChangeAndClearSpan<> span(*this);

    auto* c = new Crossing*[n];
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
            components_.emplace_back(c[sliceIdx], 1);
    }

    delete[] c;
}

} // namespace regina

