
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

#include <algorithm>
#include <iterator>
#include <sstream>
#include "algebra/abeliangroup.h"
#include "manifold/lensspace.h"
#include "manifold/sfs.h"
#include "maths/matrix.h"
#include "maths/numbertheory.h"
#include "subcomplex/satannulus.h"
#include "triangulation/dim3.h"

namespace regina {

namespace {
    /**
     * Some small exceptional fibres that we will use for comparisons.
     */
    SFSFibre two(2, 1);
    SFSFibre three(3, 1);
    SFSFibre threeB(3, 2);
    SFSFibre four(4, 1);
}

typedef std::list<SFSFibre>::iterator FibreIterator;
typedef std::list<SFSFibre>::const_iterator FibreIteratorConst;

std::ostream& operator << (std::ostream& out, const SFSFibre& f) {
    return (out << '(' << f.alpha << ',' << f.beta << ')');
}

void SFSpace::operator = (const SFSpace& cloneMe) {
    class_ = cloneMe.class_;
    genus_ = cloneMe.genus_;
    punctures_ = cloneMe.punctures_;
    puncturesTwisted_ = cloneMe.puncturesTwisted_;
    reflectors_ = cloneMe.reflectors_;
    reflectorsTwisted_ = cloneMe.reflectorsTwisted_;
    fibres_ = cloneMe.fibres_;
    nFibres_ = cloneMe.nFibres_;
    b_ = cloneMe.b_;
}

SFSFibre SFSpace::fibre(unsigned long which) const {
    FibreIteratorConst pos = fibres_.begin();
    advance(pos, which);
    return *pos;
}

void SFSpace::addHandle(bool fibreReversing) {
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
        switch (class_) {
            case o1:
                class_ = o2; break;
            case n1:
                class_ = (genus_ % 2 == 0 ? n4 : n3); break;
            case n2:
                class_ = n4; break;
            case bo1:
                class_ = bo2; break;
            case bn1:
            case bn2:
                class_ = bn3; break;
            default:
                // No change.
                break;
        }
    } else {
        // Fibre-preserving.
        // Never changes the class.
    }

    // Finally increment the genus (either orientable or non-orientable).
    if (baseOrientable())
        genus_++;
    else
        genus_ += 2;
}

void SFSpace::addCrosscap(bool fibreReversing) {
    // We're making the base orbifold non-orientable.
    // Convert orientable genus to non-orientable genus if required.
    if (baseOrientable())
        genus_ *= 2;

    // Now fix the class.
    // The transitions between classes have been worked out on paper
    // case by case (in particular, following how the generators of the
    // original handles relate to the reformulated crosscap generators in
    // the orientable case).
    // Recall also that in the orientable case we can convert +/- to -/-,
    // and in the non-orientable case we can convert +/+/+/- to +/-/-/-
    // (where + and - correspond to fibre-preserving and fibre-reversing
    // generators respectively).  See Orlik [1972], p89 for details.
    if (fibreReversing) {
        // Fibre-reversing.
        switch(class_) {
            case o1:
                class_ = n2; break;
            case o2:
                class_ = n4; break;
            case n1:
                class_ = (genus_ % 2 == 0 ? n4 : n3); break;
            case bo1:
                class_ = bn2; break;
            case bo2:
            case bn1:
                class_ = bn3; break;
            default:
                // No change.
                break;
        }
    } else {
        // Fibre-preserving.
        switch(class_) {
            case o1:
                class_ = n1; break;
            case o2:
            case n2:
            case n4:
                class_ = n3; break;
            case n3:
                class_ = n4; break;
            case bo1:
                class_ = bn1; break;
            case bo2:
            case bn2:
                class_ = bn3; break;
            default:
                // No change.
                break;
        }
    }

    // Finally increment the genus.
    // We always have non-orientable genus here.
    genus_++;
}

void SFSpace::addPuncture(bool twisted, unsigned long nPunctures) {
    if (twisted) {
        puncturesTwisted_ += nPunctures;

        if (baseOrientable())
            class_ = bo2;
        else
            class_ = bn3;
    } else {
        punctures_ += nPunctures;

        switch(class_) {
            case o1:
                class_ = bo1; break;
            case o2:
                class_ = bo2; break;
            case n1:
                class_ = bn1; break;
            case n2:
                class_ = bn2; break;
            case n3:
            case n4:
                class_ = bn3; break;
            default:
                // No change.
                break;
        }
    }
}

void SFSpace::addReflector(bool twisted, unsigned long nReflectors) {
    if (twisted) {
        reflectorsTwisted_ += nReflectors;

        if (baseOrientable())
            class_ = bo2;
        else
            class_ = bn3;
    } else {
        reflectors_ += nReflectors;

        switch(class_) {
            case o1:
                class_ = bo1; break;
            case o2:
                class_ = bo2; break;
            case n1:
                class_ = bn1; break;
            case n2:
                class_ = bn2; break;
            case n3:
            case n4:
                class_ = bn3; break;
            default:
                // No change.
                break;
        }
    }
}

void SFSpace::insertFibre(long alpha, long beta) {
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
        b_ += beta;
        return;
    }

    // Put the fibre in standard form.
    b_ += (beta / alpha);
    beta = beta % alpha;
    if (beta < 0) {
        beta += alpha;
        b_--;
    }

    // Now we have 0 <= beta < alpha and alpha >= 2.
    nFibres_++;
    SFSFibre f(alpha, beta);
    fibres_.insert(lower_bound(fibres_.begin(), fibres_.end(), f), f);

    // We're done!
}

void SFSpace::reduce(bool mayReflect) {
    FibreIterator it, it2;

    // If the SFS is non-orientable, we can get rid of b completely and
    // convert most (if not all) exceptional fibres to beta <= alpha / 2.
    if (reflectors_ || reflectorsTwisted_) {
        // (1,1) == (1,0).
        b_ = 0;
    } else if (fibreNegating() && b_) {
        // (p,q) == (p,-q), and so (1,2) == (1,0).
        b_ = b_ % 2;
        if (b_ && nFibres_) {
            // We have b == +/-1.
            // Merge this into the first exceptional fibre instead.
            // Instead of modifying the fibre directly, delete and reinsert
            // so that sorted order is maintained.
            SFSFibre f(fibres_.front().alpha,
                fibres_.front().alpha - fibres_.front().beta);
            fibres_.pop_front();

            // Rather than doing a binary search, just hunt from the
            // front (since we haven't changed alpha, so the fibre will
            // generally stay near the front).
            it = fibres_.begin();
            while (it != fibres_.end() && (*it) < f)
                it++;
            fibres_.insert(it, f);

            b_ = 0;
        }
    }

    // Completely finish off the case with no exceptional fibres.
    if (! nFibres_) {
        // Not much more we can do.
        // Just reflect if it helps.
        if (mayReflect && b_ < 0)
            b_ = -b_;
        return;
    }

    // FACT: There is at least one fibre.
    // Normalise them as best we can.

    if (fibreNegating()) {
        // (p,q) == (p,-q) == (1,1) (p,p-q) == (1,-1) (p,p-q).
        // We can therefore reduce fibres with large beta in pairs.
        // Except for the following cases, where we can simply reduce
        // all of them.
        if (reflectors_ || reflectorsTwisted_ || fibres_.front().alpha == 2) {
            // (1,1) == (1,0) if we have reflectors, and
            // (1,1) (2,1) == (1,2) (2,-1) == (2,1) if we have some alpha = 2.
            // So we can reduce _all_ fibres with large beta.
            it = fibres_.begin();
            while (it != fibres_.end())
                if (it->beta * 2 > it->alpha)
                    it = negateFibreDown(it);
                else
                    it++;
        } else {
            // We have to do them in pairs.

            // A place to store the first of a pair while we look for
            // the second:
            it2 = fibres_.end();

            it = fibres_.begin();
            while (it != fibres_.end()) {
                if (it->beta * 2 > it->alpha) {
                    // This one's worth reducing.
                    if (it2 == fibres_.end()) {
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

                        it2 = fibres_.end();
                    }
                } else
                    it++;
            }

            // Was there anything left over?  If so, pair it with the
            // final fibre (which will get larger, not smaller).
            if (it2 != fibres_.end()) {
                // It can be shown that, if we are already looking at
                // the final fibre, this code will do the right thing
                // (specifically, switch this with another fibre if this
                // will improve things, and switch this with itself if
                // it won't).
                negateFibreDown(it2);

                // No need to resort the final fibre, since it gets
                // larger anyway.
                fibres_.back().beta = fibres_.back().alpha -
                    fibres_.back().beta;
            }
        }
    } else if (reflectors_ || reflectorsTwisted_) {
        // Individual fibres cannot be negated, but we have reflector
        // boundaries.
        // We still have the option of simultaneously replacing all (p,q)
        // with (p,-q) == (1,-1) (p,p-q) == (p,p-q) if it's worth it.

        if (mayReflect) {
            unsigned long nLarge = 0;
            unsigned long nSmall = 0;
            // Don't count (2,1) fibres, they don't get changed anyway.
            for (it = fibres_.begin(); it != fibres_.end() && it->alpha == 2;
                    it++)
                ;
            // Remember where we really started.
            it2 = it;
            for ( ; it != fibres_.end(); it++) {
                if (it->beta * 2 > it->alpha)
                    nLarge++;
                else
                    nSmall++;
            }

            // So.  Was it worth it?
            if (nLarge > nSmall)
                complementAllFibres();
            else if (nLarge == nSmall && it2 != fibres_.end()) {
                // We need to look in a little more detail.
                FibreIterator next;
                bool shouldReflect = false;

                // Restore our starting position, and let it2 become a
                // temporary variable again.
                it = it2;
                while (it != fibres_.end()) {
                    // INV: it points to the next block with the same
                    // value of alpha.
                    it2 = it;
                    for (it2++; it2 != fibres_.end() &&
                            it2->alpha == it->alpha; it2++)
                        ;

                    // Now it2 points to the first element of the
                    // following block.
                    next = it2;
                    it2--;

                    // Now it2 points to the last element of this block.
                    // If the block were negated, it would also be
                    // reversed; see what would happen.
                    while (it != next) {
                        if (it2->alpha - it2->beta < it->beta) {
                            shouldReflect = true;
                            next = fibres_.end();
                            break;
                        } else if (it2->alpha - it2->beta > it->beta) {
                            shouldReflect = false;
                            next = fibres_.end();
                            break;
                        }

                        // Still tied.
                        it++;
                        it2--;
                    }

                    // Move on to the next block.
                    it = next;
                }

                if (shouldReflect)
                    complementAllFibres();
            }
        }
    } else {
        // Individual fibres cannot be negated, no reflector boundaries.
        // The best we can do is just reflect everything if b is far enough
        // negative.

        if (mayReflect) {
            if (b_ < (-b_ - static_cast<long>(nFibres_))) {
                b_ = -b_ - static_cast<long>(nFibres_);
                complementAllFibres();
            } else if (b_ == (-b_ - static_cast<long>(nFibres_))) {
                // Reflecting won't change b, but it will complement all
                // fibres.  See whether this is worthwhile.
                FibreIterator next;
                bool shouldReflect = false;

                it = fibres_.begin();
                while (it != fibres_.end()) {
                    // INV: it points to the next block with the same
                    // value of alpha.
                    it2 = it;
                    for (it2++; it2 != fibres_.end() &&
                            it2->alpha == it->alpha; it2++)
                        ;

                    // Now it2 points to the first element of the
                    // following block.
                    next = it2;
                    it2--;

                    // Now it2 points to the last element of this block.
                    // If the block were negated, it would also be
                    // reversed; see what would happen.
                    while (it != next) {
                        if (it2->alpha - it2->beta < it->beta) {
                            shouldReflect = true;
                            next = fibres_.end();
                            break;
                        } else if (it2->alpha - it2->beta > it->beta) {
                            shouldReflect = false;
                            next = fibres_.end();
                            break;
                        }

                        // Still tied.
                        it++;
                        it2--;
                    }

                    // Move on to the next block.
                    it = next;
                }

                if (shouldReflect)
                    complementAllFibres();
            }
        }
    }
}

std::list<SFSFibre>::iterator SFSpace::negateFibreDown(
        std::list<SFSFibre>::iterator it) {
    // The replacement fibre.
    SFSFibre f(it->alpha, it->alpha - it->beta);

    // The return value.  This is also a strict upper bound for the
    // location of the replacement fibre.
    std::list<SFSFibre>::iterator next = it;
    next++;

    // Delete the old iterator.
    fibres_.erase(it);

    // Insert the new.  Treat front insertion specially, so we don't
    // find ourselves doing an it-- past the beginning.
    if (fibres_.empty() || f < fibres_.front()) {
        fibres_.push_front(f);
        return next;
    }

    // It's not a front insertion.  Find the insertion place.
    // Note that this loop is guaranteed at least one iteration.
    for (it = next; it == fibres_.end() || f < *it; it--)
        ;

    // We have the first instance of *it <= f.
    // This means the insertion should take place immediately after it.
    it++;
    fibres_.insert(it, f);
    return next;
}

void SFSpace::complementAllFibres() {
    FibreIterator it, it2, next;
    for (it = fibres_.begin(); it != fibres_.end(); it++)
        it->beta = it->alpha - it->beta;

    // Ensure that the array remains in sorted order.
    // Each portion of the array with fixed index must be reversed.
    SFSFibre tmpFibre;
    it = fibres_.begin();
    while (it != fibres_.end()) {
        // INV: it points to the next block to be reversed.
        it2 = it;
        for (it2++; it2 != fibres_.end() && (*it2).alpha == (*it).alpha; it2++)
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

LensSpace* SFSpace::isLensSpace() const {
    if (punctures_ || puncturesTwisted_ || reflectors_ || reflectorsTwisted_) {
        // Not a chance.
        return 0;
    }

    if (genus_ == 0 && class_ == o1) {
        // Base orbifold is the sphere.
        if (fibres_.empty())
            return new LensSpace(b_ >= 0 ? b_ : -b_, 1);
        else if (nFibres_ == 1) {
            long q = fibres_.front().alpha;
            long p = fibres_.front().beta + (b_ * q);

            // We have SFS [S2 : (q,p)].
            return new LensSpace(p >= 0 ? p : -p, q >= 0 ? q : -q);
        } else if (nFibres_ == 2) {
            // Precisely two fibres.
            long q = fibres_.back().alpha;
            long p = fibres_.back().beta + (b_ * q);
            long x = fibres_.front().alpha;
            long y = fibres_.front().beta;

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
            return new LensSpace(p >= 0 ? p : -p, q >= 0 ? q : -q);
        }

        // Not a lens space.
        return 0;
    } else if (genus_ == 1 && class_ == n2) {
        // Base orbifold is the projective plane.
        if (nFibres_ == 1) {
            // We have precisely one exceptional fibre.
            long a = fibres_.front().alpha;
            long n = b_ * a + fibres_.front().beta;

            if (n == 1 || n == -1)
                return new LensSpace(4 * a, 2 * a - 1);
        }

        // Not a lens space.
        return 0;
    }

    return 0;
}

bool SFSpace::operator == (const SFSpace& compare) const {
    if (class_ != compare.class_)
        return false;
    if (genus_ != compare.genus_)
        return false;
    if (punctures_ != compare.punctures_)
        return false;
    if (puncturesTwisted_ != compare.puncturesTwisted_)
        return false;
    if (reflectors_ != compare.reflectors_)
        return false;
    if (reflectorsTwisted_ != compare.reflectorsTwisted_)
        return false;
    if (nFibres_ != compare.nFibres_)
        return false;
    if (! (fibres_ == compare.fibres_))
        return false;
    if (b_ != compare.b_)
        return false;

    // Exactly the same!
    return true;
}

bool SFSpace::operator < (const SFSpace& compare) const {
    // Double the genus if it's orientable, so that we can line up tori
    // with Klein bottles, etc.
    unsigned long adjGenus1 = (baseOrientable() ? genus_ * 2 : genus_);
    unsigned long adjGenus2 = (compare.baseOrientable() ?
        compare.genus_ * 2 : compare.genus_);

    // Too many punctures is worse than anything.
    if (punctures_ + puncturesTwisted_ <
            compare.punctures_ + compare.puncturesTwisted_)
        return true;
    if (punctures_ + puncturesTwisted_ >
            compare.punctures_ + compare.puncturesTwisted_)
        return false;

    // After this, order by a combination of genus and reflectors to
    // group closed spaces with approximately the same complexity.
    if (adjGenus1 + reflectors_ + reflectorsTwisted_ <
            adjGenus2 + compare.reflectors_ + compare.reflectorsTwisted_)
        return true;
    if (adjGenus1 + reflectors_ + reflectorsTwisted_ >
            adjGenus2 + compare.reflectors_ + compare.reflectorsTwisted_)
        return false;

    // Within this genus + reflectors combination, reflectors are worse.
    if (reflectors_ + reflectorsTwisted_ <
            compare.reflectors_ + compare.reflectorsTwisted_)
        return true;
    if (reflectors_ + reflectorsTwisted_ >
            compare.reflectors_ + compare.reflectorsTwisted_)
        return false;

    // If we reach this point, we must have adjGenus1 == adjGenus2.
    // Down to more mundane comparisons.

    // Comparing class will catch orientability also (placing orientable
    // before non-orientable).
    if (class_ < compare.class_)
        return true;
    if (class_ > compare.class_)
        return false;

    if (reflectorsTwisted_ < compare.reflectorsTwisted_)
        return true;
    if (reflectorsTwisted_ > compare.reflectorsTwisted_)
        return false;

    if (puncturesTwisted_ < compare.puncturesTwisted_)
        return true;
    if (puncturesTwisted_ > compare.puncturesTwisted_)
        return false;

    if (nFibres_ < compare.nFibres_)
        return true;
    if (nFibres_ > compare.nFibres_)
        return false;

    if (fibres_ < compare.fibres_)
        return true;
    if (compare.fibres_ < fibres_)
        return false;

    if (b_ < compare.b_)
        return true;
    if (b_ > compare.b_)
        return false;

    // Exactly the same!
    return false;
}

Triangulation<3>* SFSpace::construct() const {
    // Things that we don't deal with just yet.
    if (punctures_ || puncturesTwisted_ || reflectors_ || reflectorsTwisted_)
        return 0;

    // We already know how to construct lens spaces.
    LensSpace* lens = isLensSpace();
    if (lens) {
        Triangulation<3>* t = lens->construct();
        delete lens;
        return t;
    }

    // Currently we work over the 2-sphere only.
    if (genus_ != 0 || class_ != o1)
        return 0;

    // Since we've already dealt with lens spaces, we must have at least
    // three exceptional fibres.  Build a blocked structure.
    Triangulation<3>* ans = new Triangulation<3>();
    Tetrahedron<3> *a, *b, *c;

    // Begin with the first triangular solid torus.
    a = ans->newTetrahedron();
    b = ans->newTetrahedron();
    c = ans->newTetrahedron();
    a->join(1, b, Perm<4>());
    b->join(2, c, Perm<4>());
    c->join(3, a, Perm<4>(1, 2, 3, 0));

    std::list<SFSFibre>::const_iterator fit = fibres_.begin();
    SatAnnulus(a, Perm<4>(1, 0, 2, 3), b, Perm<4>(1, 2, 0, 3)).
        attachLST(ans, fit->alpha, fit->beta);
    fit++;
    SatAnnulus(b, Perm<4>(2, 1, 3, 0), c, Perm<4>(2, 3, 1, 0)).
        attachLST(ans, fit->alpha, fit->beta);
    fit++;

    // Run through the rest of the fibres, one at a time.  Each extra
    // fibre (aside from the third) will require another triangular
    // solid torus.
    Tetrahedron<3>* prevA = a;
    Tetrahedron<3>* prevC = c;

    SFSFibre nextFibre = *fit++;
    while (fit != fibres_.end()) {
        a = ans->newTetrahedron();
        b = ans->newTetrahedron();
        c = ans->newTetrahedron();
        a->join(3, prevA, Perm<4>(2, 3));
        b->join(3, prevC, Perm<4>(0, 2, 3, 1));
        a->join(1, b, Perm<4>());
        b->join(2, c, Perm<4>());
        c->join(3, a, Perm<4>(1, 2, 3, 0));

        SatAnnulus(b, Perm<4>(2, 1, 3, 0), c, Perm<4>(2, 3, 1, 0)).
            attachLST(ans, nextFibre.alpha, nextFibre.beta);

        prevA = a;
        prevC = c;
        nextFibre = *fit++;
    }

    // We have one remaining fibre.  Fill in the final annulus of the
    // last triangular solid torus.
    SatAnnulus(a, Perm<4>(1, 0, 3, 2), c, Perm<4>(2, 3, 0, 1)).attachLST(ans,
        nextFibre.alpha, -(nextFibre.beta + b_ * nextFibre.alpha));

    return ans;
}

AbelianGroup* SFSpace::homology() const {
    if (punctures_ || puncturesTwisted_) {
        // Not just now.
        return 0;
    }

    // Construct the presentation of the fundamental group and
    // abelianise.  The presentation without reflectors is given on
    // p91 of Orlik [1972].  Each reflector gives additional generators
    // y and z, for which y acts as a boundary component and z^2 = fibre.
    AbelianGroup* ans = new AbelianGroup();
    unsigned long nRef = reflectors_ + reflectorsTwisted_;
    bool twisted = fibreReversing();

    if (baseOrientable()) {
        // Orientable base surface.
        // Generators: a_1, b_1, ..., a_g, b_g, q_1, q_2, ..., q_r, h,
        //             y_1, z_1, ..., y_t, z_t (for reflectors)
        // Relations:
        //     q_j^alpha_j h^beta_j = 1
        //     z_j^2 = h
        //     q_1 ... q_r y_1 ... y_t = h^b
        //     h^2 = 1 (if twisted), or h = 1 (if twisted reflectors)
        //
        // We ignore a_i and b_i, and just add extra rank 2g at the end.
        // Generators in the matrix are q_1, ..., q_r, h, z_1, ..., z_t,
        //                              y_1, ..., y_t.
        MatrixInt pres(nFibres_ + nRef + (twisted ? 2 : 1),
            nFibres_ + 1 + 2 * nRef);

        unsigned long which = 0;
        for (FibreIteratorConst it = fibres_.begin(); it != fibres_.end();
                it++) {
            pres.entry(nFibres_ + nRef, which) = 1;

            pres.entry(which, nFibres_) = it->beta;
            pres.entry(which, which) = it->alpha;

            which++;
        }

        unsigned long ref;
        for (ref = 0; ref < nRef; ref++) {
            pres.entry(nFibres_ + ref, nFibres_) = -1;
            pres.entry(nFibres_ + ref, nFibres_ + 1 + ref) = 2;
            pres.entry(nFibres_ + nRef, nFibres_ + 1 + nRef + ref) = 1;
        }

        pres.entry(nFibres_ + nRef, nFibres_) = -b_;

        if (reflectorsTwisted_)
            pres.entry(nFibres_ + nRef + 1, nFibres_) = 1;
        else if (twisted)
            pres.entry(nFibres_ + nRef + 1, nFibres_) = 2;

        ans->addGroup(pres);
        ans->addRank(2 * genus_);
    } else {
        // Non-orientable base surface.
        // Generators: v_1, v_2, ..., v_g, q_1, q_2, ..., q_r, h,
        //             y_1, z_1, ..., y_t, z_t (for reflectors)
        // Relations:
        //     q_j^alpha_j h^beta_j = 1
        //     z_j^2 = h
        //     q_1 ... q_r v_1^2 ... v_g^2 y_1 ... y_t = h^b
        //     h^2 = 1 (if twisted), or h = 1 (if twisted reflectors)
        //
        // Generators in the matrix are q_1, ..., q_r, v_1, ..., v_g, h,
        //                              z_1, ..., z_t, y_1, ..., y_t.
        MatrixInt pres(nFibres_ + nRef + (twisted ? 2 : 1),
            nFibres_ + genus_ + 1 + 2 * nRef);

        unsigned long which = 0;
        for (FibreIteratorConst it = fibres_.begin(); it != fibres_.end();
                it++) {
            pres.entry(nFibres_ + nRef, which) = 1;

            pres.entry(which, nFibres_ + genus_) = it->beta;
            pres.entry(which, which) = it->alpha;

            which++;
        }

        unsigned long ref;
        for (ref = 0; ref < nRef; ref++) {
            pres.entry(nFibres_ + ref, nFibres_ + genus_) = -1;
            pres.entry(nFibres_ + ref, nFibres_ + genus_ + 1 + ref) = 2;
            pres.entry(nFibres_ + nRef, nFibres_ + genus_ + 1 + nRef + ref) = 1;
        }

        for (which = 0; which < genus_; which++)
            pres.entry(nFibres_ + nRef, nFibres_ + which) = 2;
        pres.entry(nFibres_ + nRef, nFibres_ + genus_) = -b_;

        if (reflectorsTwisted_)
            pres.entry(nFibres_ + nRef + 1, nFibres_ + genus_) = 1;
        else if (twisted)
            pres.entry(nFibres_ + nRef + 1, nFibres_ + genus_) = 2;

        ans->addGroup(pres);
    }

    return ans;
}

void SFSpace::writeBaseExtraCount(std::ostream& out, unsigned long count,
        const char* object, bool tex) {
    out << " + " << count << (tex ? "\\ \\mbox{" : " ") << object;
    if (count != 1)
        out << 's';
    if (tex)
        out << '}';
}

std::ostream& SFSpace::writeCommonBase(std::ostream& out, bool tex) const {
    bool named = false;

    // IMPORTANT: We do not allow spaces with > 2 reflector boundary
    // components to be named.  Otherwise this messes up the reflector
    // boundary output.
    unsigned long totRef = reflectors_ + reflectorsTwisted_;
    unsigned long totBdries = totRef + punctures_ + puncturesTwisted_;

    if (baseOrientable()) {
        // Orientable base surface.
        if (genus_ == 0 && totBdries == 0) {
            out << (tex ? "S^2" : "S2");
            named = true;
        } else if (genus_ == 0 && totBdries == 1) {
            if (totRef && tex)
                out << "\\overline{";

            out << 'D';

            if (totRef)
                out << (tex ? '}' : '_');

            named = true;
        } else if (genus_ == 0 && totBdries == 2) {
            if (totRef == 1 && tex)
                out << "\\overline{";
            else if (totRef == 2 && tex)
                out << "\\overline{\\overline{";

            out << 'A';

            if (totRef == 1)
                out << (tex ? '}' : '_');
            else if (totRef == 2)
                out << (tex ? "}}" : "=");

            named = true;
        } else if (genus_ == 1 && totBdries == 0) {
            out << (tex ? "T^2" : "T");
            named = true;
        }
    } else {
        // Non-orientable base surface.
        if (genus_ == 1 && totBdries == 0) {
            out << (tex ? "\\mathbb{R}P^2" : "RP2");
            named = true;
        } else if (genus_ == 1 && totBdries == 1) {
            if (totRef && tex)
                out << "\\overline{";

            out << 'M';

            if (totRef)
                out << (tex ? '}' : '_');

            named = true;
        } else if (genus_ == 2 && totBdries == 0) {
            out << (tex ? "K^2" : "KB");
            named = true;
        }
    }

    if (! named) {
        if (baseOrientable())
            out << (tex ? "\\mathrm{Or},\\ " : "Or, ")
                << "g=" << genus_;
        else
            out << (tex ? "\\mathrm{Non-or},\\ " : "Non-or, ")
                << "g=" << genus_;

        if (punctures_)
            writeBaseExtraCount(out, punctures_, "puncture", tex);
        if (puncturesTwisted_)
            writeBaseExtraCount(out, puncturesTwisted_,
                "twisted puncture", tex);
        if (reflectors_)
            writeBaseExtraCount(out, reflectors_, "reflector", tex);
        if (reflectorsTwisted_)
            writeBaseExtraCount(out, reflectorsTwisted_,
                "twisted reflector", tex);
    }

    if (class_ == o2 || class_ == bo2)
        out << (tex ? "/o_2" : "/o2");
    else if (class_ == n2 || class_ == bn2)
        out << (tex ? "/n_2" : "/n2");
    else if (class_ == n3 || class_ == bn3)
        out << (tex ? "/n_3" : "/n3");
    else if (class_ == n4)
        out << (tex ? "/n_4" : "/n4");

    return out;
}

std::ostream& SFSpace::writeCommonStructure(std::ostream& out, bool tex)
        const {
    if (b_ == 0 && fibres_.empty()) {
        // We have a straightforward product (possibly twisted).
        writeCommonBase(out, tex);

        // The o1/o2/n1/n2/etc specification has already been written in
        // writeCommonBase().  Just do the pretty x S1 and get out.
        if (fibreReversing())
            return out << (tex ? " \\twisted S^1" : " x~ S1");
        else
            return out << (tex ? " \\times S^1" : " x S1");
    }

    // We have at least one fibre, even if it's only (1,b).

    out << (tex ? "\\mathrm{SFS}\\left(" : "SFS [");

    writeCommonBase(out, tex);

    out << ':';
    if (fibres_.empty()) {
        // We have b non-zero.
        out << ' ' << SFSFibre(1, b_);
    } else {
        out << ' ';
        copy(fibres_.begin(), --fibres_.end(),
            std::ostream_iterator<SFSFibre>(out, " "));

        SFSFibre final = fibres_.back();
        final.beta += final.alpha * b_;
        out << final;
    }

    return out << (tex ? "\\right)" : "]");
}

std::ostream& SFSpace::writeCommonName(std::ostream& out, bool tex) const {
    // Things we don't deal with just yet.
    if (fibreNegating())
        return writeStructure(out);
    if (reflectors_ || reflectorsTwisted_ || punctures_ || puncturesTwisted_)
        return writeStructure(out);

    // We're looking at an orientable SFS (with either orientable or
    // non-orientable base orbifold), where the base orbifold has no
    // punctures or reflector boundaries.

    // Take out the lens spaces first.
    LensSpace* lens = isLensSpace();
    if (lens) {
        if (tex)
            lens->writeTeXName(out);
        else
            lens->writeName(out);
        delete lens;
        return out;
    }

    // Pull off the number of fibres we're capable of dealing with.
    // At this moment this is four.
    if (nFibres_ > 4)
        return writeStructure(out);

    SFSFibre fibre[4];
    std::copy(fibres_.begin(), fibres_.end(), fibre);

    // Note that with three fibres our reduced form will always have
    // b >= -1.

    // TODO: The four non-orientable flat manifolds are on Orlik p140.

    // SFS over the 2-sphere:
    if (genus_ == 0 && class_ == o1) {
        if (nFibres_ == 4 && fibre[0] == two && fibre[1] == two &&
                fibre[2] == two && fibre[3] == two && b_ == -2) {
            // [ S2 : (2,1), (2,1), (2,-1), (2,-1) ]
            // Orlik, p138, case M2.
            return out << (tex ? "K^2/n2 \\twisted S^1" : "KB/n2 x~ S1");
        } else if (nFibres_ == 3 && fibre[0] == two &&
                gcd(fibre[2].alpha, fibre[2].beta) == 1 && b_ >= -1) {
            // [ S2 : (2,1), (...), (...) ]

            if (fibre[1] == two) {
                // [ S2 : (2,1), (2,1), (a,b) ].
                // Orlik, p112, case (ii).

                long a = fibre[2].alpha;
                long m = fibre[2].beta + a * (b_ + 1);

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
                    long m = 6 * b_ + 3 + 2 * (fibre[1].beta + fibre[2].beta);
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
                    long m = 12 * b_ + 6 + 4 * fibre[1].beta +
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
                    long m = 30 * b_ + 15 + 10 * fibre[1].beta +
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
                        fibre[2].beta == 1 && b_ == -1) {
                    // [ S2 : (2,1), (3,1), (6,-5) ].
                    // Orlik, p138, case M5.
                    if (tex)
                        return out << "T^2 \\times I / \\homtwo{1}{1}{-1}{0}";
                    else
                        return out << "T x I / [ 1,1 | -1,0 ]";
                }
            } else if (fibre[1] == four && fibre[2] == four && b_ == -1) {
                // [ S2 : (2,1), (4,1), (4,-3) ].
                // Orlik, p138, case M4.
                if (tex)
                    return out << "T^2 \\times I / \\homtwo{0}{-1}{1}{0}";
                else
                    return out << "T x I / [ 0,1 | -1,0 ]";
            }
        } else if (nFibres_ == 3 && fibre[0] == three && fibre[1] == three
                && fibre[2] == three && b_ == -1) {
            // [ S2 : (3,1), (3,1), (3,-2) ]
            // Orlik, p138, case M3.
            if (tex)
                return out << "T^2 \\times I / \\homtwo{0}{-1}{1}{-1}";
            else
                return out << "T x I / [ -1,1 | -1,0 ]";
        }
    }

    // SFS over the real projective plane:
    if (genus_ == 1 && class_ == n2) {
        if (nFibres_ == 0) {
            // No exceptional fibres.
            if (b_ == 0) {
                // [ RP2 ]
                // Orlik, p113, remark.
                return out << (tex ? "\\mathbb{R}P^3 \\# \\mathbb{R}P^3" :
                    "RP3 # RP3");
            } else {
                // TODO: [ RP2 : (1,b) ]
                // Is this Orlik, p112, case (vi)?  What is this?
                // ans << "S3/Q" << (4 * (b > 0 ? b : -b));
            }
        } else if (nFibres_ == 1 && fibre[0].alpha > 1) {
            // Just one exceptional fibre.
            long a = fibre[0].alpha;
            long n = b_ * a + fibre[0].beta;
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
