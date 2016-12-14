
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

#include "manifold/sfs.h"
#include "subcomplex/blockedsfs.h"
#include "subcomplex/layeredsolidtorus.h"
#include "subcomplex/satblockstarter.h"
#include "subcomplex/satblocktypes.h"
#include "subcomplex/satregion.h"
#include <cstdlib> // For labs().
#include <sstream>

namespace regina {

namespace {
    /**
     * An anonymous inline boolean xor.  Experiences with plain C have
     * spoiled me for life from using equality/xor operators with bools.
     */
    inline bool regXor(bool a, bool b) {
        return ((a && ! b) || (b && !a));
    }
}

/**
 * A subclass of SatBlockStarterSearcher that, upon finding a starter
 * saturated block, attempts to flesh this out to a saturated region
 * that fills the entire triangulation (including all internal triangles).
 */
struct BlockedSFSSearcher : public SatBlockStarterSearcher {
    SatRegion* region;
        /**< The saturated region if one has been found, or 0
             if we are still searching. */

    /**
     * Creates a new searcher whose \a region pointer is null.
     */
    BlockedSFSSearcher() : region(0) {
    }

    protected:
        bool useStarterBlock(SatBlock* starter);
};

BlockedSFS::~BlockedSFS() {
    if (region_)
        delete region_;
}

bool BlockedSFS::isPluggedIBundle(std::string& name) const {
    // The triangulation needs to be closed.
    if (region_->numberOfBoundaryAnnuli() > 0)
        return false;

    unsigned long n = region_->numberOfBlocks();
    if (n < 3 || n > 4)
        return false;

    // Try one thing at a time.
    const SatBlock* block;
    const SatCube* cube;
    const SatReflectorStrip* ref;
    const SatTriPrism* tri;
    const SatTriPrism* triAdj;
    unsigned adjAnn;
    unsigned long i, j;
    int delta, deltaAdj;
    bool consistent;
    for (i = 0; i < n; i++) {
        block = region_->block(i).block;

        cube = dynamic_cast<const SatCube*>(block);
        if (cube) {
            if (cube->adjacentBlock(0) == cube &&
                    cube->adjacentAnnulus(0) == 2) {
                if (cube->adjacentReflected(0) || cube->adjacentBackwards(0))
                    return false;
                return findPluggedTori(true, 3, name,
                    cube->adjacentBlock(1), true,
                    cube->adjacentBlock(3), false);
            } else if (cube->adjacentBlock(1) == cube &&
                    cube->adjacentAnnulus(1) == 3) {
                if (cube->adjacentReflected(1) || cube->adjacentBackwards(1))
                    return false;
                return findPluggedTori(true, 3, name,
                    cube->adjacentBlock(0), true,
                    cube->adjacentBlock(2), false);
            } else if (cube->adjacentBlock(0) == cube &&
                    cube->adjacentAnnulus(0) == 1) {
                if (cube->adjacentReflected(0) || cube->adjacentBackwards(0))
                    return false;
                return findPluggedTori(false, 1, name,
                    cube->adjacentBlock(2), false,
                    cube->adjacentBlock(3), true);
            } else if (cube->adjacentBlock(1) == cube &&
                    cube->adjacentAnnulus(1) == 2) {
                if (cube->adjacentReflected(1) || cube->adjacentBackwards(1))
                    return false;
                return findPluggedTori(false, 1, name,
                    cube->adjacentBlock(3), false,
                    cube->adjacentBlock(0), true);
            } else if (cube->adjacentBlock(2) == cube &&
                    cube->adjacentAnnulus(2) == 3) {
                if (cube->adjacentReflected(2) || cube->adjacentBackwards(2))
                    return false;
                return findPluggedTori(false, 1, name,
                    cube->adjacentBlock(0), false,
                    cube->adjacentBlock(1), true);
            } else if (cube->adjacentBlock(3) == cube &&
                    cube->adjacentAnnulus(3) == 0) {
                if (cube->adjacentReflected(3) || cube->adjacentBackwards(3))
                    return false;
                return findPluggedTori(false, 1, name,
                    cube->adjacentBlock(1), false,
                    cube->adjacentBlock(2), true);
            }
        }

        ref = dynamic_cast<const SatReflectorStrip*>(block);
        if (ref) {
            if (ref->twistedBoundary())
                return false;

            if (ref->nAnnuli() == 1) {
                tri = dynamic_cast<const SatTriPrism*>(ref->adjacentBlock(0));
                if (! tri)
                    return false;

                adjAnn = ref->adjacentAnnulus(0);
                if (tri->isMajor())
                    return findPluggedTori(false, 4, name,
                        tri->adjacentBlock((adjAnn + 2) % 3), true,
                        tri->adjacentBlock((adjAnn + 1) % 3), false);
                else
                    return findPluggedTori(false, 4, name,
                        tri->adjacentBlock((adjAnn + 1) % 3), false,
                        tri->adjacentBlock((adjAnn + 2) % 3), true);
            } else if (ref->nAnnuli() == 2) {
                return findPluggedTori(true, 4, name,
                    ref->adjacentBlock(0), true,
                    ref->adjacentBlock(1), true);
            } else
                return false;
        }

        tri = dynamic_cast<const SatTriPrism*>(block);
        if (tri) {
            for (j = 0; j < 3; j++) {
                // Try the thick case...
                if (tri->adjacentBlock(j) == tri &&
                        tri->adjacentAnnulus(j) == ((j + 1) % 3)) {
                    if (tri->adjacentReflected(j) || tri->adjacentBackwards(j))
                        return false;

                    triAdj = dynamic_cast<const SatTriPrism*>(
                        tri->adjacentBlock((j + 2) % 3));
                    if (! triAdj)
                        return false;

                    // Do we have major to major and minor to minor?
                    consistent = true;
                    if (tri->isMajor())
                        consistent = ! consistent;
                    if (triAdj->isMajor())
                        consistent = ! consistent;
                    if (tri->adjacentReflected((j + 2) % 3))
                        consistent = ! consistent;
                    if (tri->adjacentBackwards((j + 2) % 3))
                        consistent = ! consistent;

                    adjAnn = tri->adjacentAnnulus((j + 2) % 3);
                    if (consistent) {
                        if (triAdj->isMajor())
                            return findPluggedTori(false, 2, name,
                                triAdj->adjacentBlock((adjAnn + 1) % 3), false,
                                triAdj->adjacentBlock((adjAnn + 2) % 3), true);
                        else
                            return findPluggedTori(false, 2, name,
                                triAdj->adjacentBlock((adjAnn + 2) % 3), true,
                                triAdj->adjacentBlock((adjAnn + 1) % 3), false);
                    } else {
                        if (triAdj->isMajor())
                            return findPluggedTori(false, 3, name,
                                triAdj->adjacentBlock((adjAnn + 2) % 3), true,
                                triAdj->adjacentBlock((adjAnn + 1) % 3), true);
                        else
                            return findPluggedTori(false, 3, name,
                                triAdj->adjacentBlock((adjAnn + 1) % 3), false,
                                triAdj->adjacentBlock((adjAnn + 2) % 3), false);
                    }
                }

                // ... and try the thin case.
                if (! (triAdj = dynamic_cast<const SatTriPrism*>(
                        tri->adjacentBlock(j))))
                    continue;

                // Do we have major to major and minor to minor?
                consistent = true;
                if (tri->isMajor())
                    consistent = ! consistent;
                if (triAdj->isMajor())
                    consistent = ! consistent;
                if (tri->adjacentReflected(j))
                    consistent = ! consistent;
                if (tri->adjacentBackwards(j))
                    consistent = ! consistent;

                adjAnn = tri->adjacentAnnulus(j);

                for (delta = 1; delta <= 2; delta++)
                    if (tri->adjacentBlock((j + delta) % 3) == triAdj) {
                        if (regXor(tri->adjacentReflected(j),
                                tri->adjacentReflected((j + delta) % 3)))
                            return false;
                        if (! regXor(tri->adjacentBackwards(j),
                                tri->adjacentBackwards((j + delta) % 3)))
                            return false;

                        // We have our Mobius strip!
                        // Make sure we come at it via the correct joining.
                        deltaAdj = (tri->adjacentBackwards(j) ?
                            3 - delta : delta);
                        if (tri->adjacentAnnulus((j + delta) % 3) !=
                                (adjAnn + deltaAdj) % 3) {
                            // It's not the way we want to see it, but
                            // we'll come at it from the correct joining later.
                            continue;
                        }

                        // Our LSTs need to be measured against the
                        // major edges in all cases here.
                        return findPluggedTori(true, consistent ? 2 : 1, name,
                            tri->adjacentBlock((j + 2 * delta) % 3),
                                tri->isMajor(),
                            triAdj->adjacentBlock((adjAnn + 2 * deltaAdj) % 3),
                                triAdj->isMajor());
                    }
            }
        }
    }

    // Nothing.
    return false;
}

Manifold* BlockedSFS::manifold() const {
    SFSpace* ans = region_->createSFS(false);
    if (! ans)
        return 0;

    ans->reduce();

    // If we have SFS(RP2/n2) with one exceptional fibre, rewrite it as
    // SFS(S2) with three exceptional fibres.

    if (ans->baseClass() == SFSpace::n2 &&
            ans->baseGenus() == 1 &&
            (! ans->baseOrientable()) &&
            ans->punctures() == 0 &&
            ans->reflectors() == 0 &&
            ans->fibreCount() <= 1) {
        SFSpace* altAns = new SFSpace(/* S2 x S1 */);
        altAns->insertFibre(2, 1);
        altAns->insertFibre(2, -1);

        SFSFibre rp2Fibre;
        if (ans->fibreCount() == 0) {
            rp2Fibre.alpha = 1;
            rp2Fibre.beta = ans->obstruction();
        } else {
            rp2Fibre = ans->fibre(0);
            rp2Fibre.beta += rp2Fibre.alpha * ans->obstruction();
        }

        // Make sure we're not going to try inserting (0,k).
        if (rp2Fibre.beta != 0) {
            altAns->insertFibre(rp2Fibre.beta, rp2Fibre.alpha);
            altAns->reduce();

            delete ans;
            return altAns;
        } else
            delete altAns;
    }

    return ans;
}

std::ostream& BlockedSFS::writeName(std::ostream& out) const {
    out << "Blocked SFS [";
    region_->writeBlockAbbrs(out, false);
    return out << ']';
}

std::ostream& BlockedSFS::writeTeXName(std::ostream& out) const {
    out << "\\mathrm{BSFS}\\left[";
    region_->writeBlockAbbrs(out, true);
    return out << "\\right]";
}

void BlockedSFS::writeTextLong(std::ostream& out) const {
    region_->writeDetail(out, "Blocked SFS");
}

BlockedSFS* BlockedSFS::isBlockedSFS(Triangulation<3>* tri) {
    // Basic property checks.
    if (tri->countComponents() > 1)
        return 0;
    if (tri->isIdeal())
        return 0;

    // Watch out for twisted block boundaries that are incompatible with
    // neighbouring blocks!  These will result in edges joined to
    // themselves in reverse.
    if (! tri->isValid())
        return 0;

    // Hunt for a starting block.
    BlockedSFSSearcher searcher;
    searcher.findStarterBlocks(tri);

    // Any luck?
    if (searcher.region) {
        // The region expansion worked, and the triangulation is known
        // to be connected.
        // This means we've got one!
        return new BlockedSFS(searcher.region);
    }

    // Nope.
    return 0;
}

bool BlockedSFSSearcher::useStarterBlock(SatBlock* starter) {
    // The region pointer should be null, but just in case...
    if (region) {
        delete starter;
        return false;
    }

    // See if we can flesh out an entire triangulation component from
    // the starter block.  At this point the region will own the given
    // starter block.
    region = new SatRegion(starter);
    if (! region->expand(usedTets, true)) {
        // Nup.  Destroy the temporary structures and keep searching.
        delete region;
        region = 0;
        return true;
    }

    // Got one!  Stop the search.
    return false;
}

bool BlockedSFS::findPluggedTori(bool thin, int id, std::string& name,
        const SatBlock* torus0, bool horiz0,
        const SatBlock* torus1, bool horiz1) {
    long p0, q0;
    long p1, q1;

    if (torus0->adjacentReflected(0))
        horiz0 = ! horiz0;
    if (torus0->adjacentBackwards(0))
        horiz0 = ! horiz0;
    if (torus1->adjacentReflected(1))
        horiz1 = ! horiz1;
    if (torus1->adjacentBackwards(1))
        horiz1 = ! horiz1;

    const SatLST* lst;
    const SatMobius* mobius;
    Perm<4> roles;

    if ((mobius = dynamic_cast<const SatMobius*>(torus0))) {
        if (mobius->position() == 2) {
            p0 = 2;
            q0 = -1;
        } else if (mobius->position() == 1) {
            p0 = 1;
            q0 = (horiz0 ? -2 : 1);
        } else {
            p0 = 1;
            q0 = (horiz0 ? 1 : -2);
        }
    } else if ((lst = dynamic_cast<const SatLST*>(torus0))) {
        roles = lst->roles();
        p0 = lst->lst()->meridinalCuts(roles[0]);
        q0 = lst->lst()->meridinalCuts(roles[horiz0 ? 1 : 2]);
        if (! ((roles[2] == 2 && horiz0) || (roles[1] == 2 && ! horiz0)))
            q0 = -q0;
    } else
        return false;

    if ((mobius = dynamic_cast<const SatMobius*>(torus1))) {
        if (mobius->position() == 2) {
            p1 = 2;
            q1 = -1;
        } else if (mobius->position() == 1) {
            p1 = 1;
            q1 = (horiz1 ? -2 : 1);
        } else {
            p1 = 1;
            q1 = (horiz1 ? 1 : -2);
        }
    } else if ((lst = dynamic_cast<const SatLST*>(torus1))) {
        roles = lst->roles();
        p1 = lst->lst()->meridinalCuts(roles[0]);
        q1 = lst->lst()->meridinalCuts(roles[horiz1 ? 1 : 2]);
        if (! ((roles[2] == 2 && horiz1) || (roles[1] == 2 && ! horiz1)))
            q1 = -q1;
    } else
        return false;

    // Do a little normalisation.
    if ((thin && (id == 3 || id == 4)) || ((! thin) && id == 1)) {
        // Complementing does nothing.
        if (p0 > 0 && p1 > 0 && q0 < 0 && q1 < 0 && q0 > -p0 && q1 > -p1 &&
                2 * q0 <= -p0 && 2 * q1 <= -p1) {
            q0 = -p0 - q0;
            q1 = -p1 - q1;
        }
    }

    if (labs(p1) > labs(p0) ||
            (labs(p1) == labs(p0) && labs(q1) < labs(q0))) {
        long tmp;
        if (thin || ((! thin) && (id == 1 || id ==3))) {
            // Swapping does nothing.
            tmp = p0; p0 = p1; p1 = tmp;
            tmp = q0; q0 = q1; q1 = tmp;
        } else if (id == 2 || id == 4) {
            // If we swap then we also complement.
            tmp = p0; p0 = p1; p1 = tmp;
            tmp = q0; q0 = q1; q1 = tmp;
            q0 = -p0 - q0;
            q1 = -p1 - q1;
        }
    }

    // All good.  Build the full name and quit.
    std::ostringstream ans;
    ans << (thin ? 'H' : 'K') << "(T~" << (thin ? 6 : 5) << '^' << id;
    if (p0 != 2 || q0 != -1 || p1 != 2 || q1 != -1)
        ans << " | " << p0 << ',' << q0;
    if (p1 != 2 || q1 != -1)
        ans << " | " << p1 << ',' << q1;
    ans << ')';
    name = ans.str();

    return true;
}

} // namespace regina

