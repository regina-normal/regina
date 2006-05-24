
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
#include <cmath>
#include <complex>
#include <vector>
#include "regina-config.h"
#include "maths/approx.h"
#include "maths/numbertheory.h"
#include "triangulation/ntriangulation.h"

namespace regina {

namespace {
    /**
     * Allows calculation of [n]! for arbitrary n.
     * Values are cached as they are calculated.
     */
    class BracketFactorial {
        private:
            mutable std::vector<double> values;
                /**< The cached values [0]!, [1]!, ... . */
            double angle;
                /**< The angle arg(q0). */

        public:
            /**
             * Precalculate all values [0]!, ..., [preCalculate]!.
             * Note that [0]! will always be calculated.
             */
            BracketFactorial(double newAngle, unsigned long preCalculate = 0) :
                    angle(newAngle) {
                values.push_back(1);
                (*this)[preCalculate];
            }

            /**
             * Calculates the single value [n] (note that there is no
             * factorial symbol included).
             * These values are individually easy to calculate and so
             * are not cached.
             */
            double bracket(unsigned long index) const {
                if (index == 0 || index == 1)
                    return 1;
                return sin(angle * index) / sin(angle);
            }

            /**
             * Returns the value [index]!.
             */
            double operator [] (unsigned long index) const {
                for (unsigned long calc = values.size();
                        calc <= index; calc++)
                    values.push_back(values.back() * bracket(calc));
                return values[index];
            }
    };

    /**
     * Represents the initial data as described in Section 7 of Turaev
     * and Viro's paper.
     */
    struct InitialData {
        unsigned long r;
            /**< The integer r. */
        double angle;
            /**< The angle arg(q0). */
        BracketFactorial fact;
            /**< The cached values [n]!. */
        double baseWSquared;
            /**< The square of the distinguished value w. */

        InitialData(unsigned long newR, double newAngle) :
                r(newR), angle(newAngle), fact(angle, 3 * r / 2) {
            baseWSquared = static_cast<double>(r) /
                (2 * sin(angle) * sin(angle));
        }

        /**
         * Determines whether (i/2, j/2, k/2) is an admissible triple.
         */
        bool isAdmissible(unsigned long i, unsigned long j,
                unsigned long k) const {
            return ((i + j + k) % 2 == 0) &&
                (i <= j + k) && (j <= i + k) && (k <= i + j) &&
                (i + j + k <= 2 * (r - 2));
        }

        /**
         * Determines the face-based contribution to the Turaev-Viro
         * invariant.  This corresponds to +/- Delta(i/2, j/2, k/2)^2.
         */
        std::complex<double> faceContrib(unsigned long i, unsigned long j,
                unsigned long k) const {
            // By admissibility, (i + j + k) is guaranteed to be even.
            std::complex<double> ans =
                fact[(i + j - k) / 2] *
                fact[(j + k - i) / 2] *
                fact[(k + i - j) / 2] /
                fact[(i + j + k + 2) / 2];

            return ((i + j + k) % 4 == 0 ? ans : -ans);
        }

        /**
         * Determines the edge-based contribution to the Turaev-Viro
         * invariant.  This corresponds to w(i/2)^2.
         */
        std::complex<double> edgeContrib(unsigned long i) const {
            return (i % 2 == 0 ? fact.bracket(i + 1) : - fact.bracket(i + 1));
        }

        /**
         * Determines the tetrahedron-based contribution to the Turaev-Viro
         * invariant.  This combines with the square roots of the face-based
         * contributions for the four tetrahedron faces to give the symbol
         *
         *     | i/2 j/2 k/2 |
         *     | l/2 m/2 n/2 | .
         */
        std::complex<double> tetContrib(unsigned long i, unsigned long j,
                unsigned long k, unsigned long l, unsigned long m,
                unsigned long n) {
            std::complex<double> ans = 0.0;

            unsigned long minZ = i + j + k;
            if (minZ < i + m + n)
                minZ = i + m + n;
            if (minZ < j + l + n)
                minZ = j + l + n;
            if (minZ < k + l + m)
                minZ = k + l + m;

            unsigned long maxZ = i + j + l + m;
            if (maxZ > i + k + l + n)
                maxZ = i + k + l + n;
            if (maxZ > j + k + m + n)
                maxZ = j + k + m + n;

            double term;
            for (unsigned long z = minZ; z <= maxZ; z++) {
                if (z % 2 != 0)
                    continue;

                // We are guaranteed that z / 2 is an integer.
                term =
                    fact[(z - i - j - k) / 2] *
                    fact[(z - i - m - n) / 2] *
                    fact[(z - j - l - n) / 2] *
                    fact[(z - k - l - m) / 2] *
                    fact[(i + j + l + m - z) / 2] *
                    fact[(i + k + l + n - z) / 2] *
                    fact[(j + k + m + n - z) / 2];
                term = fact[(z + 2) / 2] / term;
                if (z % 4 != 0)
                    term = - term;

                ans += term;
            }
            return ans;
        }
    };
}

double NTriangulation::turaevViro(unsigned long r, unsigned long whichRoot)
        const {
    // Have we already calculated this invariant?
    std::pair<unsigned long, unsigned long> tvParams(r, whichRoot);
    TuraevViroSet::const_iterator it = turaevViroCache.find(tvParams);
    if (it != turaevViroCache.end())
        return (*it).second;

    // Do some basic parameter checks.
    if (r < 3)
        return 0;
    if (whichRoot >= 2 * r)
        return 0;
    if (gcd(r, whichRoot) > 1)
        return 0;

    // Set up our initial data.
    double angle = (M_PI * whichRoot) / r;
    InitialData init(r, angle);

    // Run through all admissible colourings.
    std::complex<double> ans = 0.0;

    unsigned long nEdges = getNumberOfEdges();
    unsigned long nFaces = getNumberOfFaces();
    unsigned long* colour = new unsigned long[nEdges];

    std::fill(colour, colour + nEdges, 0);
    long curr = 0;
    std::complex<double> valColour;
    bool admissible;
    std::deque<NEdgeEmbedding>::const_iterator embit;
    long index1, index2;
    unsigned long i;
    while (curr >= 0) {
        // Have we found an admissible colouring?
        if (curr >= static_cast<long>(nEdges)) {
            // Increment ans appropriately.
            valColour = 1.0;
            for (i = 0; i < vertices.size(); i++)
                valColour /= init.baseWSquared;
            for (i = 0; i < nEdges; i++)
                valColour *= init.edgeContrib(colour[i]);
            for (i = 0; i < nFaces; i++)
                valColour *= init.faceContrib(
                    colour[getEdgeIndex(faces[i]->getEdge(0))],
                    colour[getEdgeIndex(faces[i]->getEdge(1))],
                    colour[getEdgeIndex(faces[i]->getEdge(2))]);
            for (i = 0; i < tetrahedra.size(); i++)
                valColour *= init.tetContrib(
                    colour[getEdgeIndex(tetrahedra[i]->getEdge(0))],
                    colour[getEdgeIndex(tetrahedra[i]->getEdge(1))],
                    colour[getEdgeIndex(tetrahedra[i]->getEdge(3))],
                    colour[getEdgeIndex(tetrahedra[i]->getEdge(5))],
                    colour[getEdgeIndex(tetrahedra[i]->getEdge(4))],
                    colour[getEdgeIndex(tetrahedra[i]->getEdge(2))]
                    );

            ans += valColour;

            // Step back down one level.
            curr--;
            if (curr >= 0)
                colour[curr]++;
            continue;
        }

        // Have we run out of values to try at this level?
        if (colour[curr] > r - 2) {
            colour[curr] = 0;
            curr--;
            if (curr >= 0)
                colour[curr]++;
            continue;
        }

        // Does the current value for colour[curr] preserve admissibility?
        admissible = true;
        const std::deque<NEdgeEmbedding>& embs(edges[curr]->getEmbeddings());
        for (embit = embs.begin(); embit != embs.end(); embit++) {
            index1 = getEdgeIndex((*embit).getTetrahedron()->getEdge(
                edgeNumber[(*embit).getVertices()[0]]
                [(*embit).getVertices()[2]]));
            index2 = getEdgeIndex((*embit).getTetrahedron()->getEdge(
                edgeNumber[(*embit).getVertices()[1]]
                [(*embit).getVertices()[2]]));
            if (index1 <= curr && index2 <= curr) {
                // We've decided upon colours for all three edges of
                // this face containing the current edge.
                if (! init.isAdmissible(colour[index1], colour[index2],
                        colour[curr])) {
                    admissible = false;
                    break;
                }
            }
        }

        // Use the current value for colour[curr] if appropriate;
        // otherwise step forwards to the next value.
        if (admissible)
            curr++;
        else
            colour[curr]++;
    }

    delete[] colour;

    if (isNonZero(ans.imag())) {
        // This should never happen, since the Turaev-Viro invariant is the
        // square of the modulus of the Witten invariant for sl_2.
        std::cerr <<
            "WARNING: The Turaev-Viro invariant has an imaginary component.\n"
            "         This should never happen.\n"
            "         Please report this (along with the 3-manifold that"
            "         was used) to " << PACKAGE_BUGREPORT << "." << std::endl;
    }
    turaevViroCache[tvParams] = ans.real();
    return ans.real();
}

} // namespace regina

