
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "subcomplex/nsfs.h"
#include "subcomplex/nlensspace.h"

ostream& operator << (ostream& out, const NExceptionalFibre& f) {
    return (out << '(' << f.alpha << ", " << f.beta << ')');
}

bool NExceptionalFibre::operator < (const NExceptionalFibre& compare) const {
    if (alpha == 1)
        return false;
    if (compare.alpha == 1)
        return true;
    if (alpha < compare.alpha)
        return true;
    if (compare.alpha < alpha)
        return false;
    if (compare.alpha == 0)
        return (beta < compare.beta);
    long b1 = beta % alpha;
    if (b1 < 0)
        b1 += alpha;
    long b2 = compare.beta % compare.alpha;
    if (b2 < 0)
        b2 += compare.alpha;
    return (b1 < b2);
}

NSFS::NSFS(const NSFS& cloneMe) : orbitGenus(cloneMe.orbitGenus),
        orbitOrientable(cloneMe.orbitOrientable),
        orbitPunctures(cloneMe.orbitPunctures), k(cloneMe.k),
        nNonZeroFibres(cloneMe.nNonZeroFibres) {
    for (FibreIterator it(cloneMe.fibres); ! it.done(); it++)
        fibres.addLast(*it);
}

void NSFS::operator = (const NSFS& cloneMe) {
    orbitGenus = cloneMe.orbitGenus;
    orbitOrientable = cloneMe.orbitOrientable;
    orbitPunctures = cloneMe.orbitPunctures;

    fibres.flush();
    for (FibreIterator it(cloneMe.fibres); ! it.done(); it++)
        fibres.addLast(*it);

    k = cloneMe.k;
    nNonZeroFibres = cloneMe.nNonZeroFibres;
}

unsigned long NSFS::getFibreCount() const {
    unsigned long size = fibres.size();
    if (size == 0)
        return (k == 0 ? 0 : 1);
    else
        return size;
}

NExceptionalFibre NSFS::getFibre(unsigned long which) const {
    unsigned long size = fibres.size();
    if (size == 0)
        return NExceptionalFibre(1, k);
    if (which == size - 1)
        return getModifiedFinalFibre();
    return fibres[which];
}

void NSFS::insertFibre(const NExceptionalFibre& fibre) {
    // We are assuming that the parameters of this fibre are coprime and
    // that alpha is non-negative.

    if (fibre.alpha == 0) {
        fibres.addFirst(NExceptionalFibre(0, 1));
        return;
    }
    if (fibre.alpha == 1) {
        k += fibre.beta;
        return;
    }

    // Put the fibre in standard form.
    long a = fibre.alpha;
    long b = fibre.beta;
    k += (b / a);
    b = b % a;
    if (b < 0) {
        b += a;
        k--;
    }

    // Now we have 0 <= b < a and a >= 2.
    if (b != 0)
        nNonZeroFibres++;
    fibres.addSort(NExceptionalFibre(a, b));

    // We're done!
}

void NSFS::reduce() {
    // Decide whether all the fibres should be negated.
    if (k < (-k - nNonZeroFibres)) {
        // Negate all the fibres.
        k = -k;

        FibreIterator it, it2, next;
        for (it.init(fibres); ! it.done(); it++)
            if ((*it).alpha > 0 && (*it).beta > 0) {
                (*it).beta = (*it).alpha - (*it).beta;
                k--;
            }

        // Resort the array.
        // Each portion of the array with fixed index must be reversed.
        NExceptionalFibre tmpFibre;
        it.init(fibres);
        while (! it.done()) {
            // INV: it points to the next block to be reversed.
            it2 = it;
            for (it2++; (! it2.done()) && (*it2).alpha == (*it).alpha; it2++)
                ;

            // Now it2 points to the first element of the following block.
            next = it2;
            if (it2.done())
                it2.initEnd(fibres);
            else
                it2--;

            // Now it2 points to the last element of this block.
            // Reverse this block by swapping elements at each end and
            // working towards the centre.
            while (it.getArrayIndex() < it2.getArrayIndex()) {
                tmpFibre = (*it);
                (*it) = (*it2);
                (*it2) = tmpFibre;
                it++;
                it2--;
            }

            // Move on to the next block.
            it = next;
        }
    }
}

NLensSpace* NSFS::isLensSpace() const {
    if (orbitGenus == 0) {
        // Orbit manifold is the sphere.
        if (fibres.size() == 0)
            return new NLensSpace(k >= 0 ? k : -k, 1);
        if (fibres.size() > 2)
            return 0;
        long p, q;
        // We will write this space as SFS(S2, (q,p)).
        if (fibres.size() == 1) {
            q = fibres[0].alpha;
            p = fibres[0].beta + (k * q);
        } else {
            // Precisely two fibres.

            // Check whether we have a (0,1) fibre, in which case there are
            // no guarantees.
            if (fibres[0].alpha == 0)
                return 0;

            q = fibres[1].alpha;
            p = fibres[1].beta + (k * q);
            long a = fibres[0].alpha;
            long b = fibres[0].beta;

            // INV: We have space (a,b) (q,p) with 0 <= b < a.
            while (b > 0) {
                a = a - b;
                q = q + p;
                if (b >= a) {
                    p += (q * (b / a));
                    b = b % a;
                }
            }
            // We should now have (a,b) == (1,0).
        }
        return new NLensSpace(p >= 0 ? p : -p, q >= 0 ? q : -q);
    } else {
        // TODO: Recognise Lens space over RP2.
        return 0;
    }
}

void NSFS::writeTextShort(ostream& out) const {
    out << "SFS [";
    if (orbitOrientable) {
        if (orbitGenus == 0)
            out << "S2";
        else if (orbitGenus == 1)
            out << "T";
        else
            out << "Or, g=" << orbitGenus;
    } else {
        if (orbitGenus == 1)
            out << "RP2";
        else if (orbitGenus == 2)
            out << "KB";
        else
            out << "Non-or, g=" << orbitGenus;
    }
    if (orbitPunctures == 1)
        out << " + " << orbitPunctures << " puncture";
    else if (orbitPunctures > 1)
        out << " + " << orbitPunctures << " punctures";

    out << ':';
    if (fibres.size() == 0) {
        if (k == 0)
            out << " no fibres";
        else
            out << ' ' << NExceptionalFibre(1, k);
    } else {
        unsigned long nFibres = fibres.size();
        for (unsigned long i = 0; i < nFibres; i++)
            if (i < nFibres - 1)
                out << ' ' << fibres[i];
            else {
                out << ' ' << getModifiedFinalFibre();
            }
    }
    out << ']';
}

NExceptionalFibre NSFS::getModifiedFinalFibre() const {
    if (fibres.size() == 0)
        return NExceptionalFibre(1, k);
    NExceptionalFibre ans(fibres[fibres.size() - 1]);
    ans.beta += (k * ans.alpha);
    return ans;
}
