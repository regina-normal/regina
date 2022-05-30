
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#define _USE_MATH_DEFINES // for M_PI, which is non-standard

#include <algorithm>
#include <cmath>
#include <complex>
#include <thread>
#include "regina-config.h"
#include "libnormaliz/cone.h"
#include "maths/cyclotomic.h"
#include "maths/numbertheory.h"
#include "maths/mfloat.h"
#include "progress/progresstracker.h"
#include "treewidth/treedecomposition.h"
#include "triangulation/dim3.h"
#include "utilities/sequence.h"
#include <gmpxx.h>
#include <map>


// #define TV_BACKTRACK_DUMP_COLOURINGS
// #define TV_IGNORE_CACHE

#define TV_UNCOLOURED -1
#define TV_AGGREGATED -2

// When tracking progress, try to give much more weight to larger bags.
// (Of course, this should *really* be exponential, but it's nice to see
// some visual progress for smaller bags, so we try not to completely
// dwarf them in the weightings.)
#define HARD_BAG_WEIGHT(bag) (double(bag->size())*(bag->size())*(bag->size()))

namespace regina {

namespace {
	enum calcType {exact, floating, multiPrecision};
    template <calcType method>
    struct TuraevViroDetails;

    template <>
    struct TuraevViroDetails<exact> {
        typedef Cyclotomic TVType;
        typedef Cyclotomic TVResult;

        static TVResult zero() {
            return Cyclotomic(1);
        }
    };

    template <>
    struct TuraevViroDetails<floating> {
        typedef std::complex<double> TVType;
        typedef double TVResult;

        static TVResult zero() {
            return 0;
        }
    };

    template <>
    struct TuraevViroDetails<multiPrecision> {
        typedef MFloat TVType;
        typedef MFloat TVResult;

        static TVResult zero() {
            MFloat zero = MFloat(0.0);
            return zero;
        }
    };

    /**
     * Allows calculation of [n]! for arbitrary n.
     * Values are cached as they are calculated.
     */
    template <calcType method>
    class BracketFactorial {
        public:
            using TVType = typename TuraevViroDetails<method>::TVType;
            using TVResult = typename TuraevViroDetails<method>::TVResult;

        private:
            TVResult* bracket_;
                /**< The cached brackets [0], [1], ..., [r-1] . */
            TVResult* fact_;
                /**< The cached values [0]!, [1]!, ..., [r-1]! . */
            TVResult* inv_;
                /**< The cached inverses of the values stored in fact[]. */

        public:
            /**
             * Precalculate all values [0]!, ..., [r-1]!.
             *
             * Requires r >= 3.
             */
            BracketFactorial(unsigned long r, unsigned long whichRoot);

            /**
             * Clean up memory.
             */
            ~BracketFactorial() {
                delete[] bracket_;
                delete[] fact_;
                delete[] inv_;
            }

            /**
             * To clean up the content of the arrays of the destuctor.
             */
            void clearMembers(unsigned long r);

            /**
             * Returns the single value [index] (with no factorial symbol).
             * Requires index < r.
             */
            const TVResult& bracket(unsigned long index) const {
                return bracket_[index];
            }

            /**
             * Returns the value [index]!.
             * Requires index < r.
             */
            const TVResult& operator [] (unsigned long index) const {
                return fact_[index];
            }

            /**
             * Returns the value [index]! ^ -1.
             * Requires index < r.
             */
            const TVResult& inverse(unsigned long index) const {
                return inv_[index];
            }
    };

    template <>
    BracketFactorial<exact>::BracketFactorial(
            unsigned long r, unsigned long whichRoot) :
            bracket_(new TVResult[r]),
            fact_(new TVResult[r]),
            inv_(new TVResult[r]) {
        bool halfField = (r % 2 != 0 && whichRoot % 2 == 0);
        bracket_[0].init(halfField ? r : 2 * r);
        bracket_[0][0] = 1;
        fact_[0] = fact_[1] = inv_[0] = inv_[1] =
            bracket_[1] = bracket_[0];

        TVResult q(halfField ? r : 2 * r);
        q[1] = 1;
        TVResult qInv(q);
        qInv.invert();

        TVResult base(q);
        base -= qInv;
        base.invert();

        TVResult qPow(q);
        TVResult qPowInv(qInv);

        TVResult tmp;
        for (unsigned long i = 2; i < r; i++) {
            qPow *= q;
            qPowInv *= qInv;

            bracket_[i] = qPow;
            bracket_[i] -= qPowInv;
            bracket_[i] *= base;
            fact_[i] = fact_[i - 1];
            fact_[i] *= bracket_[i];
            inv_[i] = inv_[i - 1];
            inv_[i] /= bracket_[i];
        }
    }

    template <>
    BracketFactorial<floating>::BracketFactorial(
            unsigned long r, unsigned long whichRoot) :
            bracket_(new TVResult[r]),
            fact_(new TVResult[r]),
            inv_(new TVResult[r]) {
        TVResult angle = (M_PI * whichRoot) / r;
        bracket_[0] = bracket_[1] = fact_[0] = fact_[1] =
            inv_[0] = inv_[1] = 1.0;
        for (unsigned long i = 2; i < r; i++) {
            bracket_[i] = sin(angle * i) / sin(angle);
            fact_[i] = fact_[i - 1] * bracket_[i];
            inv_[i] = inv_[i - 1] / bracket_[i];
        }
    }
    
    template <>
    BracketFactorial<multiPrecision>::BracketFactorial(
            unsigned long r, unsigned long whichRoot) :
            bracket_(new TVResult[r]),
            fact_(new TVResult[r]),
            inv_(new TVResult[r]) {

        TVResult angle,sinangle,siniangle,facti,invi;
        
        angle.setPi();
        angle *= whichRoot;
        angle /= r;

        unsigned long one = 1;

        bracket_[0].set(one);
        bracket_[1].set(one);
        fact_[0].set(one);
        fact_[1].set(one);
        inv_[0].set(one);
        inv_[1].set(one);

        sinangle = angle;
        sinangle.sin();

        siniangle = angle;
        facti = fact_[1];
        invi = inv_[1];

        for (unsigned long i = 2; i < r; i++) {
            siniangle *= i;
            siniangle.sin();
            bracket_[i] = siniangle;
            bracket_[i] /= sinangle;

            facti *= bracket_[i];
            fact_[i] = facti;
            invi /= bracket_[i];
            inv_[i] = invi;

            siniangle = angle;
        }

        angle.clear();
        sinangle.clear();
        siniangle.clear();
        facti.clear();
        invi.clear();
    }

    /**
     * Represents the initial data as described in Section 7 of Turaev
     * and Viro's paper.
     */
    template <calcType method>
    struct InitialData {
        using TVType = typename TuraevViroDetails<method>::TVType;
        using TVResult = typename TuraevViroDetails<method>::TVResult;

        unsigned long r, whichRoot;
            /**< The Turaev-Viro parameters. */
        bool halfField;
        BracketFactorial<method> fact;
            /**< The cached values [n]!. */
        TVType vertexContrib;
            /**< The vertex-based contribution to the Turaev-Viro invariant;
                 this is the inverse square of the distinguished value w. */

        InitialData(unsigned long newR, unsigned long newWhichRoot);

        /**
         * To clear BracketFactorial, to clear and delete an array of TVType of size n and to clear an element of type TVType.
         */
        void clearData();
        static void clearVar(TVType& var);
        static void clearArray(TVType* ar,unsigned long n);
//         template<class T>
        static void clearMap(std::map<LightweightSequence<int>, TVType> *map);

        static void negate(TVType& x);

        void initZero(TVType& x) const;
        void initOne(TVType& x) const;

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
         * Multiplies ans by the triangle-based contribution to the Turaev-Viro
         * invariant.  This corresponds to +/- Delta(i/2, j/2, k/2)^2.
         */
        void triContrib(unsigned long i, unsigned long j, unsigned long k,
                TVType& ans) const {
            // By admissibility, (i + j + k) is guaranteed to be even.
            ans *= fact[(i + j - k) / 2];
            ans *= fact[(j + k - i) / 2];
            ans *= fact[(k + i - j) / 2];
            ans *= fact.inverse((i + j + k + 2) / 2);
            if ((i + j + k) % 4 != 0)
                negate(ans);
        }

        /**
         * Multiplies ans by the edge-based contribution to the Turaev-Viro
         * invariant.  This corresponds to w(i/2)^2.
         */
        void edgeContrib(unsigned long i, TVType& ans) const {
            ans *= fact.bracket(i + 1);
            if (i % 2 != 0)
                negate(ans);
        }

        /**
         * Sets ansToOverwrite to the tetrahedron-based contribution to the
         * Turaev-Viro invariant.  This combines with the square roots of the
         * triangle-based contributions for the four tetrahedron faces to
         * give the symbol
         *
         *     | i/2 j/2 k/2 |
         *     | l/2 m/2 n/2 | .
         */
        void tetContrib(unsigned long i, unsigned long j,
                unsigned long k, unsigned long l, unsigned long m,
                unsigned long n, TVType& ansToOverwrite) const {
            ansToOverwrite = 0;

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

            TVType term;
            for (unsigned long z = minZ; z <= maxZ; z++) {
                if (z % 2 != 0)
                    continue;

                // We are guaranteed that z / 2 is an integer.
                if (((z + 2) / 2) < r) {
                    term = fact[(z + 2) / 2];
                    term *= fact.inverse((z - i - j - k) / 2);
                    term *= fact.inverse((z - i - m - n) / 2);
                    term *= fact.inverse((z - j - l - n) / 2);
                    term *= fact.inverse((z - k - l - m) / 2);
                    term *= fact.inverse((i + j + l + m - z) / 2);
                    term *= fact.inverse((i + k + l + n - z) / 2);
                    term *= fact.inverse((j + k + m + n - z) / 2);

                    if (z % 4 == 0)
                        ansToOverwrite += term;
                    else
                        ansToOverwrite -= term;
                }
            }
            clearVar(term);
        }

        /**
         * Multiplies ans by a single tetrahedron-based contribution
         * along with all triangle and edge contributions for which that
         * tetrahedron is responsible.  A tetrahedron is "responsible" for
         * a triangle or edge contribution iff it is the tetrahedron
         * referenced by front() for that triangle or edge.
         *
         * The six arguments colour0, ..., colour5 refer to the colours
         * on tetrahedron edges 0, ..., 5 respectively.
         */
        void tetContrib(const Tetrahedron<3>* tet,
                unsigned long colour0, unsigned long colour1,
                unsigned long colour2, unsigned long colour3,
                unsigned long colour4, unsigned long colour5,
                TVType& ans) const {
            TVType tmp(halfField ? r : 2 * r);
            tetContrib(colour0, colour1, colour3, colour5, colour4, colour2,
                tmp);
            ans *= tmp;

            int i;
            const Triangle<3>* triangle;
            const Edge<3>* edge;
            for (i = 0; i < 4; ++i) {
                triangle = tet->triangle(i);
                if (triangle->front().tetrahedron() == tet &&
                        triangle->front().triangle() == i) {
                    switch (i) {
                        case 0:
                            triContrib(colour3, colour4, colour5, ans);
                            break;
                        case 1:
                            triContrib(colour1, colour2, colour5, ans);
                            break;
                        case 2:
                            triContrib(colour0, colour2, colour4, ans);
                            break;
                        case 3:
                            triContrib(colour0, colour1, colour3, ans);
                            break;
                    }
                }
            }
            for (i = 0; i < 6; ++i) {
                edge = tet->edge(i);
                if (edge->front().tetrahedron() == tet &&
                        edge->front().edge() == i) {
                    switch (i) {
                        case 0: edgeContrib(colour0, ans); break;
                        case 1: edgeContrib(colour1, ans); break;
                        case 2: edgeContrib(colour2, ans); break;
                        case 3: edgeContrib(colour3, ans); break;
                        case 4: edgeContrib(colour4, ans); break;
                        case 5: edgeContrib(colour5, ans); break;
                    }
                }
            }
        }
    };

    template <>
    InitialData<exact>::InitialData(
            unsigned long newR, unsigned long newWhichRoot) :
            r(newR),
            whichRoot(newWhichRoot),
            halfField(r % 2 != 0 && whichRoot % 2 == 0),
            fact(r, whichRoot) {
        // vertexContrib should be |q - q^-1|^2 / 2r.
        vertexContrib.init(halfField ? r : 2 * r);
        vertexContrib[1] = 1;
        TVResult inv(vertexContrib);
        inv.invert();

        vertexContrib -= inv;           // Pure imaginary.
        vertexContrib *= vertexContrib; // Gives -|..|^2
        vertexContrib.negate();         // Gives +|..|^2
        vertexContrib /= (2 * r);
    }

    template <>
    InitialData<floating>::InitialData(
            unsigned long newR, unsigned long newWhichRoot) :
            r(newR),
            whichRoot(newWhichRoot),
            halfField(r % 2 != 0 && whichRoot % 2 == 0),
            fact(r, whichRoot) {
        double tmp = sin(M_PI * whichRoot / r);
        vertexContrib = 2.0 * tmp * tmp / r;
    }
    
    template <>
    InitialData<multiPrecision>::InitialData(
            unsigned long newR, unsigned long newWhichRoot) :
            r(newR),
            whichRoot(newWhichRoot),
            halfField(r % 2 != 0 && whichRoot % 2 == 0),
            fact(r, whichRoot) {
        TVResult tmp;
        tmp.setPi();
        tmp *= whichRoot;
        tmp /= r;
        tmp.sin();
        tmp *= tmp;
        unsigned long two = 2;
        tmp *= two;
        tmp /= r;
        vertexContrib = tmp;
        tmp.clear();
    }

        template <>
    inline void BracketFactorial<multiPrecision>::clearMembers(unsigned long r) {
        for (unsigned long i = 0; i<r;++i) {
            bracket_[i].clear();
            fact_[i].clear();
            inv_[i].clear();
        }
    }

    template <>
    inline void InitialData<multiPrecision>::clearData() {
        fact.clearMembers(r);
        vertexContrib.clear();
    }



    template <>
    inline void InitialData<exact>::clearVar(TVType& var) {}

    template <>
    inline void InitialData<floating>::clearVar(TVType& var) {}

    template <>
    inline void InitialData<multiPrecision>::clearVar(TVType& var) {
        var.clear();
    }

    template <>
    inline void InitialData<exact>::clearArray(TVType* ar,unsigned long n) {
        delete[] ar;
    }

    template <>
    inline void InitialData<floating>::clearArray(TVType* ar,unsigned long n) {
        delete[] ar;
    }

    template <>
    inline void InitialData<multiPrecision>::clearArray(TVType* ar,unsigned long n) {
        for (unsigned long i=0;i<n;++i) {
            ar[i].clear();
        }
        delete[] ar;
    }

    template <>
//     template <class T>
    inline void InitialData<exact>::clearMap(std::map<LightweightSequence<int>, TVType>* map) {
        delete map;
    }

    template <>
//     template <class T>
    inline void InitialData<floating>::clearMap(std::map<LightweightSequence<int>, TVType>* map) {
        delete[] map;
    }

    template<>
//     template <class T>
    inline void InitialData<multiPrecision>::clearMap(std::map<LightweightSequence<int>, TVType>* map) {
        for(auto &x:*map) {x.second.clear();}
        delete map;
    }


    template <>
    inline void InitialData<exact>::negate(InitialData<exact>::TVType& x) {
        x.negate();
    }

    template <>
    inline void InitialData<floating>::negate(InitialData<floating>::TVType& x) {
        x = -x;
    }

    template <>
    inline void InitialData<multiPrecision>::negate(InitialData<multiPrecision>::TVType& x) {
        x.negate();
    }

    template <>
    inline void InitialData<exact>::initZero(InitialData<exact>::TVType& x)
            const {
        x.init(halfField ? r : 2 * r);
    }

    template <>
    inline void InitialData<floating>::initZero(InitialData<floating>::TVType& x)
            const {
        x = 0.0;
    }

    template <>
    inline void InitialData<multiPrecision>::initZero(InitialData<multiPrecision>::TVType& x)
            const {
        x.set(0.0);
    }

    template <>
    inline void InitialData<exact>::initOne(InitialData<exact>::TVType& x)
            const {
        x.init(halfField ? r : 2 * r);
        x[0] = 1;
    }

    template <>
    inline void InitialData<floating>::initOne(InitialData<floating>::TVType& x)
            const {
        x = 1.0;
    }

    template <>
    inline void InitialData<multiPrecision>::initOne(InitialData<multiPrecision>::TVType& x)
            const {
        x.set(1.0);
    }

    template <calcType method>
    typename InitialData<method>::TVType turaevViroBacktrack(
            const Triangulation<3>& tri,
            const InitialData<method>& init,
            ProgressTracker* tracker) {
        using TVType = typename InitialData<method>::TVType;

        if (tracker)
            tracker->newStage("Enumerating colourings");

        unsigned long nEdges = tri.countEdges();
        unsigned long nTriangles = tri.countTriangles();
        unsigned long nTet = tri.size();

        // Our plan is to run through all admissible colourings via a
        // backtracking search, with the high-degree edges towards the root
        // of the search tree and the low-degree edges towards the leaves.

        // We first sort the edges by degree.
        unsigned long i, j;
        auto* sortedEdges = new unsigned long[nEdges];
        auto* edgePos = new unsigned long[nEdges];

        for (i = 0; i < nEdges; ++i)
            sortedEdges[i] = i;
        std::sort(sortedEdges, sortedEdges + nEdges,
            DegreeGreaterThan<3, 1>(tri));
        for (i = 0; i < nEdges; ++i)
            edgePos[sortedEdges[i]] = i;

        // Work out which triangles and tetrahedra will be completely
        // coloured at each level of the search tree.
        //
        // The following code contains some quadratic loops; we don't
        // worry about this since it makes the code simpler and the
        // overall algorithm is much slower (exponential) anyway.
        unsigned long* tmp;

        tmp = new unsigned long[nTriangles];
        for (i = 0; i < nEdges; ++i) {
            for (auto& emb : *tri.edge(sortedEdges[i]))
                tmp[emb.tetrahedron()->
                    triangle(emb.vertices()[2])->index()] = i;
        }
        auto* triDone = new unsigned long[nTriangles];
        auto* triDoneStart = new unsigned long[nEdges + 1];
        triDoneStart[0] = 0;
        for (i = 0; i < nEdges; ++i) {
            triDoneStart[i + 1] = triDoneStart[i];
            for (j = 0; j < nTriangles; ++j)
                if (tmp[j] == i)
                    triDone[triDoneStart[i + 1]++] = j;
        }
        delete[] tmp;

        tmp = new unsigned long[nTet];
        for (i = 0; i < nEdges; ++i)
            for (auto& emb : *tri.edge(sortedEdges[i]))
                tmp[emb.tetrahedron()->index()] = i;
        auto* tetDone = new unsigned long[nTet];
        auto* tetDoneStart = new unsigned long[nEdges + 1];
        tetDoneStart[0] = 0;
        for (i = 0; i < nEdges; ++i) {
            tetDoneStart[i + 1] = tetDoneStart[i];
            for (j = 0; j < nTet; ++j)
                if (tmp[j] == i)
                    tetDone[tetDoneStart[i + 1]++] = j;
        }
        delete[] tmp;

        // Caches for partially computed weights of colourings:
        auto* edgeCache = new TVType[nEdges + 1];
        init.initOne(edgeCache[0]);

        auto* triangleCache = new TVType[nEdges + 1];
        init.initOne(triangleCache[0]);

        auto* tetCache = new TVType[nEdges + 1];
        init.initOne(tetCache[0]);

        // Run through all admissible colourings.
        TVType ans;
        init.initZero(ans);

        // Now hunt for colourings.
        auto* colour = new unsigned long[nEdges];

        std::fill(colour, colour + nEdges, 0);
        long curr = 0;
        TVType valColour(init.halfField ? init.r : 2 * init.r);
        TVType tmpTVType(init.halfField ? init.r : 2 * init.r);
        bool admissible;
        const Tetrahedron<3>* tet;
        const Triangle<3>* triangle;

        double percent;
        double* coeff;
        if (tracker) {
            coeff = new double[nEdges];
            if (nEdges) {
                coeff[0] = 100.0 / (init.r - 1);
                for (i = 1; i < nEdges; ++i)
                    coeff[i] = coeff[i - 1] / (init.r - 1);
            }
        }

        while (curr >= 0) {
            // Have we found an admissible colouring?
            if (curr >= static_cast<long>(nEdges)) {
#ifdef TV_BACKTRACK_DUMP_COLOURINGS
                for (i = 0; i < nEdges; ++i) {
                    if (i > 0)
                        std::cout << ' ';
                    std::cout << colour[i];
                }
#endif
                // Increment ans appropriately.
                valColour = edgeCache[curr];
                valColour *= triangleCache[curr];
                valColour *= tetCache[curr];

#ifdef TV_BACKTRACK_DUMP_COLOURINGS
                std::cout << "  -->  " << valColour << std::endl;
#endif
                ans += valColour;

                // Step back down one level.
                curr--;
                if (curr >= 0)
                    colour[sortedEdges[curr]]++;
                continue;
            }

            // From here we have 0 <= curr < nEdges.

            if (tracker) {
                percent = 0;
                for (i = 0; i <= curr; ++i)
                    percent += coeff[i] * colour[sortedEdges[i]];

                if (! tracker->setPercent(percent))
                    break;
            }

            // Have we run out of values to try at this level?
            if (colour[sortedEdges[curr]] > init.r - 2) {
                colour[sortedEdges[curr]] = 0;
                curr--;
                if (curr >= 0)
                    colour[sortedEdges[curr]]++;
                continue;
            }

            // Does the current value for colour[sortedEdges[curr]]
            // preserve admissibility?
            admissible = true;
            for (i = triDoneStart[curr];
                    admissible && i < triDoneStart[curr + 1]; ++i) {
                triangle = tri.triangle(triDone[i]);
                if (! init.isAdmissible(
                        colour[triangle->edge(0)->index()],
                        colour[triangle->edge(1)->index()],
                        colour[triangle->edge(2)->index()]))
                    admissible = false;
            }

            // Use the current value for colour[curr] if appropriate;
            // otherwise step forwards to the next value.
            if (admissible) {
                curr++;

                edgeCache[curr] = edgeCache[curr - 1];
                init.edgeContrib(colour[sortedEdges[curr - 1]],
                    edgeCache[curr]);

                triangleCache[curr] = triangleCache[curr - 1];
                for (i = triDoneStart[curr - 1]; i < triDoneStart[curr]; ++i) {
                    triangle = tri.triangle(triDone[i]);
                    init.triContrib(
                        colour[triangle->edge(0)->index()],
                        colour[triangle->edge(1)->index()],
                        colour[triangle->edge(2)->index()],
                        triangleCache[curr]);
                }

                tetCache[curr] = tetCache[curr - 1];
                for (i = tetDoneStart[curr - 1]; i < tetDoneStart[curr]; ++i) {
                    // Unlike the others, this call overwrites tmpTVType.
                    tet = tri.tetrahedron(tetDone[i]);
                    init.tetContrib(
                        colour[tet->edge(0)->index()],
                        colour[tet->edge(1)->index()],
                        colour[tet->edge(3)->index()],
                        colour[tet->edge(5)->index()],
                        colour[tet->edge(4)->index()],
                        colour[tet->edge(2)->index()],
                        tmpTVType);
                    tetCache[curr] *= tmpTVType;
                }
            } else
                colour[sortedEdges[curr]]++;
        }

        delete[] colour;
        delete[] sortedEdges;
        delete[] edgePos;
        delete[] triDone;
        delete[] triDoneStart;
        delete[] tetDone;
        delete[] tetDoneStart;

        InitialData<method>::clearArray(edgeCache,nEdges + 1);
        InitialData<method>::clearArray(triangleCache,nEdges + 1);
        InitialData<method>::clearArray(tetCache,nEdges + 1);

        InitialData<method>::clearVar(valColour);
        InitialData<method>::clearVar(tmpTVType);

        if (tracker) {
            delete[] coeff;
            if (tracker->isCancelled())
                return TuraevViroDetails<method>::zero();
        }

        // Compute the vertex contributions separately, since these are
        // constant.
        for (i = 0; i < tri.countVertices(); i++)
            ans *= init.vertexContrib;

        return ans;
    }

    template <calcType method>
    typename InitialData<method>::TVType turaevViroNaive(
            const Triangulation<3>& tri,
            const InitialData<method>& init,
            ProgressTracker* tracker) {
        using TVType = typename InitialData<method>::TVType;

        if (tracker)
            tracker->newStage("Enumerating colourings");

        unsigned long nEdges = tri.countEdges();

        // Our plan is to run through all admissible colourings via a
        // backtracking search, with the high-degree edges towards the root
        // of the search tree and the low-degree edges towards the leaves.

        // We first sort the edges by degree.
        unsigned long i;
        auto* sortedEdges = new unsigned long[nEdges];
        auto* edgePos = new unsigned long[nEdges];

        for (i = 0; i < nEdges; ++i)
            sortedEdges[i] = i;
        std::sort(sortedEdges, sortedEdges + nEdges,
            DegreeGreaterThan<3, 1>(tri));
        for (i = 0; i < nEdges; ++i)
            edgePos[sortedEdges[i]] = i;

        // Run through all admissible colourings.
        TVType ans;
        init.initZero(ans);

        // Now hunt for colourings.
        auto* colour = new unsigned long[nEdges];

        std::fill(colour, colour + nEdges, 0);
        long curr = 0;
        TVType valColour(init.halfField ? init.r : 2 * init.r);
        bool admissible;
        long index1, index2;
        const Tetrahedron<3>* tet;

        double percent;
        double* coeff;
        if (tracker) {
            coeff = new double[nEdges];
            if (nEdges) {
                coeff[0] = 100.0 / (init.r - 1);
                for (i = 1; i < nEdges; ++i)
                    coeff[i] = coeff[i - 1] / (init.r - 1);
            }
        }

        while (curr >= 0) {
            // Have we found an admissible colouring?
            if (curr >= static_cast<long>(nEdges)) {
#ifdef TV_BACKTRACK_DUMP_COLOURINGS
                for (i = 0; i < nEdges; ++i) {
                    if (i > 0)
                        std::cout << ' ';
                    std::cout << colour[i];
                }
#endif
                // Increment ans appropriately.
                valColour = 1;
                for (i = 0; i < tri.size(); i++) {
                    tet = tri.tetrahedron(i);
                    init.tetContrib(tet,
                        colour[tet->edge(0)->index()],
                        colour[tet->edge(1)->index()],
                        colour[tet->edge(2)->index()],
                        colour[tet->edge(3)->index()],
                        colour[tet->edge(4)->index()],
                        colour[tet->edge(5)->index()],
                        valColour);
                }

#ifdef TV_BACKTRACK_DUMP_COLOURINGS
                std::cout << "  -->  " << valColour << std::endl;
#endif
                ans += valColour;

                // Step back down one level.
                curr--;
                if (curr >= 0)
                    colour[sortedEdges[curr]]++;
                continue;
            }

            // From here we have 0 <= curr < nEdges.

            if (tracker) {
                percent = 0;
                for (i = 0; i <= curr; ++i)
                    percent += coeff[i] * colour[sortedEdges[i]];

                if (! tracker->setPercent(percent))
                    break;
            }

            // Have we run out of values to try at this level?
            if (colour[sortedEdges[curr]] > init.r - 2) {
                colour[sortedEdges[curr]] = 0;
                curr--;
                if (curr >= 0)
                    colour[sortedEdges[curr]]++;
                continue;
            }

            // Does the current value for colour[curr] preserve admissibility?
            admissible = true;
            for (auto& emb : *tri.edge(sortedEdges[curr])) {
                index1 = emb.tetrahedron()->edge(
                    Edge<3>::edgeNumber[emb.vertices()[0]]
                    [emb.vertices()[2]])->index();
                index2 = emb.tetrahedron()->edge(
                    Edge<3>::edgeNumber[emb.vertices()[1]]
                    [emb.vertices()[2]])->index();
                if (edgePos[index1] <= curr && edgePos[index2] <= curr) {
                    // We've decided upon colours for all three edges of
                    // this triangle containing the current edge.
                    if (! init.isAdmissible(colour[index1], colour[index2],
                            colour[sortedEdges[curr]])) {
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
                colour[sortedEdges[curr]]++;
        }

        delete[] colour;
        delete[] sortedEdges;
        delete[] edgePos;

        if (tracker) {
            delete[] coeff;
            if (tracker->isCancelled())
                return TuraevViroDetails<method>::zero();
        }

        // Compute the vertex contributions separately, since these are
        // constant.
        for (i = 0; i < tri.countVertices(); i++)
            ans *= init.vertexContrib;

        InitialData<method>::clearVar(valColour);
        return ans;
    }

    template <calcType method>
    typename InitialData<method>::TVType turaevViroTreewidth(
            const Triangulation<3>& tri,
            InitialData<method>& init,
            ProgressTracker* tracker) {
        using TVType = typename InitialData<method>::TVType;

        // Progress:
        // - weight of forget/join bag processing is 0.9
        // - weight of leaf/introduce bag processing is 0.05
        // - weight of other miscellaneous tasks is 0.05

        if (tracker)
            tracker->newStage("Building tree decomposition", 0.03);

        const TreeDecomposition& d = tri.niceTreeDecomposition();

        int nEdges = tri.countEdges();
        size_t nBags = d.size();
        size_t nEasyBags = 0;
        double hardBagWeightSum = 0;
        const TreeBag *bag, *child, *sibling;
        int i, j;
        int index;
        const Tetrahedron<3>* tet;
        const Edge<3>* edge;

        if (tracker) {
            if (tracker->isCancelled())
                return TuraevViroDetails<method>::zero();
            tracker->newStage("Analysing bags", 0.01);
        }

        // In the seenDegree[] array, an edge that has been seen in all
        // of its tetrahedra will be marked as seenDegree[i] = -1 (as
        // opposed to seenDegree[i] = tri.edge(i)->degree()).
        // This is simply to make such a condition easier to test.
        auto* seenDegree = new LightweightSequence<int>[nBags];

        for (bag = d.first(); bag; bag = bag->next()) {
            index = bag->index();
            seenDegree[index].init(nEdges);

            if (bag->isLeaf()) {
                ++nEasyBags;
                std::fill(seenDegree[index].begin(),
                    seenDegree[index].end(), 0);
            } else if (bag->type() == NICE_INTRODUCE) {
                // Introduce bag.
                ++nEasyBags;
                child = bag->children();
                std::copy(seenDegree[child->index()].begin(),
                    seenDegree[child->index()].end(),
                    seenDegree[index].begin());
            } else if (bag->type() == NICE_FORGET) {
                // Forget bag.
                hardBagWeightSum += HARD_BAG_WEIGHT(bag);
                child = bag->children();
                tet = tri.tetrahedron(child->element(bag->subtype()));
                std::copy(seenDegree[child->index()].begin(),
                    seenDegree[child->index()].end(),
                    seenDegree[index].begin());
                for (i = 0; i < 6; ++i) {
                    edge = tet->edge(i);
                    ++seenDegree[index][edge->index()];
                    if (seenDegree[index][edge->index()] == edge->degree())
                        seenDegree[index][edge->index()] = -1;
                }
            } else {
                // Join bag.
                hardBagWeightSum += HARD_BAG_WEIGHT(bag);
                child = bag->children();
                sibling = child->sibling();
                for (i = 0; i < nEdges; ++i) {
                    seenDegree[index][i] = seenDegree[child->index()][i] +
                        seenDegree[sibling->index()][i];
                    if (seenDegree[index][i] == tri.edge(i)->degree())
                        seenDegree[index][i] = -1;
                }
            }
        }

        using SolnSet = std::map<LightweightSequence<int>, TVType>;
        using SolnIterator = typename SolnSet::iterator;

        auto* partial = new SolnSet*[nBags];
        std::fill(partial, partial + nBags, nullptr);

        std::pair<SolnIterator, bool> existingSoln;
        int tetEdge[6];
        int colour[6];
        int level;
        bool ok;

        auto* overlap = new size_t[nEdges];
        size_t nOverlap;
        SolnIterator *leftIndexed, *rightIndexed;
        size_t nLeft, nRight;
        SolnIterator *subit, *subit2;
        std::pair<SolnIterator*, SolnIterator*> subrange, subrange2;
        double increment, percent;

        // For each new tetrahedron that appears in a forget bag, we
        // colour its edges in the order 5,4,3,2,1,0.
        // This is so that we get triangles appearing as soon as possible
        // (edges 5-4-3 form a triangle, but edges 0-1-2 do not).
        //
        // To help in the decision making, choiceType[i] stores for
        // tetrahedron edge i:
        // * 0 if we must colour the edge here;
        // * -1 if the edge was already coloured in the child bag;
        // * x ∈ {5,...,1} if the edge was already coloured in this bag
        //   when it appeared as edge x of this same new tetrahedron.
        int choiceType[6];

        for (bag = d.first(); bag; bag = bag->next()) {
            index = bag->index();

            if (bag->isLeaf()) {
                if (tracker) {
                    if (tracker->isCancelled())
                        break;
                    tracker->newStage(
                        "Processing leaf bag (" + std::to_string(index) +
                            '/' + std::to_string(nBags) + ')',
                        0.05 / nEasyBags);
                }

                // A single empty colouring.
                LightweightSequence<int> seq(nEdges);
                std::fill(seq.begin(), seq.end(), TV_UNCOLOURED);

                TVType val;
                init.initOne(val);

                partial[index] = new SolnSet;
                partial[index]->emplace(std::move(seq), std::move(val));
            } else if (bag->type() == NICE_INTRODUCE) {
                // Introduce bag.
                if (tracker) {
                    if (tracker->isCancelled())
                        break;
                    tracker->newStage(
                        "Processing introduce bag (" + std::to_string(index) +
                            '/' + std::to_string(nBags) + ')',
                        0.05 / nEasyBags);
                }

                child = bag->children();
                partial[index] = partial[child->index()];
                partial[child->index()] = nullptr;
            } else if (bag->type() == NICE_FORGET) {
                // Forget bag.
                if (tracker) {
                    if (tracker->isCancelled())
                        break;
                    tracker->newStage(
                        "Processing forget bag (" + std::to_string(index) +
                            '/' + std::to_string(nBags) + ')',
                        0.9 * HARD_BAG_WEIGHT(bag) / hardBagWeightSum);
                }

                child = bag->children();
                tet = tri.tetrahedron(child->element(bag->subtype()));

                for (i = 5; i >= 0; --i) {
                    tetEdge[i] = tet->edge(i)->index();
                    if (seenDegree[child->index()][tetEdge[i]] > 0) {
                        // The child will have already coloured this edge.
                        choiceType[i] = -1;
                    } else {
                        choiceType[i] = 0;
                        for (j = 5; j > i; --j)
                            if (tetEdge[j] == tetEdge[i]) {
                                // We will have already coloured this edge
                                // because it reappears as a higher-numbered
                                // edge of this same tetrahedron.
                                choiceType[i] = j;
                                break;
                            }
                    }
                }

                partial[index] = new SolnSet;

                increment = 100.0 / partial[child->index()]->size();
                percent = 0;

                for (const auto& soln : *partial[child->index()]) {
                    if (tracker) {
                        percent += increment;
                        if (! tracker->setPercent(percent))
                            break;
                    }
                    for (i = 0; i < 6; ++i)
                        colour[i] = (choiceType[i] < 0 ?
                            soln.first[tetEdge[i]] : -1);

                    level = 5;
                    while (level < 6) {
                        if (level < 0) {
                            // We have an admissible partial colouring.

                            // First, compute its (partial) weight:
                            TVType val = soln.second;
                            init.tetContrib(tet,
                                colour[0], colour[1], colour[2],
                                colour[3], colour[4], colour[5], val);

                            // Next, compute the sequence of colours
                            // that we will use as a lookup key.
                            // For any edges that never appear beyond
                            // this bag, we mark them for aggregation.
                            LightweightSequence<int> seq(nEdges);
                            for (i = 0; i < nEdges; ++i)
                                if (seenDegree[index][i] < 0)
                                    seq[i] = TV_AGGREGATED;
                                else
                                    seq[i] = soln.first[i];
                            for (i = 0; i < 6; ++i)
                                if (choiceType[i] == 0 &&
                                        seq[tetEdge[i]] != TV_AGGREGATED)
                                    seq[tetEdge[i]] = colour[i];

                            // Finally, insert the solution into the
                            // lookup table, aggregating with existing
                            // solutions if need be.
                            existingSoln = partial[index]->try_emplace(
                                std::move(seq), std::move(val));
                            if (! existingSoln.second) {
                                existingSoln.first->second += val;
                                InitialData<method>::clearVar(val);
                            }
                            ++level;
                            while (level < 6 && choiceType[level] != 0)
                                ++level;
                            continue;
                        }

                        if (choiceType[level] > 0)
                            colour[level] = colour[choiceType[level]];
                        else if (choiceType[level] == 0) {
                            if (colour[level] <
                                    static_cast<long>(init.r) - 2)
                                ++colour[level];
                            else {
                                // Out of choices at this level.
                                colour[level] = -1;
                                ++level;
                                while (level < 6 && choiceType[level] != 0)
                                    ++level;
                                continue;
                            }
                        }

                        ok = true;
                        if (level == 3 && ! init.isAdmissible(
                                colour[3], colour[4], colour[5]))
                            ok = false;
                        if (level == 1 && ! init.isAdmissible(
                                colour[1], colour[2], colour[5]))
                            ok = false;
                        if (level == 0 && ! init.isAdmissible(
                                colour[0], colour[2], colour[4]))
                            ok = false;
                        if (level == 0 && ! init.isAdmissible(
                                colour[0], colour[1], colour[3]))
                            ok = false;
                        if (! ok) {
                            // This colouring is inadmissible.
                            // If we have a choice for this edge then
                            // move on to the next colour.
                            // If the colour of this edge is forced then
                            // backtrack.
                            while (level < 6 && choiceType[level] != 0)
                                ++level;
                            continue;
                        }

                        --level;
                    }
                }

                InitialData<method>::clearMap(partial[child->index()]);
                partial[child->index()] = nullptr;
            } else {
                // Join bag.
                if (tracker) {
                    if (tracker->isCancelled())
                        break;
                    tracker->newStage(
                        "Processing join bag (" + std::to_string(index) +
                            '/' + std::to_string(nBags) + ')',
                        0.9 * HARD_BAG_WEIGHT(bag) / hardBagWeightSum);
                }

                partial[index] = new SolnSet;

                child = bag->children();
                sibling = child->sibling();

                nOverlap = 0;
                for (i = 0; i < nEdges; ++i)
                    if (seenDegree[child->index()][i] != 0 &&
                            seenDegree[sibling->index()][i] != 0)
                    overlap[nOverlap++] = i;

                // The final template argument (which expands to size_t*)
                // should be deducible, but gcc bug #79501 prevents us from
                // declaring the appropriate deduction guide.
                LightweightSequence<int>::SubsequenceCompareFirst<
                    decltype(overlap)> compare(overlap, overlap + nOverlap);

                if (tracker && tracker->isCancelled())
                    break;

                nLeft = 0;
                leftIndexed = new SolnIterator[partial[child->index()]->size()];
                for (auto it = partial[child->index()]->begin();
                        it != partial[child->index()]->end(); ++it)
                    leftIndexed[nLeft++] = it;
                std::sort(leftIndexed, leftIndexed + nLeft, compare);

                if (tracker && tracker->isCancelled())
                    break;

                nRight = 0;
                rightIndexed = new SolnIterator[
                    partial[sibling->index()]->size()];
                for (auto it = partial[sibling->index()]->begin();
                        it != partial[sibling->index()]->end(); ++it)
                    rightIndexed[nRight++] = it;
                std::sort(rightIndexed, rightIndexed + nRight, compare);

                subrange.second = leftIndexed;
                subrange2.second = rightIndexed;

                while (subrange.second != leftIndexed + nLeft &&
                        subrange2.second != rightIndexed + nRight) {
                    if (tracker) {
                        percent = 100.0 * (
                            (subrange.second - leftIndexed) +
                            (subrange2.second - rightIndexed)) /
                            (nLeft + nRight);
                        if (! tracker->setPercent(percent))
                            break;
                    }

                    subrange.first = subrange.second;
                    while (subrange.second != leftIndexed + nLeft &&
                            compare.equal(*subrange.first, *subrange.second))
                        ++subrange.second;

                    subrange2.first = subrange2.second;
                    while (subrange2.first != rightIndexed + nRight &&
                            compare.less(*subrange2.first, *subrange.first))
                        ++subrange2.first;

                    if (subrange2.first == rightIndexed + nRight)
                        break;
                    if (compare.less(*subrange.first, *subrange2.first))
                        continue;

                    subrange2.second = subrange2.first;
                    while (subrange2.second != rightIndexed + nRight &&
                            compare.equal(*subrange2.first, *subrange2.second))
                        ++subrange2.second;

                    for (subit = subrange.first;
                            subit != subrange.second; ++subit)
                        for (subit2 = subrange2.first;
                                subit2 != subrange2.second; ++subit2) {
                            // We have two compatible solutions.
                            // Combine them and store the corresponding
                            // value, again aggregating if necessary.
                            TVType val = (*subit)->second;
                            val *=(*subit2)->second;

                            LightweightSequence<int> seq(nEdges);
                            for (i = 0; i < nEdges; ++i)
                                if (seenDegree[index][i] < 0)
                                    seq[i] = TV_AGGREGATED;
                                else if (seenDegree[child->index()][i] > 0)
                                    seq[i] = ((*subit)->first)[i];
                                else
                                    seq[i] = ((*subit2)->first)[i];

                            existingSoln = partial[index]->try_emplace(
                                std::move(seq), std::move(val));
                            if (! existingSoln.second) {
                                existingSoln.first->second += val;
                                InitialData<method>::clearVar(val);
                            }
                        }
                }

                delete[] leftIndexed;
                delete[] rightIndexed;

                InitialData<method>::clearMap(partial[child->index()]);
                partial[child->index()] = nullptr;

                InitialData<method>::clearMap(partial[sibling->index()]);
                partial[sibling->index()] = nullptr;
            }

#ifdef TV_BACKTRACK_DUMP_COLOURINGS
            std::cout << "Bag " << bag->index() << ":" << std::endl;
            for (const auto& soln : partial[index])
                std::cout << soln.first << " -> " soln.second << std::endl;
#endif
        }

        // Clean up.
        // Unfortunately, if we have cancelled mid-calculation, the
        // cleanup could be significant.
        // If we made it to the end, then the cleanup is O(1).

        delete[] seenDegree;
        delete[] overlap;

        if (tracker && tracker->isCancelled()) {
            // We don't know which elements of partial[] have been
            // deallocated, so check them all.
            for (i = 0; i < nBags; ++i)
                delete partial[i];
            delete[] partial;

            return TuraevViroDetails<method>::zero();
        }

        // We made it to the end.
        // All elements of partial[] except the last should have already
        // been deallocated (during the processing of their parent bags).
        // The final bag contains no tetrahedra, and so it should have
        // only one colouring stored (in which all edge colours are aggregated).
        TVType ans = partial[nBags - 1]->begin()->second;

        InitialData<method>::clearMap(partial[nBags - 1]);
        delete[] partial;

        for (i = 0; i < tri.countVertices(); i++)
            ans *= init.vertexContrib;
        return ans;
    }

    template <calcType method>
    typename InitialData<method>::TVType turaevViroPolytope(
            const Triangulation<3>& tri,
            InitialData<method>& init) {
        using TVType = typename InitialData<method>::TVType;

        std::vector<std::vector<mpz_class> > input;
        unsigned long nTri = tri.countTriangles();

        const Tetrahedron<3>* tet;
        Perm<4> p;
        unsigned long i;
        for (Edge<3>* edge : tri.edges()) {
            for (auto& emb : *edge) {
                std::vector<mpz_class>& v(input.emplace_back());
                v.reserve(3 * nTri);

                for (i = 0; i < 3 * nTri; ++i)
                    v.emplace_back(long(0));

                tet = emb.tetrahedron();
                p = emb.vertices();

                ++v[3 * tet->triangle(p[2])->index() +
                    tet->triangleMapping(p[2]).pre(p[0])];
                ++v[3 * tet->triangle(p[2])->index() +
                    tet->triangleMapping(p[2]).pre(p[1])];
                --v[3 * tet->triangle(p[3])->index() +
                    tet->triangleMapping(p[3]).pre(p[0])];
                --v[3 * tet->triangle(p[3])->index() +
                    tet->triangleMapping(p[3]).pre(p[1])];
            }
        }

        std::cout << "Equations:" << std::endl;
        for (const auto& in : input) {
            for (auto j = in.begin(); j != in.end(); ++j) {
                if (j != in.begin())
                    std::cout << ' ';
                std::cout << *j;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << "Hilbert basis:" << std::endl;

        libnormaliz::Cone<mpz_class> cone(libnormaliz::Type::equations, input);
        libnormaliz::ConeProperties wanted(
            libnormaliz::ConeProperty::HilbertBasis);
        cone.deactivateChangeOfPrecision();
        cone.compute(wanted);

        if (! cone.isComputed(libnormaliz::ConeProperty::HilbertBasis)) {
            std::cerr << "ERROR: Hilbert basis not computed!" << std::endl;
            return TVType(init.halfField ? init.r : 2 * init.r);
        }
        const std::vector<std::vector<mpz_class> > basis =
            cone.getHilbertBasis();

        unsigned long j;
        for (i = 0; i < basis.size(); ++i) {
            for (j = 0; j < basis[i].size(); ++j)
                std::cout << basis[i][j] << ' ';
            std::cout << std::endl;
        }

        return TVType(init.halfField ? init.r : 2 * init.r);
    }
}

double Triangulation<3>::turaevViroApprox(unsigned long r,
        unsigned long whichRoot, Algorithm alg, unsigned long prec) const {
    // Do some basic parameter checks.
    if (r < 3)
        return 0;
    if (whichRoot >= 2 * r)
        return 0;
    if (gcd(r, whichRoot) > 1)
        return 0;

    if (prec<65) {
    	// Set up our initial data.
		InitialData<floating> init(r, whichRoot);

		InitialData<floating>::TVType ans;
		switch (alg) {
		    case ALG_BACKTRACK:
		        ans = turaevViroBacktrack(*this, init, nullptr);
		        break;
		    case ALG_NAIVE:
		        ans = turaevViroNaive(*this, init, nullptr);
		        break;
		    default:
		        ans = turaevViroTreewidth(*this, init, nullptr);
		        break;
		}
		/*
		 * Disable this check for now, since testing whether img(z) == 0 is
		 * error-prone due to floating-point approximation.
		 *
		if (isNonZero(ans.imag())) {
		    // This should never happen, since the Turaev-Viro invariant is the
		    // square of the modulus of the Witten invariant for sl_2.
		    std::cerr <<
		        "WARNING: The Turaev-Viro invariant has an imaginary component.\n"
		        "         This should never happen.\n"
		        "         Please report this (along with the 3-manifold that"
		        "         was used) to Regina's authors." << std::endl;
		}
		 */
		return ans.real();
	} else {
		// Set up our initial data.
		MFloat::setPrec(prec);
		InitialData<multiPrecision> init(r, whichRoot);

		double ans;
		switch (alg) {
		    case ALG_BACKTRACK:
		        ans = MFloat::extractDouble(turaevViroBacktrack(*this, init, 0));
		        break;
		    case ALG_NAIVE:
		        ans = MFloat::extractDouble(turaevViroNaive(*this, init, 0));
		        break;
		    default:
		        ans = MFloat::extractDouble(turaevViroTreewidth(*this, init, 0));
		        break;
		}
		init.clearData();
		MFloat::freeCache();
		std::cout.flush();
		return ans;
	}
	
}

Cyclotomic Triangulation<3>::turaevViro(unsigned long r, bool parity,
        Algorithm alg, ProgressTracker* tracker) const {
    // Do some basic parameter checks.
    if (r < 3) {
        if (tracker)
            tracker->setFinished();
        return Cyclotomic();
    }
    if (r % 2 == 0)
        parity = false; // As required by allCalculatedTuraevViroInvariants().

    // Have we already calculated this invariant?
    std::pair<unsigned long, bool> tvParams(r, parity);
#ifndef TV_IGNORE_CACHE
    auto it = prop_.turaevViroCache_.find(tvParams);
    if (it != prop_.turaevViroCache_.end()) {
        if (tracker)
            tracker->setFinished();
        return (*it).second;
    }
#endif

    // Set up our initial data.
    InitialData<exact> init(r, (parity ? 1 : 0));

    InitialData<exact>::TVType ans;
    switch (alg) {
        case ALG_BACKTRACK:
            ans = turaevViroBacktrack(*this, init, tracker);
            break;
        case ALG_NAIVE:
            ans = turaevViroNaive(*this, init, tracker);
            break;
        default:
            ans = turaevViroTreewidth(*this, init, tracker);
            break;
    }

    if (tracker) {
        if (tracker->isCancelled()) {
            tracker->setFinished();
            return Cyclotomic();
        } else {
            prop_.turaevViroCache_[tvParams] = ans;
            tracker->setFinished();
            return ans;
        }
    } else
        return (prop_.turaevViroCache_[tvParams] = std::move(ans));
}

} // namespace regina

