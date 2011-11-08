
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

#include "enumerate/nhilbertdual.h"
#include <iostream>
#include <list>

namespace regina {

bool NHilbertDual::reduces(const VecSpec& vec, const VecSpecList& against,
        int listSign, VecSpecList::const_iterator ignore) {
    for (VecSpecList::const_iterator it = against.begin(); it != against.end();
            ++it) {
        if (it == ignore)
            continue;

        if (! (**it <= vec))
            continue;

        if (listSign > 0) {
            if ((**it).nextHyp() <= vec.nextHyp())
                return true;
        } else if (listSign < 0) {
            if (vec.nextHyp() <= (**it).nextHyp())
                return true;
        } else {
            if (vec.nextHyp() == (**it).nextHyp())
                return true;
        }
    }

    return false;
}

void NHilbertDual::reduceBasis(VecSpecList& reduce, VecSpecList& against,
        int listSign) {
    VecSpecList::iterator i, next;

    i = reduce.begin();
    while (i != reduce.end()) {
        next = i;
        ++next;

        // See if **i is redundant.
        if (reduces(**i, against, listSign, i)) {
            delete *i;
            reduce.erase(i);
        }

        i = next;
    }
}

void NHilbertDual::intersectHyperplane(VecSpecVector& list,
        const NMatrixInt& subspace, unsigned row,
        const NEnumConstraintList* constraints) {
    // These must be linked lists because we need fast insertion and
    // deletion at arbitrary locations.
    VecSpecList zero, pos, neg, newZero, newPos, newNeg;
    VecSpecList::iterator it, posit, negit;
    VecSpecList::iterator posPrevGen, negPrevGen;

    // Decant the existing basis elements into 0/+/- sets according to the
    // new hyperplane.
    int s;
    for (VecSpecVector::iterator srcit = list.begin(); srcit != list.end();
            srcit++) {
        (*srcit)->initNextHyp(subspace, row);

        s = (*srcit)->sign();
        if (s == 0)
            zero.push_back(*srcit);
        else if (s < 0)
            neg.push_back(*srcit);
        else
            pos.push_back(*srcit);
    }
    list.clear();

    posPrevGen = pos.begin();
    negPrevGen = neg.begin();

    // TODO: Optimise from here down: (d), Sec.3

    // Keep enlarging these sets until they enlarge no more.
    NEnumConstraintList::const_iterator cit;
    std::set<unsigned>::const_iterator coordit;
    unsigned nonZero;
    bool broken;
    bool reachedPosPrevGen;
    VecSpec sum(subspace.columns());
#if 1
    std::cout << "Start iteration:" << std::endl;
#endif
    while (true) {
#if 1
        std::cout << "    Intermediate 0/+/-: " << zero.size()
            << ' ' << pos.size() << ' ' << neg.size() << std::endl;
#endif

        // Generate all valid (pos + neg) pairs that cannot be reduced using
        // the present lists.
        reachedPosPrevGen = false;
        for (posit = pos.begin(); posit != pos.end(); ++posit) {
            if (posit == posPrevGen)
                reachedPosPrevGen = true;

            for (negit = (reachedPosPrevGen ? neg.begin() : negPrevGen);
                    negit != neg.end(); ++negit) {
                // Check for validity.
                if (constraints && ! constraints->empty()) {
                    // TODO: Optimise using bitmasks.
                    broken = false;
                    for (cit = constraints->begin(); cit != constraints->end();
                            ++cit) {
                        nonZero = 0;
                        for (coordit = cit->begin(); coordit != cit->end();
                                ++coordit) {
                            if ((**posit)[*coordit] != NLargeInteger::zero ||
                                    (**negit)[*coordit] != NLargeInteger::zero){
                                if (++nonZero > 1) {
                                    broken = true;
                                    break;
                                }
                            }
                        }
                        if (broken)
                            break;
                    }
                    if (broken)
                        continue;
                }

                // Check whether the vector can be reduced; if not, use it.
                sum.formSum(**posit, **negit);
                s = sum.sign();
                if (s == 0) {
                    if (! reduces(sum, zero, 0, zero.end()))
                        newZero.push_back(new VecSpec(sum));
                } else if (s > 0) {
                    // If this decomposes as a sum of (possibly many)
                    // terms in pos and/or zero, at least one such term must
                    // be in pos.  Therefore we only need to test
                    // reduction against pos, and not zero also.
                    if (! reduces(sum, pos, 1, pos.end()))
                        newPos.push_back(new VecSpec(sum));
                } else if (s < 0) {
                    // Likewise: test only against neg, and not zero also.
                    if (! reduces(sum, neg, -1, neg.end()))
                        newNeg.push_back(new VecSpec(sum));
                }
            }
        }

        if (newZero.empty() && newPos.empty() && newNeg.empty()) {
            // The basis has not changed, which means we're done.
            break;
        }

        // Independently reduce the basis on each side.
        reduceBasis(newZero, newZero, 0);
        reduceBasis(zero, newZero, 0);

        reduceBasis(newPos, newPos, 1);
        reduceBasis(pos, newPos, 1);

        reduceBasis(newNeg, newNeg, -1);
        reduceBasis(neg, newNeg, -1);

        // Add the new vectors to the bases.
        zero.splice(zero.end(), newZero);

        if (newPos.empty()) {
            posPrevGen = pos.end();
        } else {
            posPrevGen = newPos.begin();
            pos.splice(pos.end(), newPos);
        }

        if (newNeg.empty()) {
            negPrevGen = neg.end();
        } else {
            negPrevGen = newNeg.begin();
            neg.splice(neg.end(), newNeg);
        }
    }

    // We have a final Hilbert basis!
    // Clean up and return.
    for (it = pos.begin(); it != pos.end(); ++it)
        delete *it;
    for (it = neg.begin(); it != neg.end(); ++it)
        delete *it;
    for (it = zero.begin(); it != zero.end(); ++it)
        list.push_back(*it);
}

} // namespace regina

