
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <algorithm>
#include <iterator>
#include <sstream>
#include "algebra/nabeliangroup.h"
#include "maths/nmatrixint.h"
#include "maths/numbertheory.h"
#include "subcomplex/nsfs.h"
#include "subcomplex/nlensspace.h"

namespace regina {

typedef std::list<NExceptionalFibre>::iterator FibreIterator;
typedef std::list<NExceptionalFibre>::const_iterator FibreIteratorConst;

std::ostream& operator << (std::ostream& out, const NExceptionalFibre& f) {
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

void NSFS::operator = (const NSFS& cloneMe) {
    orbitGenus = cloneMe.orbitGenus;
    orbitOrientable = cloneMe.orbitOrientable;
    orbitPunctures = cloneMe.orbitPunctures;
    fibres = cloneMe.fibres;
    k = cloneMe.k;
    nNonZeroFibres = cloneMe.nNonZeroFibres;
}

unsigned long NSFS::getFibreCount() const {
    if (fibres.empty())
        return (k == 0 ? 0 : 1);
    else
        return fibres.size();
}

NExceptionalFibre NSFS::getFibre(unsigned long which) const {
    // Avoid calling fibres.size() which takes linear time.

    // Do we have no fibres?
    if (fibres.empty())
        return NExceptionalFibre(1, k);

    // Locate the fibre in question.
    FibreIteratorConst pos = fibres.begin();
    advance(pos, which);

    // Is this the final fibre?
    FibreIteratorConst next = pos;
    next++;
    if (next == fibres.end())
        return getModifiedFinalFibre();

    // Return the fibre we found.
    return *pos;
}

void NSFS::insertFibre(const NExceptionalFibre& fibre) {
    // We are assuming that the parameters of this fibre are coprime and
    // that alpha is non-negative.

    if (fibre.alpha == 0) {
        fibres.push_front(NExceptionalFibre(0, 1));
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
    NExceptionalFibre f(a, b);
    fibres.insert(lower_bound(fibres.begin(), fibres.end(), f), f);

    // We're done!
}

void NSFS::reduce() {
    // Decide whether all the fibres should be negated.
    if (k < (-k - nNonZeroFibres)) {
        // Negate all the fibres.
        k = -k;

        FibreIterator it, it2, next;
        for (it = fibres.begin(); it != fibres.end(); it++)
            if ((*it).alpha > 0 && (*it).beta > 0) {
                (*it).beta = (*it).alpha - (*it).beta;
                k--;
            }

        // Resort the array.
        // Each portion of the array with fixed index must be reversed.
        NExceptionalFibre tmpFibre;
        it = fibres.begin();
        while (it != fibres.end()) {
            // INV: it points to the next block to be reversed.
            it2 = it;
            for (it2++; it2 != fibres.end() && (*it2).alpha == (*it).alpha;
                    it2++)
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
}

NLensSpace* NSFS::isLensSpace() const {
    if (orbitGenus == 0) {
        // Orbit manifold is the sphere.
        if (fibres.empty())
            return new NLensSpace(k >= 0 ? k : -k, 1);
        unsigned long size = fibres.size(); // NOTE: size() takes linear time!
        if (size > 2)
            return 0;
        long p, q;
        // We will write this space as SFS(S2, (q,p)).
        if (size == 1) {
            q = fibres.front().alpha;
            p = fibres.front().beta + (k * q);
        } else {
            // Precisely two fibres.

            // Check whether we have a (0,1) fibre, in which case there are
            // no guarantees.
            if (fibres.front().alpha == 0)
                return 0;

            q = fibres.back().alpha;
            p = fibres.back().beta + (k * q);
            long a = fibres.front().alpha;
            long b = fibres.front().beta;

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

std::string NSFS::getCommonName() const {
    // Lens spaces.
    NLensSpace* lens = isLensSpace();
    if (lens) {
        std::string ans = lens->getCommonName();
        delete lens;
        return ans;
    }

    // Pull off the number of fibres we're capable of dealing with.
    // At this moment this is three.
    NExceptionalFibre fibre[3];
    unsigned nFibres = 0;
    FibreIteratorConst it = fibres.begin();
    while (nFibres < 3) {
        if (it == fibres.end())
            break;
        fibre[nFibres++] = *it++;
    }

    // Are there too many fibres?
    if (it != fibres.end())
        return 0;

    // SFS over the 2-sphere:
    if (orbitGenus == 0 && orbitOrientable && orbitPunctures == 0) {
        // [ S2 : (2,-1), (2,1), (n,b) ] for n,b coprime and positive.
        NExceptionalFibre two(2, 1);
        NExceptionalFibre three(3, 1);
        if (nFibres == 3 && fibre[0] == two && fibre[1] == two &&
                gcd(fibre[2].alpha, fibre[2].beta) == 1 && k >= -1) {
            // We can name this.
            // Note that n > 1 in this case.
            long n = fibre[2].alpha;
            long b = fibre[2].beta + n * (k + 1);

            if (b % 2 == 1) {
                // S3/Q{4n} x Z{b} (see Matveev).
                std::ostringstream ans;
                ans << "S3/Q" << (n * 4);
                if (b > 1)
                    ans << "xZ" << b;
                return ans.str();
            } else {
                // S3/D{2^{k+2}n} x Z{2m+1} where b=2^k(2m+1) (see Matveev).
                long odd = b;
                long twos = 1;
                while (! (odd & 1)) {
                    odd >>= 1;
                    twos <<= 1;
                }

                std::ostringstream ans;
                ans << "S3/D" << ((twos << 2) * n);
                if (odd > 1)
                    ans << "xZ" << odd;
                return ans.str();
            }
        }

        else if (nFibres == 3 && fibre[0] == two && fibre[1] == three &&
                gcd(fibre[2].alpha, fibre[2].beta) == 1 && k >= -1) {
            // We might still be able to name this.
            // Note that n > 2 and b > 0 in this case.
            long n = fibre[2].alpha;
            long b = fibre[2].beta + n * (k + 1);

            if (n == 3) {
                /**
                 * The formula given by Matveev seems to be wrong in this
                 * case.  We'll leave it until there's time to sit down
                 * and work out what it should have been.
                 */
                /*
                // S3/P'{8(3^{k+1})}xZ{2m+1} where (3^k)m=2b-1 (see Matveev)
                if (b == 1)
                    return "S3/P24";

                long m = 2 * b - 1; // Known: m >= 1.
                long threes = 1;
                while (m % 3 == 0) {
                    m = m / 3;
                    threes *= 3;
                }

                std::ostringstream ans;
                ans << "S3/P'" << (threes * 24) << "xZ" << (2 * m + 1);
                return ans.str();
                */
            } else if (n == 4) {
                // S3/P48xZ{3b-2} (see Matveev).
                std::ostringstream ans;
                ans << "S3/P48";
                if (b > 1)
                    ans << "xZ" << (b * 3 - 2);
                return ans.str();
            } else if (n == 5) {
                // S3/P120xZ{6b-5} (see Matveev).
                std::ostringstream ans;
                ans << "S3/P120";
                if (b > 1)
                    ans << "xZ" << (b * 6 - 5);
                return ans.str();
            }
        }
    }

    return toString();
}

void NSFS::writeTextShort(std::ostream& out) const {
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
    if (fibres.empty()) {
        if (k == 0)
            out << " no fibres";
        else
            out << ' ' << NExceptionalFibre(1, k);
    } else {
        out << ' ';
        copy(fibres.begin(), --fibres.end(),
            std::ostream_iterator<NExceptionalFibre>(out, " "));
        out << getModifiedFinalFibre();
    }
    out << ']';
}

NExceptionalFibre NSFS::getModifiedFinalFibre() const {
    if (fibres.empty())
        return NExceptionalFibre(1, k);
    NExceptionalFibre ans(fibres.back());
    ans.beta += (k * ans.alpha);
    return ans;
}

NAbelianGroup* NSFS::getHomologyH1() const {
    NAbelianGroup* ans = new NAbelianGroup();
    unsigned long nFibres = fibres.size();

    if (orbitOrientable) {
        NMatrixInt pres(nFibres + 1, nFibres + orbitPunctures + 1);

        unsigned long which = 0;
        for (FibreIteratorConst it = fibres.begin(); it != fibres.end(); it++) {
            pres.entry(0, which + 1) = 1;

            pres.entry(which + 1, 0) = -(*it).beta;
            pres.entry(which + 1, which + 1) = (*it).alpha;

            which++;
        }

        for (which = 0; which < orbitPunctures; which++)
            pres.entry(0, nFibres + 1 + which) = 1;

        pres.entry(0, 0) = k;

        ans->addGroup(pres);
        ans->addRank(2 * orbitGenus);
    } else {
        NMatrixInt pres(nFibres + 2, orbitGenus + nFibres + orbitPunctures + 1);

        unsigned long which = 0;
        for (FibreIteratorConst it = fibres.begin(); it != fibres.end(); it++) {
            pres.entry(0, which + 1) = 1;

            pres.entry(which + 1, 0) = -(*it).beta;
            pres.entry(which + 1, which + 1) = (*it).alpha;

            which++;
        }

        for (which = 0; which < orbitPunctures; which++)
            pres.entry(0, nFibres + 1 + which) = 1;
        for (which = 0; which < orbitGenus; which++)
            pres.entry(0, nFibres + orbitPunctures + 1 + which) = 2;
        pres.entry(0, 0) = k;

        pres.entry(nFibres + 1, 0) = 2;

        ans->addGroup(pres);
    }
    
    return ans;
}

} // namespace regina
