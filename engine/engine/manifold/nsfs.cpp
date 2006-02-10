
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <algorithm>
#include <iterator>
#include <sstream>
#include "algebra/nabeliangroup.h"
#include "manifold/nlensspace.h"
#include "manifold/nsfs.h"
#include "maths/nmatrixint.h"
#include "maths/numbertheory.h"
#include "triangulation/ntriangulation.h"
#include "utilities/boostutils.h"

namespace regina {

namespace {
    /**
     * Some small exceptional fibres that we will use for comparisons.
     */
    NSFSFibre two(2, 1);
    NSFSFibre three(3, 1);
    NSFSFibre threeB(3, 2);
    NSFSFibre four(4, 1);
}

typedef std::list<NSFSFibre>::iterator FibreIterator;
typedef std::list<NSFSFibre>::const_iterator FibreIteratorConst;

std::ostream& operator << (std::ostream& out, const NSFSFibre& f) {
    return (out << '(' << f.alpha << ',' << f.beta << ')');
}

void NSFSpace::operator = (const NSFSpace& cloneMe) {
    baseClass = cloneMe.baseClass;
    baseGenus = cloneMe.baseGenus;
    basePunctures = cloneMe.basePunctures;
    baseReflectors = cloneMe.baseReflectors;
    fibres = cloneMe.fibres;
    nFibres = cloneMe.nFibres;
    b = cloneMe.b;
}

NSFSFibre NSFSpace::getFibre(unsigned long which) const {
    FibreIteratorConst pos = fibres.begin();
    advance(pos, which);
    return *pos;
}

void NSFSpace::addHandle(bool fibreReversing) {
    // First fix the class.
    // The transitions between classes have been worked out on paper
    // case by case (in particular, following how the generators of the
    // handle relate to the new crosscap generators in the non-orientable
    // case).
    // Recall also that in the orientable case we can convert +/- to -/-,
    // and in the non-orientable case we can convert +/+/+/- to +/-/-/-
    // (where + and - correspond to fibre-preserving and fibre-reversing
    // generators respectively).  See Orlik [1972], p89 for details.
    if (fibreReversing) {
        // Fibre-reversing.
        switch (baseClass) {
            case o1:
                baseClass = o2; break;
            case n1:
                baseClass = (baseGenus % 2 == 0 ? n4 : n3); break;
            case n2:
                baseClass = n4; break;
            default:
                // No change.
                break;
        }
    } else {
        // Fibre-preserving.
        // Never changes the class.
    }

    // Finally increment the genus (either orientable or non-orientable).
    if (baseClass == o1 || baseClass == o2)
        baseGenus++;
    else
        baseGenus += 2;
}

void NSFSpace::addCrosscap(bool fibreReversing) {
    // We're making the base orbifold non-orientable.
    // Convert orientable genus to non-orientable genus if required.
    if (baseClass == o1 || baseClass == o2)
        baseGenus *= 2;

    // Now fix the class.
    // The transitions between classes have been worked out on paper
    // case by case (in particular, following how the generators of the
    // handle relate to the new crosscap generators in the non-orientable
    // case).
    // Recall also that in the orientable case we can convert +/- to -/-,
    // and in the non-orientable case we can convert +/+/+/- to +/-/-/-
    // (where + and - correspond to fibre-preserving and fibre-reversing
    // generators respectively).  See Orlik [1972], p89 for details.
    if (fibreReversing) {
        // Fibre-reversing.
        switch(baseClass) {
            case o1:
                baseClass = n2; break;
            case o2:
                baseClass = n4; break;
            case n1:
                baseClass = (baseGenus % 2 == 0 ? n4 : n3); break;
            default:
                // No change.
                break;
        }
    } else {
        // Fibre-preserving.
        switch(baseClass) {
            case o1:
                baseClass = n1; break;
            case o2:
            case n2:
            case n4:
                baseClass = n3; break;
            case n3:
                baseClass = n4; break;
            default:
                // No change.
                break;
        }
    }

    // Finally increment the genus.
    // We always have non-orientable genus here.
    baseGenus++;
}

void NSFSpace::insertFibre(long alpha, long beta) {
    // We are assuming that the parameters of this fibre are coprime and
    // that alpha is strictly positive.

    // Sanity check.
    if (alpha == 0) {
        // TODO: We should probably throw an exception here or something.
        std::cerr << "ERROR: Inserting illegal fibre (0," << beta <<
            ")." << std::endl;
        return;
    }

    // Is it a regular fibre?
    if (alpha == 1) {
        b += beta;
        return;
    }

    // Put the fibre in standard form.
    b += (beta / alpha);
    beta = beta % alpha;
    if (beta < 0) {
        beta += alpha;
        b--;
    }

    // Now we have 0 <= beta < alpha and alpha >= 2.
    nFibres++;
    NSFSFibre f(alpha, beta);
    fibres.insert(lower_bound(fibres.begin(), fibres.end(), f), f);

    // We're done!
}

void NSFSpace::reduce(bool mayReflect) {
    FibreIterator it, it2;

    // Can we negate an exceptional fibre by following an
    // orientation-reversing curve in the 3-manifold?
    bool negatable = (baseClass != o1 && baseClass != n2);

    // If the SFS is non-orientable, we can get rid of b completely and
    // convert most (if not all) exceptional fibres to beta <= alpha / 2.
    if (baseReflectors) {
        // (1,1) == (1,0).
        b = 0;
    } else if (negatable && b) {
        // (p,q) == (p,-q), and so (1,2) == (1,0).
        b = b % 2;
        if (b && nFibres) {
            // We have b == +/-1.
            // Merge this into the first exceptional fibre instead.
            // Instead of modifying the fibre directly, delete and reinsert
            // so that sorted order is maintained.
            NSFSFibre f(fibres.front().alpha,
                fibres.front().alpha - fibres.front().beta);
            fibres.pop_front();

            // Rather than doing a binary search, just hunt from the
            // front (since we haven't changed alpha, so the fibre will
            // generally stay near the front).
            it = fibres.begin();
            while (it != fibres.end() && (*it) < f)
                it++;
            fibres.insert(it, f);

            b = 0;
        }
    }

    // Completely finish off the case with no exceptional fibres.
    if (! nFibres) {
        // Not much more we can do.
        // Just reflect if it helps.
        if (mayReflect && b < 0)
            b = -b;
        return;
    }

    // FACT: There is at least one fibre.
    // Normalise them as best we can.

    if (negatable) {
        // (p,q) == (p,-q) == (1,1) (p,p-q) == (1,-1) (p,p-q).
        // We can therefore reduce fibres with large beta in pairs.
        // Except for the following cases, where we can simply reduce
        // all of them.
        if (baseReflectors || fibres.front().alpha == 2) {
            // (1,1) == (1,0) if we have reflectors, and
            // (1,1) (2,1) == (1,2) (2,-1) == (2,1) if we have some alpha = 2.
            // So we can reduce _all_ fibres with large beta.
            it = fibres.begin();
            while (it != fibres.end())
                if (it->beta * 2 > it->alpha)
                    it = negateFibreDown(it);
                else
                    it++;
        } else {
            // We have to do them in pairs.

            // A place to store the first of a pair while we look for
            // the second:
            it2 = fibres.end();

            it = fibres.begin();
            while (it != fibres.end()) {
                if (it->beta * 2 > it->alpha) {
                    // This one's worth reducing.
                    if (it2 == fibres.end()) {
                        // First in a pair.
                        // Remember it and move on.
                        it2 = it++;
                    } else {
                        // Second in a pair.
                        // Process them both (first then last, so we
                        // don't mess up the sequence of iterators in
                        // the loop).
                        negateFibreDown(it2);
                        it = negateFibreDown(it);

                        it2 = fibres.end();
                    }
                } else
                    it++;
            }

            // Was there anything left over?  If so, pair it with the
            // final fibre (which will get larger, not smaller).
            if (it2 != fibres.end()) {
                negateFibreDown(it2);

                // No need to resort the final fibre, since it gets
                // larger anyway.
                fibres.back().beta = fibres.back().alpha - fibres.back().beta;
            }
        }
    } else if (baseReflectors) {
        // Individual fibres cannot be negated, but we have reflector
        // boundaries.
        // We still have the option of simultaneously replacing all (p,q)
        // with (p,-q) == (1,-1) (p,p-q) == (p,p-q) if it's worth it.

        if (mayReflect) {
            unsigned long nLarge = 0;
            unsigned long nSmall = 0;
            // Don't count (2,1) fibres, they don't get changed anyway.
            for (it = fibres.begin(); it != fibres.end() && it->alpha == 2;
                    it++)
                ;
            // Remember where we really started.
            it2 = it;
            for ( ; it != fibres.end(); it++) {
                if (it->beta * 2 > it->alpha)
                    nLarge++;
                else
                    nSmall++;
            }

            // So.  Was it worth it?
            if (nLarge > nSmall)
                complementAllFibres();
            else if (nLarge == nSmall && it2 != fibres.end() &&
                    it2->beta * 2 > it2->alpha)
                complementAllFibres();
        }
    } else {
        // Individual fibres cannot be negated, no reflector boundaries.
        // The best we can do is just reflect everything if b is far enough
        // negative.

        if (mayReflect && b < (-b - static_cast<long>(nFibres))) {
            b = -b - static_cast<long>(nFibres);
            complementAllFibres();
        }
    }
}

std::list<NSFSFibre>::iterator NSFSpace::negateFibreDown(
        std::list<NSFSFibre>::iterator it) {
    // The replacement fibre.
    NSFSFibre f(it->alpha, it->alpha - it->beta);

    // The return value.  This is also a strict upper bound for the
    // location of the replacement fibre.
    std::list<NSFSFibre>::iterator next = it;
    next++;

    // Delete the old iterator.
    fibres.erase(it);

    // Insert the new.  Treat front insertion specially, so we don't
    // find ourselves doing an it-- past the beginning.
    if (fibres.empty() || f < fibres.front()) {
        fibres.push_front(f);
        return next;
    }

    // It's not a front insertion.  Find the insertion place.
    // Note that this loop is guaranteed at least one iteration.
    for (it = next; it == fibres.end() || f < *it; it--)
        ;

    // We have the first instance of *it <= f.
    // This means the insertion should take place immediately after it.
    it++;
    fibres.insert(it, f);
    return next;
}

void NSFSpace::complementAllFibres() {
    FibreIterator it, it2, next;
    for (it = fibres.begin(); it != fibres.end(); it++)
        it->beta = it->alpha - it->beta;

    // Ensure that the array remains in sorted order.
    // Each portion of the array with fixed index must be reversed.
    NSFSFibre tmpFibre;
    it = fibres.begin();
    while (it != fibres.end()) {
        // INV: it points to the next block to be reversed.
        it2 = it;
        for (it2++; it2 != fibres.end() && (*it2).alpha == (*it).alpha; it2++)
            ;

        // Now it2 points to the first element of the following block.
        next = it2;
        it2--;

        // Now it2 points to the last element of this block.
        // Reverse this block by swapping elements at each end and
        // working towards the centre.
        while (it != it2) {
            tmpFibre = (*it);
            (*it) = (*it2);
            (*it2) = tmpFibre;

            it++;
            if (it == it2)
                break;
            it2--;
        }

        // Move on to the next block.
        it = next;
    }
}

NLensSpace* NSFSpace::isLensSpace() const {
    if (basePunctures || baseReflectors) {
        // Not a chance.
        return 0;
    }

    if (baseGenus == 0 && baseClass == o1) {
        // Base orbifold is the sphere.
        if (fibres.empty())
            return new NLensSpace(b >= 0 ? b : -b, 1);
        else if (nFibres == 1) {
            long q = fibres.front().alpha;
            long p = fibres.front().beta + (b * q);

            // We have SFS [S2 : (q,p)].
            return new NLensSpace(p >= 0 ? p : -p, q >= 0 ? q : -q);
        } else if (nFibres == 2) {
            // Precisely two fibres.
            long q = fibres.back().alpha;
            long p = fibres.back().beta + (b * q);
            long x = fibres.front().alpha;
            long y = fibres.front().beta;

            // INV: We have SFS [S2 : (x,y) (q,p)] with 0 <= y < x.
            while (y > 0) {
                x = x - y;
                q = q + p;
                if (y >= x) {
                    p += (q * (y / x));
                    y = y % x;
                }
            }

            // We should now have (x,y) == (1,0).
            return new NLensSpace(p >= 0 ? p : -p, q >= 0 ? q : -q);
        }

        // Not a lens space.
        return 0;
    } else if (baseGenus == 1 && baseClass == n2) {
        // Base orbifold is the projective plane.
        if (nFibres == 1) {
            // We have precisely one exceptional fibre.
            long a = fibres.front().alpha;
            long n = b * a + fibres.front().beta;

            if (n == 1 || n == -1)
                return new NLensSpace(4 * a, 2 * a - 1);
        }

        // Not a lens space.
        return 0;
    }

    return 0;
}

bool NSFSpace::operator < (const NSFSpace& compare) const {
    if (baseGenus < compare.baseGenus)
        return true;
    if (baseGenus > compare.baseGenus)
        return false;

    if (baseReflectors < compare.baseReflectors)
        return true;
    if (baseReflectors > compare.baseReflectors)
        return false;

    if (basePunctures < compare.basePunctures)
        return true;
    if (basePunctures > compare.basePunctures)
        return false;

    if (baseClass < compare.baseClass)
        return true;
    if (baseClass > compare.baseClass)
        return false;

    if (nFibres < compare.nFibres)
        return true;
    if (nFibres > compare.nFibres)
        return false;

    if (fibres < compare.fibres)
        return true;
    if (compare.fibres < fibres)
        return false;

    if (b < compare.b)
        return true;
    if (b > compare.b)
        return false;

    // Exactly the same!
    return false;
}

NTriangulation* NSFSpace::construct() const {
    // Things that we don't deal with just yet.
    if (basePunctures || baseReflectors)
        return 0;

    // We already know how to construct lens spaces.
    NLensSpace* lens = isLensSpace();
    if (lens) {
        NTriangulation* t = lens->construct();
        delete lens;
        return t;
    }

    // Currently we work over the 2-sphere only.
    if (baseGenus != 0 || baseClass != o1)
        return 0;

    // Pull off the number of fibres we're capable of dealing with.
    // At this moment this is three.
    if (nFibres > 3)
        return 0;

    NSFSFibre fibre[3];
    std::copy(fibres.begin(), fibres.end(), fibre);

    // Since we're working over the 2-sphere and we've already dealt
    // with lens spaces, we must in fact have precisely three
    // exceptional fibres.

    // Some parameters allow particularly nice triangulations.
    if (b == -1 && fibre[0] == two && fibre[1] == two && fibre[2].beta == 1) {
        // (2, 1) (2, 1) (a, -a+1)
        NTriangulation* ans = new NTriangulation();
        ans->insertLayeredLoop(fibre[2].alpha, true);
        return ans;
    }

    // Fall through to the default case.
    NTriangulation* ans = new NTriangulation();
    ans->insertAugTriSolidTorus(fibre[0].alpha, fibre[0].beta,
        fibre[1].alpha, fibre[1].beta, fibre[2].alpha,
        fibre[2].beta + (b - 1) * fibre[2].alpha);
    return ans;
}

NAbelianGroup* NSFSpace::getHomologyH1() const {
    if (basePunctures || baseReflectors) {
        // Not just now.
        return 0;
    }

    // Construct the presentation of the fundamental group and
    // abelianise.  The presentation is given on p91 of Orlik [1972].
    NAbelianGroup* ans = new NAbelianGroup();

    if (baseClass == o1 || baseClass == o2) {
        // Orientable base surface.
        // Generators: a_1, b_1, ..., a_g, b_g, q_1, q_2, ..., q_r, h
        // Relations:
        //     q_j^alpha_j h^beta_j = 1
        //     q_1 ... q_r = h^b
        //     h^2 = 1 (only for class o2)
        //
        // We ignore a_i and b_i, and just add extra rank 2g at the end.
        // Generators in the matrix are q_1, ..., q_r, h.
        NMatrixInt pres(nFibres + (baseClass == o1 ? 1 : 2), nFibres + 1);

        unsigned long which = 0;
        for (FibreIteratorConst it = fibres.begin(); it != fibres.end(); it++) {
            pres.entry(nFibres, which) = 1;

            pres.entry(which, nFibres) = it->beta;
            pres.entry(which, which) = it->alpha;

            which++;
        }

        pres.entry(nFibres, nFibres) = -b;
        if (baseClass == o2)
            pres.entry(nFibres + 1, nFibres) = 2;

        ans->addGroup(pres);
        ans->addRank(2 * baseGenus);
    } else {
        // Non-orientable base surface.
        // Generators: v_1, v_2, ..., v_g, q_1, q_2, ..., q_r, h
        // Relations:
        //     q_j^alpha_j h^beta_j = 1
        //     q_1 ... q_r v_1^2 ... v_g^2 = h^b
        //     h^2 = 1 (only for classes n2, n3, n4)
        //
        // Generators in the matrix are q_1, ..., q_r, v_1, ..., v_g, h.
        NMatrixInt pres(nFibres + (baseClass == n1 ? 1 : 2),
            nFibres + baseGenus + 1);

        unsigned long which = 0;
        for (FibreIteratorConst it = fibres.begin(); it != fibres.end(); it++) {
            pres.entry(nFibres, which) = 1;

            pres.entry(which, nFibres + baseGenus) = it->beta;
            pres.entry(which, which) = it->alpha;

            which++;
        }

        for (which = 0; which < baseGenus; which++)
            pres.entry(nFibres, nFibres + which) = 2;
        pres.entry(nFibres, nFibres + baseGenus) = -b;

        if (baseClass != n1)
            pres.entry(nFibres + 1, nFibres + baseGenus) = 2;

        ans->addGroup(pres);
    }

    return ans;
}

void NSFSpace::writeBaseExtraCount(std::ostream& out, unsigned long count,
        const char* object, bool tex) {
    out << " + " << count << (tex ? "\\ \\mbox{" : " ") << object;
    if (count != 1)
        out << 's';
    if (tex)
        out << '}';
}

std::ostream& NSFSpace::writeCommonBase(std::ostream& out, bool tex) const {
    bool named = false;

    // IMPORTANT: We do not allow spaces with > 2 reflector boundary
    // components to be named.  Otherwise this messes up the reflector
    // boundary output.
    unsigned long bdries = basePunctures + baseReflectors;

    if (baseClass == o1 || baseClass == o2) {
        // Orientable base surface.
        if (baseGenus == 0 && bdries == 0) {
            out << (tex ? "S^2" : "S2");
            named = true;
        } else if (baseGenus == 0 && bdries == 1) {
            if (baseReflectors && tex)
                out << "\\overline{";

            out << 'D';

            if (baseReflectors)
                out << (tex ? '}' : '_');

            named = true;
        } else if (baseGenus == 0 && bdries == 2) {
            if (baseReflectors == 1 && tex)
                out << "\\overline{";
            else if (baseReflectors == 2 && tex)
                out << "\\overline{\\overline{";

            out << 'A';

            if (baseReflectors == 1)
                out << (tex ? '}' : '_');
            else if (baseReflectors == 2)
                out << (tex ? "}}" : "=");

            named = true;
        } else if (baseGenus == 1 && bdries == 0) {
            out << (tex ? "T^2" : "T");
            named = true;
        }
    } else {
        // Non-orientable base surface.
        if (baseGenus == 1 && bdries == 0) {
            out << (tex ? "\\mathbb{R}P^2" : "RP2");
            named = true;
        } else if (baseGenus == 1 && bdries == 1) {
            if (baseReflectors && tex)
                out << "\\overline{";

            out << 'M';

            if (baseReflectors)
                out << (tex ? '}' : '_');

            named = true;
        } else if (baseGenus == 2 && bdries == 0) {
            out << (tex ? "K^2" : "KB");
            named = true;
        }
    }

    if (! named) {
        if (baseClass == o1 || baseClass == o2)
            out << (tex ? "\\mathrm{Or},\\ " : "Or, ")
                << "g=" << baseGenus;
        else
            out << (tex ? "\\mathrm{Non-or},\\ " : "Non-or, ")
                << "g=" << baseGenus;

        // TODO: Should we make it clear that these are ordinary boundary
        // components, and that reflector boundaries are not included?
        writeBaseExtraCount(out, basePunctures, "puncture", tex);
    }

    if (baseReflectors && ! named)
        writeBaseExtraCount(out, baseReflectors, "reflector", tex);

    if (baseClass == o2)
        out << (tex ? "/o_2" : "/o2");
    else if (baseClass == n2)
        out << (tex ? "/n_2" : "/n2");
    else if (baseClass == n3)
        out << (tex ? "/n_3" : "/n3");
    else if (baseClass == n4)
        out << (tex ? "/n_4" : "/n4");

    return out;
}

std::ostream& NSFSpace::writeCommonStructure(std::ostream& out, bool tex)
        const {
    if (b == 0 && fibres.empty()) {
        // We have a straightforward product (possibly twisted).
        writeCommonBase(out, tex);

        if (baseClass == o1 || baseClass == n1)
            return out << (tex ? " \\times S^1" : " x S1");
        else
            return out << (tex ? " \\twisted S^1" : " x~ S1");
    }

    // We have at least one fibre, even if it's only (1,b).

    out << (tex ? "\\mathrm{SFS}\\left(" : "SFS [");

    writeCommonBase(out, tex);

    out << ':';
    if (fibres.empty()) {
        // We have b non-zero.
        out << ' ' << NSFSFibre(1, b);
    } else {
        out << ' ';
        copy(fibres.begin(), --fibres.end(),
            std::ostream_iterator<NSFSFibre>(out, " "));

        NSFSFibre final = fibres.back();
        final.beta += final.alpha * b;
        out << final;
    }

    return out << (tex ? "\\right)" : "]");
}

std::ostream& NSFSpace::writeCommonName(std::ostream& out, bool tex) const {
    // Lens spaces.
    NLensSpace* lens = isLensSpace();
    if (lens) {
        if (tex)
            lens->writeTeXName(out);
        else
            lens->writeName(out);
        delete lens;
        return out;
    }

    // Things we don't deal with just yet.
    if (baseClass != o1 && baseClass != n2)
        return writeStructure(out);
    if (baseReflectors || basePunctures)
        return writeStructure(out);

    // FACT: We have an orientable 3-manifold whose base orbifold has no
    // punctures or reflector boundary components.

    // Pull off the number of fibres we're capable of dealing with.
    // At this moment this is four.
    if (nFibres > 4)
        return writeStructure(out);

    NSFSFibre fibre[4];
    std::copy(fibres.begin(), fibres.end(), fibre);

    // Note that with three fibres our reduced form will always have
    // b >= -1.

    // TODO: The four non-orientable flat manifolds are on Orlik p140.

    // SFS over the 2-sphere:
    if (baseGenus == 0 && baseClass == o1) {
        if (nFibres == 4 && fibre[0] == two && fibre[1] == two &&
                fibre[2] == two && fibre[3] == two && b == -2) {
            // [ S2 : (2,1), (2,1), (2,-1), (2,-1) ]
            // Orlik, p138, case M2.
            return out << (tex ? "K^2/n2 \\twisted S^1" : "KB/n2 x~ S1");
        } else if (nFibres == 3 && fibre[0] == two &&
                gcd(fibre[2].alpha, fibre[2].beta) == 1 && b >= -1) {
            // [ S2 : (2,1), (...), (...) ]

            if (fibre[1] == two) {
                // [ S2 : (2,1), (2,1), (a,b) ].
                // Orlik, p112, case (ii).

                long a = fibre[2].alpha;
                long m = fibre[2].beta + a * (b + 1);

                // Note that a,m >= 0.

                if (gcd(m, 2 * a) == 1) {
                    // S3/Q{4a} x Z{m}.
                    if (tex)
                        out << "S^3/Q_{" << (a * 4) << '}';
                    else
                        out << "S3/Q" << (a * 4);

                    if (m > 1) {
                        if (tex)
                            out << " \\times \\mathbb{Z}_{" << m << '}';
                        else
                            out << " x Z" << m;
                    }

                    return out;
                } else if (m % 2 == 0) {
                    // S3/D{2^{k+2}a} x Z{2m''+1} where m=2^k(2m''+1).
                    // It seems Orlik is missing a factor of two here?
                    // He uses m=2^{k+1}(2m''+1).
                    long odd = m;
                    long twos = 1;
                    while (! (odd & 1)) {
                        odd >>= 1;
                        twos <<= 1;
                    }

                    if (tex)
                        out << "S^3/D_{" << ((twos << 2) * a) << '}';
                    else
                        out << "S3/D" << ((twos << 2) * a);

                    if (odd > 1) {
                        if (tex)
                            out << " \\times \\mathbb{Z}_{" << odd << '}';
                        else
                            out << " x Z" << odd;
                    }

                    return out;
                }
            } else if (fibre[1] == three || fibre[1] == threeB) {
                // [ S2 : (2,1), (3,1/2), (a,b) ]
                long a = fibre[2].alpha;

                if (a == 3) {
                    // [ S2 : (2,1), (3,x), (3,y) ]
                    // Orlik, p112, case (iii).
                    long m = 6 * b + 3 + 2 * (fibre[1].beta + fibre[2].beta);
                    // Note that m >= 1.

                    if (m % 2 != 0 && m % 3 != 0) {
                        out << (tex ? "S^3/P_{24}" : "S3/P24");

                        if (m > 1) {
                            if (tex)
                                out << " \\times \\mathbb{Z}_{" << m << '}';
                            else
                                out << " x Z" << m;
                        }

                        return out;
                    } else if (m % 2 != 0) {
                        long threes = 1;
                        while (m % 3 == 0) {
                            m = m / 3;
                            threes *= 3;
                        }

                        // I believe Orlik is missing a factor of three.
                        // He claims this should be (threes * 8).
                        if (tex)
                            out << "S^3/P'_{" << (threes * 24) << '}';
                        else
                            out << "S3/P'" << (threes * 24);

                        if (m > 1) {
                            if (tex)
                                out << " \\times \\mathbb{Z}_{" << m << '}';
                            else
                                out << " x Z" << m;
                        }

                        return out;
                    }
                } else if (a == 4) {
                    // [ S2 : (2,1), (3,x), (4,y) ]
                    // Orlik, p112, case (iv).
                    long m = 12 * b + 6 + 4 * fibre[1].beta +
                        3 * fibre[2].beta;
                    // Note that m >= 1.

                    out << (tex ? "S^3/P_{48}" : "S3/P48");

                    if (m > 1) {
                        if (tex)
                            out << " \\times \\mathbb{Z}_{" << m << '}';
                        else
                            out << " x Z" << m;
                    }

                    return out;
                } else if (a == 5) {
                    // [ S2 : (2,1), (3,x), (5,y) ]
                    // Orlik, p112, case (v).
                    long m = 30 * b + 15 + 10 * fibre[1].beta +
                        6 * fibre[2].beta;
                    // Note that m >= 1.

                    out << (tex ? "S^3/P_{120}" : "S3/P120");

                    if (m > 1) {
                        if (tex)
                            out << " \\times \\mathbb{Z}_{" << m << '}';
                        else
                            out << " x Z" << m;
                    }

                    return out;
                } else if (a == 6 && fibre[1].beta == 1 &&
                        fibre[2].beta == 1 && b == -1) {
                    // [ S2 : (2,1), (3,1), (6,-5) ].
                    // Orlik, p138, case M5.
                    if (tex)
                        return out << "T^2 \\times I / \\homtwo{1}{1}{-1}{0}";
                    else
                        return out << "T x I / [ 1,1 | -1,0 ]";
                }
            } else if (fibre[1] == four && fibre[2] == four && b == -1) {
                // [ S2 : (2,1), (4,1), (4,-3) ].
                // Orlik, p138, case M4.
                if (tex)
                    return out << "T^2 \\times I / \\homtwo{0}{-1}{1}{0}";
                else
                    return out << "T x I / [ 0,-1 | 1,0 ]";
            }
        } else if (nFibres == 3 && fibre[0] == three && fibre[1] == three
                && fibre[2] == three && b == -1) {
            // [ S2 : (3,1), (3,1), (3,-2) ]
            // Orlik, p138, case M3.
            if (tex)
                return out << "T^2 \\times I / \\homtwo{0}{-1}{1}{-1}";
            else
                return out << "T x I / [ 0,-1 | 1,-1 ]";
        }
    }

    // SFS over the real projective plane:
    if (baseGenus == 1 && baseClass == n2) {
        if (nFibres == 0) {
            // No exceptional fibres.
            if (b == 0) {
                // [ RP2 ]
                // Orlik, p113, remark.
                return out << (tex ? "\\mathbb{R}P^3 \\# \\mathbb{R}P^3" :
                    "RP3 # RP3");
            } else {
                // TODO: [ RP2 : (1,b) ]
                // Is this Orlik, p112, case (vi)?  What is this?
                // ans << "S3/Q" << (4 * (b > 0 ? b : -b));
            }
        } else if (nFibres == 1 && fibre[0].alpha > 1) {
            // Just one exceptional fibre.
            long a = fibre[0].alpha;
            long n = b * a + fibre[0].beta;
            if (n < 0)
                n = -n;

            if (n > 1) {
                // We have a prism manifold.
                // Orlik, p112, case (vi).
                if (a % 2 != 0) {
                    return (tex ?
                        out << "S^3/Q_{" << (4 * n) <<
                            "} \\times \\mathbb{Z}_{" << a << "}":
                        out << "S3/Q" << (4 * n) << " x Z" << a);
                } else {
                    long odd = a;
                    long twos = 1;
                    while (! (odd & 1)) {
                        odd >>= 1;
                        twos <<= 1;
                    }

                    if (tex)
                        out << "S^3/D_{" << ((twos << 2) * n) << '}';
                    else
                        out << "S3/D" << ((twos << 2) * n);

                    if (odd > 1) {
                        if (tex)
                            out << " \\times \\mathbb{Z}_{" << odd << '}';
                        else
                            out << " x Z" << odd;
                    }

                    return out;
                }
            }
        }
    }

    return writeStructure(out);
}

} // namespace regina
