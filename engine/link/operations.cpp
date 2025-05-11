
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
#include <algorithm>

namespace regina {

bool Link::makeAlternating() {
    if (crossings_.empty())
        return true;

    // Run a breadth-first search through each connected piece of the diagram.
    // Here status[i] takes one of the following values for each crossing i:
    //   * 0 means crossing not yet visited;
    //   * 1 means crossing will be preserved;
    //   * -1 means crossing will be changed.

    size_t n = crossings_.size();
    FixedArray<int> status(n, 0);
    bool needsChange = false;

    FixedArray<size_t> queue(n);
    size_t queueStart = 0, queueEnd = 0;

    for (size_t i = 0; i < n; ++i) {
        // Find a starting point for the next connected component of this
        // link diagram.
        if (status[i])
            continue;

        // This crossing will be preserved, and will act as a starting point
        // for the next breadth-first search.
        queue[queueEnd++] = i;
        status[i] = 1;

        while (queueStart < queueEnd) {
            size_t srcIndex = queue[queueStart++];
            Crossing* from = crossings_[srcIndex];

            // The search only needs to consider forward arrows, since this is
            // enough to reach the entire connected piece of the diagram.
            for (int j = 0; j < 2; ++j) {
                StrandRef next = from->next_[j];
                size_t nextIndex = next.crossing_->index();

                if (status[nextIndex]) {
                    // We have already chosen what to do with next.crossing_.
                    bool sameAction = (status[nextIndex] == status[srcIndex]);
                    if ((next.strand_ == j && sameAction) ||
                            (next.strand_ != j && ! sameAction)) {
                        // The diagram cannot be made alternating.
                        return false;
                    }
                } else {
                    // Here is where we decide what to do with next.crossing_.
                    if (next.strand_ == j) {
                        status[nextIndex] = -status[srcIndex];
                        // The first time we see a crossing that needs
                        // changing, we will hit this point in the code.
                        // (Possibly we hit this point again many times
                        // after that also, but this is not relevant.)
                        needsChange = true;
                    } else
                        status[nextIndex] = status[srcIndex];

                    // Propagate our search through next.crossing_.
                    queue[queueEnd++] = nextIndex;
                }
            }
        }
    }

    // The diagram can be made alternating, and we know how to do it.
    if (! needsChange)
        return true;

    // There will be changes: go ahead and make them.
    ChangeAndClearSpan<> span(*this);

    for (size_t i = 0; i < n; ++i)
        if (status[i] < 0)
            change(crossings_[i]);

    return true;
}

bool Link::selfFrame() {
    // Some notes:
    //
    // We arbitrarily decide to put all twists on the left.
    //
    // Note: the r1 moves we use are always legal.
    //
    // We are safe to iterate through components_ while we add our twists,
    // since r1 does not change the components_ array and does not invalidate
    // existing strand references.

    bool changed = false;
    for (StrandRef c : components_) {
        long w = writheOfComponent(c);
        if (w > 0) {
            changed = true;
            do {
                r1(c, 0 /* left */, -1);
                --w;
            } while (w != 0);
        } else if (w < 0) {
            changed = true;
            do {
                r1(c, 0 /* left */, 1);
                ++w;
            } while (w != 0);
        }
    }
    return changed;
}

void Link::reflect() {
    // Properties that are preserved under this operation:
    ssize_t tmpGenus = virtualGenus_;

    ChangeAndClearSpan<> span(*this);
    for (Crossing* cross : crossings_)
        cross->sign_ = -cross->sign_;

    // Restore properties that did not change:
    virtualGenus_ = tmpGenus;
}

void Link::reverse() {
    // Properties that are preserved under this operation:
    ssize_t tmpGenus = virtualGenus_;

    ChangeAndClearSpan<> span(*this);
    for (Crossing* cross : crossings_) {
        std::swap(cross->next_[0], cross->prev_[0]);
        std::swap(cross->next_[1], cross->prev_[1]);
    }

    // Restore properties that did not change:
    virtualGenus_ = tmpGenus;
}

void Link::reverse(StrandRef component) {
    if (! component)
        return;

    // Properties that are preserved under this operation:
    ssize_t tmpGenus = virtualGenus_;

    ChangeAndClearSpan<> span(*this);

    StrandRef s = component;
    do {
        auto cross = s.crossing();
        auto strand = s.strand();
        std::swap(cross->next_[strand], cross->prev_[strand]);
        cross->sign_ = -cross->sign_;
        --s; // because we just reversed s
    } while (s != component);

    // Restore properties that did not change:
    virtualGenus_ = tmpGenus;
}

void Link::rotate() {
    // Properties that are preserved under this operation:
    ssize_t tmpGenus = virtualGenus_;

    ChangeAndClearSpan<ChangeType::PreserveTopology> span(*this);

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

    // Restore properties that did not change:
    virtualGenus_ = tmpGenus;
}

void Link::change(Crossing* c) {
    // Properties that are preserved under this operation:
    ssize_t tmpGenus = virtualGenus_;

    ChangeAndClearSpan<> span(*this);

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

    // Restore properties that did not change:
    virtualGenus_ = tmpGenus;
}

void Link::changeAll() {
    // Properties that are preserved under this operation:
    ssize_t tmpGenus = virtualGenus_;

    ChangeAndClearSpan<> span(*this);

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

    // Restore properties that did not change:
    virtualGenus_ = tmpGenus;
}

void Link::resolve(Crossing* c) {
    ChangeAndClearSpan<> span(*this);

    // Note: we remove and destroy c at the end of this list of cases.

    if (c->next_[0].crossing() == c) {
        if (c->next_[1].crossing() == c) {
            if (c->next_[0].strand() == 1) {
                // This is a 1-crossing unknot component, and it resolves
                // into two 0-crossing unknot components.
                for (StrandRef& s : components_)
                    if (s.crossing_ == c) {
                        // 0-crossing component #1:
                        s.crossing_ = nullptr;
                        s.strand_ = 0;
                        break;
                    }
                // 0-crossing component #2:
                components_.emplace_back();
            } else {
                // This is a 1-crossing 2-component virtual link, and it
                // resolves into a single 0-crossing unknot component.

                // Find the first component at c and make it a 0-crossing
                // unknot.
                auto it = components_.begin();
                while (it->crossing_ != c)
                    ++it;
                it->crossing_ = nullptr;
                it->strand_ = 0;

                // Continue on to find the other component at c and remove it
                // entirely.
                ++it;
                while (it->crossing_ != c)
                    ++it;
                components_.erase(it);
            }
        } else {
            if (c->next_[0].strand() == 1) {
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

                components_.emplace_back();
            } else {
                // This is a virtual link, with a 1-crossing component that
                // runs from c->lower() back to itself.  This short component
                // will be lost when we resolve the crossing (it merges into
                // the other longer component that runs through c->upper()).
                StrandRef from = c->prev_[1];
                StrandRef to = c->next_[1];
                from.crossing()->next_[from.strand()] = to;
                to.crossing()->prev_[to.strand()] = from;

                // Fix the components.
                auto drop = components_.end();
                for (auto it = components_.begin(); it != components_.end();
                        ++it)
                    if (it->crossing_ == c) {
                        if (it->strand_ == 0) {
                            // This component will be removed entirely.
                            drop = it;
                        } else {
                            // This component needs a new starting point.
                            *it = to;
                        }
                    }
                // We should have found drop, since that component only has
                // one possible starting point (i.e., c->lower()).
                components_.erase(drop);
            }
        }
    } else if (c->next_[1].crossing() == c) {
        if (c->next_[1].strand() == 0) {
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

            components_.emplace_back();
        } else {
            // This is again a virtual link, with a 1-crossing component that
            // runs from c->upper() back to itself.  This short component
            // will be lost when we resolve the crossing (it merges into
            // the other longer component that runs through c->lower()).
            StrandRef from = c->prev_[0];
            StrandRef to = c->next_[0];
            from.crossing()->next_[from.strand()] = to;
            to.crossing()->prev_[to.strand()] = from;

            // Fix the components.
            auto drop = components_.end();
            for (auto it = components_.begin(); it != components_.end();
                    ++it)
                if (it->crossing_ == c) {
                    if (it->strand_ == 1) {
                        // This component will be removed entirely.
                        drop = it;
                    } else {
                        // This component needs a new starting point.
                        *it = to;
                    }
                }
            // We should have found drop, since that component only has
            // one possible starting point (i.e., c->upper()).
            components_.erase(drop);
        }
    } else {
        // This crossing does not connect to itself at all.

        // Ensure that no component uses c as its starting point.
        // Note that this could potentially happen twice.
        for (StrandRef& s : components_)
            if (s.crossing_ == c)
                ++s;

        // See whether c belongs to one or two components.
        auto comp = components_.end();
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
    }

    // In all cases, we finish by destroying the original crossing.
    crossings_.erase(crossings_.begin() + c->index());
    delete c;
}

void Link::makeVirtual(Crossing* crossing) {
    if (! crossing)
        return;

    ChangeAndClearSpan<> span(*this);

    StrandRef upper = crossing->upper();
    StrandRef lower = crossing->lower();

    // Plan how we will adjust any components that begin at the given
    // crossing.
    StrandRef upperBecomes, lowerBecomes;

    // If upper.next() == upper, then the upper strand will become a
    // zero-crossing unknot.
    if (upper.next() != upper) {
        upperBecomes = upper.next(); // Note: this _could_ be equal to lower.
        Link::join(upper.prev(), upper.next());
    }

    if (lower.next() == lower) {
        // lowerBecomes is already (correctly) a null reference, but we might
        // need to adjust upperBecomes also in case the crossing had
        // originally formed a 1-crossing unknot.
        if (upperBecomes == lower)
            upperBecomes = StrandRef();
    } else {
        lowerBecomes = lower.next(); // This will _not_ be equal to upper.
        Link::join(lower.prev(), lower.next());
    }

    // Update any components that started at the original crossing.
    int found = 0;
    for (StrandRef& c : components_)
        if (c.crossing() == crossing) {
            if (c.strand() == 0)
                c = lowerBecomes;
            else
                c = upperBecomes;
            if (++found == 2)
                break;
        }

    // Finally, delete the original crossing.
    crossings_.erase(crossings_.begin() + crossing->index());
    delete crossing;
}

void Link::graft(StrandRef first, StrandRef second) {
    ChangeAndClearSpan<> span(*this);

    if (first && ! second)
        std::swap(first, second);

    if (! first) {
        // Find the first zero-crossing component.
        auto trivial = componentFor(first);
        if (trivial == components_.end())
            throw InvalidArgument("graft(): a null reference was given "
                "but this link has no zero-crossing components");

        if (! second) {
            // Continue to find the second zero-crossing component.
            auto next = trivial;
            for (++next; *next && next != components_.end(); ++next)
                ;
            if (next == components_.end())
                throw InvalidArgument("graft(): two null references were given "
                    "but this link only has one zero-crossing component");
        }

        // Absorb the first zero-crossing component into the other component.
        components_.erase(trivial);
        return;
    }

    if (first == second) {
        // Split off a new zero-crossing component.
        components_.emplace_back();
        return;
    }

    // We know now that first and second are distinct and both non-null.

    // We need to know which link component they each belong to before the
    // graft takes place.
    auto firstComp = componentFor(first);
    auto secondComp = componentFor(second);

    // Perform the graft.
    StrandRef tmp = second.next();
    join(second, first.next()); // changes second.next()
    join(first, tmp);

    // Update the list of components.
    if (firstComp == secondComp) {
        // We have just split one component into two.
        if (componentFor(first) == components_.end())
            components_.push_back(first);
        else
            components_.push_back(second);
    } else {
        // We have just merged two components into one.
        components_.erase(secondComp);
    }
}

} // namespace regina

