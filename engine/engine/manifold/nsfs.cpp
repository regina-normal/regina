
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
#include "manifold/nlensspace.h"
#include "manifold/nsfs.h"
#include "maths/nmatrixint.h"
#include "maths/numbertheory.h"
#include "triangulation/ntriangulation.h"
#include "utilities/boostutils.h"

namespace regina {

typedef std::list<NExceptionalFibre>::iterator FibreIterator;
typedef std::list<NExceptionalFibre>::const_iterator FibreIteratorConst;

std::ostream& operator << (std::ostream& out, const NExceptionalFibre& f) {
    return (out << '(' << f.alpha << ',' << f.beta << ')');
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
    nFibres = cloneMe.nFibres;
}

unsigned long NSFS::getFibreCount() const {
    if (fibres.empty())
        return (k == 0 ? 0 : 1);
    else
        return nFibres;
}

NExceptionalFibre NSFS::getFibre(unsigned long which) const {
    // Do we have no fibres?
    if (fibres.empty())
        return NExceptionalFibre(1, k);

    // Locate the fibre in question.
    FibreIteratorConst pos = fibres.begin();
    advance(pos, which);

    // Is this the final fibre?
    if (regina::boost::next(pos) == fibres.end())
        return getModifiedFinalFibre();

    // Return the fibre we found.
    return *pos;
}

void NSFS::insertFibre(long alpha, long beta) {
    // We are assuming that the parameters of this fibre are coprime and
    // that alpha is strictly positive.

    if (alpha == 0) {
        // TODO: We should probably throw an exception here or something.
        std::cerr << "ERROR: Inserting illegal fibre (0," << beta <<
            ")." << std::endl;
        return;
    }
    if (alpha == 1) {
        k += beta;
        return;
    }

    // Put the fibre in standard form.
    k += (beta / alpha);
    beta = beta % alpha;
    if (beta < 0) {
        beta += alpha;
        k--;
    }

    // Now we have 0 <= beta < alpha and alpha >= 2.
    nFibres++;
    NExceptionalFibre f(alpha, beta);
    fibres.insert(lower_bound(fibres.begin(), fibres.end(), f), f);

    // We're done!
}

void NSFS::reduce() {
    // Decide whether all the fibres should be negated.
    if (k < (-k - static_cast<long>(nFibres))) {
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
    if (orbitGenus == 0 && orbitOrientable && orbitPunctures == 0) {
        // Orbit manifold is the sphere.
        if (fibres.empty())
            return new NLensSpace(k >= 0 ? k : -k, 1);
        else if (nFibres == 1) {
            long q = fibres.front().alpha;
            long p = fibres.front().beta + (k * q);

            // We have SFS [S2 : (q,p)].
            return new NLensSpace(p >= 0 ? p : -p, q >= 0 ? q : -q);
        } else if (nFibres == 2) {
            // Precisely two fibres.
            long q = fibres.back().alpha;
            long p = fibres.back().beta + (k * q);
            long a = fibres.front().alpha;
            long b = fibres.front().beta;

            // INV: We have SFS [S2 : (a,b) (q,p)] with 0 <= b < a.
            while (b > 0) {
                a = a - b;
                q = q + p;
                if (b >= a) {
                    p += (q * (b / a));
                    b = b % a;
                }
            }

            // We should now have (a,b) == (1,0).
            return new NLensSpace(p >= 0 ? p : -p, q >= 0 ? q : -q);
        }

        // Not a lens space.
        return 0;
    } else if (orbitGenus == 1 && (! orbitOrientable) && orbitPunctures == 0) {
        // Orbit manifold is the projective plane.
        if (nFibres == 1) {
            // We have precisely one exceptional fibre.
            long a = fibres.front().alpha;
            long n = k * a + fibres.front().beta;

            if (n == 1 || n == -1)
                return new NLensSpace(4 * a, 2 * a - 1);
        }

        // Not a lens space.
        return 0;
    }

    return 0;
}

NTriangulation* NSFS::construct() const {
    // We already know how to construct lens spaces.
    NLensSpace* lens = isLensSpace();
    if (lens) {
        NTriangulation* t = lens->construct();
        delete lens;
        return t;
    }

    return 0;
}

NAbelianGroup* NSFS::getHomologyH1() const {
    NAbelianGroup* ans = new NAbelianGroup();

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

std::ostream& NSFS::writeName(std::ostream& out) const {
    return writeCommonName(out, false);
}

std::ostream& NSFS::writeTeXName(std::ostream& out) const {
    return writeCommonName(out, true);
}

std::ostream& NSFS::writeStructure(std::ostream& out) const {
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
    return out << ']';
}

NExceptionalFibre NSFS::getModifiedFinalFibre() const {
    if (fibres.empty())
        return NExceptionalFibre(1, k);
    NExceptionalFibre ans(fibres.back());
    ans.beta += (k * ans.alpha);
    return ans;
}

std::ostream& NSFS::writeCommonName(std::ostream& out, bool tex) const {
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

    // Pull off the number of fibres we're capable of dealing with.
    // At this moment this is four.
    if (nFibres > 4)
        return writeStructure(out);

    NExceptionalFibre fibre[4];
    std::copy(fibres.begin(), fibres.end(), fibre);

    // Note that with three fibres our reduced form will always have
    // k >= -1.

    // SFS over the 2-sphere:
    if (orbitGenus == 0 && orbitOrientable && orbitPunctures == 0) {
        NExceptionalFibre two(2, 1);
        NExceptionalFibre three(3, 1);
        NExceptionalFibre threeB(3, 2);

        if (nFibres == 4 && fibre[0] == two && fibre[1] == two &&
                fibre[2] == two && fibre[3] == two && k == -2) {
            // [ S2 : (2,1), (2,1), (2,-1), (2,-1) ]
            return (tex ? out << "$K^2 \\twisted S^1$" : out << "KB x~ S1");
        } else if (nFibres == 3 && fibre[0] == two &&
                gcd(fibre[2].alpha, fibre[2].beta) == 1 && k >= -1) {
            // [ S2 : (2,1), (...), (...) ]

            if (fibre[1] == two) {
                // [ S2 : (2,1), (2,1), (a,b) ].
                // Orlik, p112, case (ii).

                long a = fibre[2].alpha;
                long m = fibre[2].beta + a * (k + 1);

                // Note that a,m >= 0.

                if (gcd(m, 2 * a) == 1) {
                    // S3/Q{4a} x Z{m}.
                    if (tex)
                        out << "$S^3/Q_{" << (a * 4) << '}';
                    else
                        out << "S3/Q" << (a * 4);

                    if (m > 1) {
                        if (tex)
                            out << " \\times \\mathbb{Z}_{" << m << '}';
                        else
                            out << " x Z" << m;
                    }

                    if (tex)
                        out << '$';
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
                        out << "$S^3/D_{" << ((twos << 2) * a) << '}';
                    else
                        out << "S3/D" << ((twos << 2) * a);

                    if (odd > 1) {
                        if (tex)
                            out << " \\times \\mathbb{Z}_{" << odd << '}';
                        else
                            out << " x Z" << odd;
                    }

                    if (tex)
                        out << '$';
                    return out;
                }
            } else if (fibre[1] == three || fibre[1] == threeB) {
                // [ S2 : (2,1), (3,1/2), (a,b) ]
                long a = fibre[2].alpha;

                if (a == 3) {
                    // [ S2 : (2,1), (3,x), (3,y) ]
                    // Orlik, p112, case (iii).
                    long m = 6 * k + 3 + 2 * (fibre[1].beta + fibre[2].beta);
                    // Note that m >= 1.

                    if (m % 2 != 0 && m % 3 != 0) {
                        out << (tex ? "$S^3/P_{24}" : "S3/P24");

                        if (m > 1) {
                            if (tex)
                                out << " \\times \\mathbb{Z}_{" << m << '}';
                            else
                                out << " x Z" << m;
                        }

                        if (tex)
                            out << '$';
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
                            out << "$S^3/P'_{" << (threes * 24) << '}';
                        else
                            out << "S3/P'" << (threes * 24);

                        if (m > 1) {
                            if (tex)
                                out << " \\times \\mathbb{Z}_{" << m << '}';
                            else
                                out << " x Z" << m;
                        }

                        if (tex)
                            out << '$';
                        return out;
                    }
                } else if (a == 4) {
                    // [ S2 : (2,1), (3,x), (4,y) ]
                    // Orlik, p112, case (iv).
                    long m = 12 * k + 6 + 4 * fibre[1].beta +
                        3 * fibre[2].beta;
                    // Note that m >= 1.

                    out << (tex ? "$S^3/P_{48}" : "S3/P48");

                    if (m > 1) {
                        if (tex)
                            out << " \\times \\mathbb{Z}_{" << m << '}';
                        else
                            out << " x Z" << m;
                    }

                    if (tex)
                        out << '$';
                    return out;
                } else if (a == 5) {
                    // [ S2 : (2,1), (3,x), (5,y) ]
                    // Orlik, p112, case (v).
                    long m = 30 * k + 15 + 10 * fibre[1].beta +
                        6 * fibre[2].beta;
                    // Note that m >= 1.

                    out << (tex ? "$S^3/P_{120}" : "S3/P120");

                    if (m > 1) {
                        if (tex)
                            out << " \\times \\mathbb{Z}_{" << m << '}';
                        else
                            out << " x Z" << m;
                    }

                    if (tex)
                        out << '$';
                    return out;
                }
            }
        }
    }

    // SFS over the real projective plane:
    if (orbitGenus == 1 && (! orbitOrientable) && orbitPunctures == 0) {
        if (nFibres == 0) {
            // No exceptional fibres.
            if (k == 0) {
                // [ RP2 ]
                // Orlik, p113, remark.
                return (tex ? out << "$\\mathbb{R}P^3 \\# \\mathbb{R}P^3$" :
                    out << "RP3 # RP3");
            } else {
                // TODO: [ RP2 : (1,k) ]
                // Is this Orlik, p112, case (vi)?  What is this?
                // ans << "S3/Q" << (4 * (k > 0 ? k : -k));
            }
        } else if (nFibres == 1 && fibre[0].alpha > 1) {
            // Just one exceptional fibre.
            long a = fibre[0].alpha;
            long n = k * a + fibre[0].beta;
            if (n < 0)
                n = -n;

            if (n > 1) {
                // We have a prism manifold.
                // Orlik, p112, case (vi).
                if (a % 2 != 0) {
                    return (tex ?
                        out << "$S^3/Q_{" << (4 * n) <<
                            "} \\times \\mathbb{Z}_{" << a << "}$":
                        out << "S3/Q" << (4 * n) << " x Z" << a);
                } else {
                    long odd = a;
                    long twos = 1;
                    while (! (odd & 1)) {
                        odd >>= 1;
                        twos <<= 1;
                    }

                    if (tex)
                        out << "$S^3/D_{" << ((twos << 2) * n) << '}';
                    else
                        out << "S3/D" << ((twos << 2) * n);

                    if (odd > 1) {
                        if (tex)
                            out << " \\times \\mathbb{Z}_{" << odd << '}';
                        else
                            out << " x Z" << odd;
                    }

                    if (tex)
                        out << '$';
                    return out;
                }
            }
        }
    }

    // SFS over the Klein bottle:
    if (orbitGenus == 2 && (! orbitOrientable) && orbitPunctures == 0) {
        if (nFibres == 0 && k == 0) {
            // This is also [ S2 : (2,1), (2,1), (2,-1), (2,-1) ].
            return (tex ? out << "$K^2 \\twisted S^1$" : out << "KB x~ S1");
        }
    }

    return writeStructure(out);
}

} // namespace regina
