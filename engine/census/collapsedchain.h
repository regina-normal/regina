
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

/*! \file census/collapsedchain.h
 *  \brief Supports searching through all possible sets of tetrahedron
 *  gluing permutations for a given tetrahedron face pairing.
 */

#ifndef __COLLAPSEDCHAIN_H
#ifndef __DOXYGEN
#define __COLLAPSEDCHAIN_H
#endif

#include "ngluingpermsearcher.h"

#include <unordered_set>

namespace regina {

/**
 * A gluing permutation search class that offers a specialised search
 * algorithm for when (i) only closed prime minimal P2-irreducible
 * triangulations are required, (ii) the given face pairing has
 * order at least three, and (iii) the given face pairing has a chain of length
 * at least one.
 *
 * The search algorithm is significantly different from the default
 * algorithm provided by NGluingPermSearcher.  It is heavily optimised
 * and takes advantage of a number of results regarding the underlying
 * face pairing graph.
 *
 * Note that additional unwanted triangulations (e.g., non-prime or
 * non-minimal triangulations) may still be produced by this search.
 * However, significantly fewer unwanted triangulations will be produced
 * when using this class instead of NGluingPermSearcher.
 *
 * \ifacespython Not present.
 */
class REGINA_API CollapsedChainSearcher : public NGluingPermSearcher {
    public:
        static const char dataTag_;
            /**< A character used to identify this class when reading
                 and writing tagged data in text format. */

    private:
        EnumerationDB *enumDB;
            /**< A database of enumeration results sorted by face pairing
                 graph. If possible, this searcher will use these results after
                 a collapse rather than rebuilding smaller triangulations. */
        NFacePairing *modified;
            /**< The modified face pairing graph. */
        NIsomorphism *iso;
            /**< The isomorphism applied to the original pairing to retrieve
                 create modified.

                 Note that since modified contains fewer simplices than the
                 original face pairing, these extra simplices will have to be
                 added back in first. Then the isomorphism can be applied, and
                 lastly the chains can be rebuilt. */
        NIsomorphism *isoInv;
            /**< The isomorphism to apply to modified to retrieve the original
                 labels of all simplices in modified.

                 Note that since modified contains fewer simplices than the
                 original face pairing, these extra simplices will have to be
                 added back in first. Then the isomorphism can be applied, and
                 lastly the chains can be rebuilt. */
        std::list<NIsomorphism*> automorphs;
        std::list<NIsomorphism*> stabilizers;
        struct IsoComp {
            bool operator()(const NIsomorphism* lhs, const NIsomorphism* rhs)
                const {
                if (lhs->size() != rhs->size())
                    return lhs->size() < rhs->size();
                for(int simp=0; simp < lhs->size(); ++simp) {
                    if (lhs->simpImage(simp) != rhs->simpImage(simp))
                        return lhs->simpImage(simp) < rhs->simpImage(simp);
                    if (lhs->facetPerm(simp) != rhs->facetPerm(simp))
                        return lhs->facetPerm(simp) < rhs->facetPerm(simp);
                }
                return false; // are identical
            }
        };


        std::set<NIsomorphism*,IsoComp> automorphsDone;
        NTetFace *chainEnds;
        enum EdgeType { EDGE_CHAIN_END, EDGE_CHAIN_INTERNAL_FIRST,
            EDGE_CHAIN_INTERNAL_SECOND };
        EdgeType *orderType;
        int *chainNo;
        int numChains;
        bool *chainSym;
        bool *shortChain;
        bool collapse;
        bool empty;
        unsigned maxOrder;
        int* chainPermIndices;
            /**< Stores the two possible gluing permutations that must be
                 tried for each face in the order[] array.

                 Note that for each face, the gluing permutation can be derived
                 from the permutation chosen for the previous face.  In this
                 case we store the two permutations for this face that
                 correspond to the two possible permutations for the previous
                 face.  */

    public:
        /**
         * Creates a new search manager for use when (i) only closed prime
         * minimal P2-irreducible triangulations are required, (ii) the
         * given face pairing has order at least three, and (iii) the given
         * face pairing contains a chain of length at least one. Note that
         * other unwanted triangulations may still be produced (e.g., non-prime
         * or non-minimal triangulations), but there will be far fewer of these
         * than when using the NGluingPermSearcher class directly.
         *
         * For details on how a search manager is used, see the
         * NGluingPermSearcher documentation.  Note in particular that
         * this class will be automatically used by
         * NGluingPermSearcher::findAllPerms() if possible, so there is
         * often no need for an end user to instantiate this class
         * directly.
         *
         * All constructor arguments are the same as for the
         * NGluingPermSearcher constructor, though some arguments (such as
         * \a finiteOnly and \a whichPurge) are not needed here since they
         * are already implied by the specialised search context.
         *
         * \pre The given face pairing is connected, i.e., it is possible
         * to reach any tetrahedron from any other tetrahedron via a
         * series of matched face pairs.
         * \pre The given face pairing is in canonical form as described
         * by NFacePairing::isCanonical().  Note that all face pairings
         * constructed by NFacePairing::findAllPairings() are of this form.
         * \pre The given face pairing has no boundary faces and has at
         * least three tetrahedra.
         */
        CollapsedChainSearcher(const NFacePairing* pairing,
                const NFacePairing::IsoList* autos, bool orientableOnly,
                char* enumDBPath, UseGluingPerms use, void* useArgs = 0);

        /**
         * Initialises a new search manager based on data read from the
         * given input stream.  This may be a new search or a partially
         * completed search.
         *
         * This routine reads data in the format written by dumpData().
         * If you wish to read data whose precise class is unknown,
         * consider using dumpTaggedData() and readTaggedData() instead.
         *
         * If the data found in the input stream is invalid or incorrectly
         * formatted, the routine inputError() will return \c true but
         * the contents of this object will be otherwise undefined.
         *
         * The arguments \a use and \a useArgs are the same as for the
         * NGluingPermSearcher constructor.
         *
         * \warning The data format is liable to change between Regina
         * releases.  Data in this format should be used on a short-term
         * temporary basis only.
         *
         * @param in the input stream from which to read.
         */
        CollapsedChainSearcher(std::istream& in,
            UseGluingPerms use, void* useArgs = 0);

        /**
         * Destroys this search manager and all supporting data
         * structures.
         */
        virtual ~CollapsedChainSearcher();

        // Overridden methods:
        virtual void dumpData(std::ostream& out) const;
        virtual void runSearch(long maxDepth = -1);

        // Helper to call member function from static function
        static void extendTriHelper(const NGluingPermSearcher *, void *);
        // Copy gluings from NGluingPermSearcher* to this
        void extendTri(const NGluingPermSearcher *);
        // Copy gluings from NTriangulation* to this
        void extendTri(const NTriangulation *);
        // Build up all possible triangulations with the pre-existing base.
        // Note: Clears out permIndices_ before returning.
        void buildUp();
    protected:
        // Overridden methods:
        virtual char dataTag() const;

    private:
        /**
         * Collapse the chain on the given pair of faces. Note that collapsing
         * here does not remove the tetrahedra, it only disconnects them. They
         * will be removed when the resulting NFacePairing is made canonical.
         * Note that chains of length 0 (so a tetrahedron with two faces
         * identified together and remaining two faces identified with two
         * other distinct tetrahedra) will not be collapsed.
         *
         * Returns true if a chain was collapsed, false otherwise (most likely
         * due to finding a short chain).
         */
        bool collapseChain(NFacePair pair, int tet);

};


// Inline functions for CollapsedChainSearcher

inline char CollapsedChainSearcher::dataTag() const {
    return CollapsedChainSearcher::dataTag_;
}

}; // namespace

#endif /* COLLAPSEDCHAIN_H */

