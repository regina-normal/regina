
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

/*! \file census/nfacepairing.h
 *  \brief Deals with pairing off tetrahedron faces in a census of
 *  3-manifold triangulations.
 */

#ifndef __NFACEPAIRING_H
#ifndef __DOXYGEN
#define __NFACEPAIRING_H
#endif

#include "regina-core.h"
#include "census/ngenericfacetpairing.h"
#include "triangulation/nisomorphism.h"

namespace regina {

class NFacePair;

/**
 * \weakgroup census
 * @{
 */

/**
 * A legacy typedef provided for backward compatibility only.
 *
 * \deprecated As of Regina 4.94, this typedef is now available as
 * NFacePairing::IsoList.  The old typedef NFacePairingIsoList is
 * provided for backward compatibility, but will be removed in some
 * future version of Regina.
 */
typedef std::list<NIsomorphism*> NFacePairingIsoList;

/**
 * A legacy typedef provided for backward compatibility only.
 *
 * \deprecated As of Regina 4.94, this typedef is now available as
 * NFacePairing::Use.  The old typedef UseFacePairing is
 * provided for backward compatibility, but will be removed in some
 * future version of Regina.
 */
typedef void (*UseFacePairing)(const NFacePairing*, const NFacePairingIsoList*,
    void*);

/**
 * Represents a specific pairwise matching of tetrahedron faces.
 * Given a fixed number of tetrahedra, each tetrahedron face is either
 * paired with some other tetrahedron face (which is in turn paired with
 * it) or remains unmatched.  A tetrahedron face cannot be paired with
 * itself.
 *
 * Such a matching models part of the structure of a triangulation, in
 * which each tetrahedron face is either glued to some other tetrahedron
 * face (which is in turn glued to it) or is an unglued boundary face.
 *
 * Note that if this pairing is used to construct an actual
 * triangulation, the individual gluing permutations will still need to
 * be specified; they are not a part of this structure.
 *
 * \testpart
 */
class REGINA_API NFacePairing : public NGenericFacetPairing<3> {
    public:
        /**
         * Creates a new face pairing that is a clone of the given face
         * pairing.
         *
         * @param cloneMe the face pairing to clone.
         */
        NFacePairing(const NFacePairing& cloneMe);

        /**
         * Creates the face pairing of the given 3-manifold triangulation.
         * This is the face pairing that describes how the tetrahedron faces
         * of the given triangulation are joined together, as described in the
         * class notes.
         *
         * \pre The given triangulation is not empty.
         *
         * @param tri the triangulation whose face pairing should be
         * constructed.
         */
        NFacePairing(const NTriangulation& tri);

        /**
         * A legacy alias for size(), provided for backward
         * compatibility only.
         *
         * This routine returns the number of tetrahedra whose faces are
         * described by this face pairing.
         *
         * \deprecated As of Regina 4.94, this routine has been renamed
         * to size().  The old name getNumberOfTetrahedra() is provided
         * for backward compatibility, but will be removed in some future
         * version of Regina.
         *
         * @return the number of tetrahedra under consideration.
         */
        unsigned getNumberOfTetrahedra() const;

        /**
         * Follows a chain as far as possible from the given point.
         *
         * A chain is the underlying face pairing for a layered chain;
         * specifically it involves one tetrahedron joined to a second
         * along two faces, the remaining two faces of the second
         * tetrahedron joined to a third and so on.  A chain can involve
         * as few as one tetrahedron or as many as we like.  Note that
         * the remaining two faces of the first tetrahedron and the
         * remaining two faces of the final tetrahedron remain
         * unaccounted for by this structure.
         *
         * This routine begins with two faces of a given tetrahedron,
         * described by parameters \a tet and \a faces.
         * If these two faces are both joined to some different
         * tetrahedron, parameter \a tet will be changed to this new
         * tetrahedron and parameter \a faces will be changed to the
         * remaining faces of this new tetrahedron (i.e., the two faces
         * that were not joined to the original faces of the original
         * tetrahedron).
         *
         * This procedure is repeated as far as possible until either
         * the two faces in question join to two different tetrahedra,
         * the two faces join to each other, or at least one of the
         * two faces is unmatched.
         *
         * Thus, given one end of a chain, this procedure can be used to
         * follow the face pairings through to the other end of the chain.
         *
         * \warning You must be sure when calling this routine that you
         * are not inside a chain that loops back onto itself!
         * If the face pairing forms a
         * large loop with each tetrahedron joined by two faces to the
         * next, this routine will cycle around the loop forever and
         * never return.
         *
         * @param tet the index in the underlying triangulation of the
         * tetrahedron to begin at.  This parameter will be modified
         * directly by this routine as a way of returning the results.
         * @param faces the pair of face numbers in the given
         * tetrahedron at which we begin.  This parameter will also be
         * modified directly by this routine as a way of returning results.
         */
        void followChain(unsigned& tet, NFacePair& faces) const;

        /**
         * Determines whether this face pairing contains a triple edge.
         * A triple edge is where two different tetrahedra are joined
         * along three of their faces.
         *
         * A face pairing containing a triple edge cannot model a closed
         * minimal irreducible P^2-irreducible 3-manifold triangulation on
         * more than two tetrahedra.  See "Face pairing graphs and 3-manifold
         * enumeration", Benjamin A. Burton, J. Knot Theory Ramifications
         * 13 (2004), 1057--1101.
         *
         * @return \c true if and only if this face pairing contains a
         * triple edge.
         */
        bool hasTripleEdge() const;

        /**
         * Determines whether this face pairing contains a broken
         * double-ended chain.
         *
         * A chain involves a sequence of tetrahedra, each joined to the
         * next along two faces, and is described in detail in the
         * documentation for followChain().
         *
         * A one-ended chain is a chain in which the first tetrahedron
         * is also joined to itself along one face (i.e., the underlying
         * face pairing for a layered solid torus).  A double-ended
         * chain is a chain in which the first tetrahedron is joined to
         * itself along one face and the final tetrahedron is also
         * joined to itself along one face (i.e., the underlying
         * face pairing for a layered lens space).
         *
         * A broken double-ended chain consists of two one-ended chains
         * (using distinct sets of tetrahedra) joined together along one
         * face.  The remaining two faces (one from each chain)
         * that were unaccounted for by the individual one-ended chains
         * remain unaccounted for by this broken double-ended chain.
         *
         * In this routine we are interested specifically in finding a
         * broken double-ended chain that is not a part of a complete
         * double-ended chain, i.e., the final two unaccounted faces are
         * not joined together.
         *
         * A face pairing containing a broken double-ended chain cannot
         * model a closed minimal irreducible P^2-irreducible 3-manifold
         * triangulation on more than two tetrahedra.  See "Face pairing
         * graphs and 3-manifold enumeration", Benjamin A. Burton,
         * J. Knot Theory Ramifications 13 (2004), 1057--1101.
         *
         * @return \c true if and only if this face pairing contains a
         * broken double-ended chain that is not part of a complete
         * double-ended chain.
         */
        bool hasBrokenDoubleEndedChain() const;

        /**
         * Determines whether this face pairing contains a one-ended chain
         * with a double handle.
         *
         * A chain involves a sequence of tetrahedra, each joined to the
         * next along two faces, and is described in detail in the
         * documentation for followChain().
         *
         * A one-ended chain is a chain in which the first tetrahedron
         * is also joined to itself along one face (i.e., the underlying
         * face pairing for a layered solid torus).
         *
         * A one-ended chain with a double handle begins with a one-ended
         * chain.  The two faces that are unaccounted for by this
         * one-ended chain must be joined
         * to two different tetrahedra, and these two tetrahedra must be
         * joined to each other along two faces.  The remaining two
         * faces of these two tetrahedra remain unaccounted for by this
         * structure.
         *
         * A face pairing containing a one-ended chain with a double handle
         * cannot model a closed minimal irreducible P^2-irreducible
         * 3-manifold triangulation on more than two tetrahedra.  See
         * "Face pairing graphs and 3-manifold enumeration",
         * Benjamin A. Burton, J. Knot Theory Ramifications 13 (2004),
         * 1057--1101.
         *
         * @return \c true if and only if this face pairing contains a
         * one-ended chain with a double handle.
         */
        bool hasOneEndedChainWithDoubleHandle() const;

        /**
         * Determines whether this face pairing contains a wedged
         * double-ended chain.
         *
         * A chain involves a sequence of tetrahedra, each joined to the
         * next along two faces, and is described in detail in the
         * documentation for followChain().
         *
         * A one-ended chain is a chain in which the first tetrahedron
         * is also joined to itself along one face (i.e., the underlying
         * face pairing for a layered solid torus).  A double-ended
         * chain is a chain in which the first tetrahedron is joined to
         * itself along one face and the final tetrahedron is also
         * joined to itself along one face (i.e., the underlying
         * face pairing for a layered lens space).
         *
         * A wedged double-ended chain is created from two one-ended
         * chains as follows.  Two new tetrahedra are added, and each
         * one-ended chain is joined to each of the new tetrahedra along
         * a single face.  In addition, the two new tetrahedra are
         * joined to each other along a single face.  The remaining two
         * faces (one from each of the new tetrahedra) remain
         * unaccounted for by this structure.
         *
         * An alternative way of viewing a wedged double-ended chain is
         * as an ordinary double-ended chain, where one of the internal
         * tetrahedra is removed and replaced with a pair of tetrahedra
         * joined to each other.  Whereas the original tetrahedron met its
         * two neighbouring tetrahedra along two faces each (giving a
         * total of four face identifications), the two new tetrahedra now
         * meet each of the two neighbouring tetrahedra along a single
         * face each (again giving four face identifications).
         *
         * Note that if this alternate construction is used to replace
         * one of the end tetrahedra of the double-ended chain (not an
         * internal tetrahedron), the resulting structure will either be
         * a triple edge or a one-ended chain with a double handle
         * (according to whether the original chain has zero or positive
         * length).  See hasTripleEdge() and
         * hasOneEndedChainWithDoubleHandle() for further details.
         *
         * A face pairing containing a wedged double-ended chain
         * cannot model a closed minimal irreducible P^2-irreducible
         * 3-manifold triangulation on more than two tetrahedra.  See
         * "Enumeration of non-orientable 3-manifolds using face-pairing
         * graphs and union-find", Benjamin A. Burton,
         * Discrete Comput. Geom. 38 (2007), no. 3, 527--571.
         *
         * @return \c true if and only if this face pairing contains a
         * wedged double-ended chain.
         */
        bool hasWedgedDoubleEndedChain() const;

        /**
         * Determines whether this face pairing contains a one-ended
         * chain with a stray bigon.
         *
         * A chain involves a sequence of tetrahedra, each joined to the
         * next along two faces, and is described in detail in the
         * documentation for followChain().
         *
         * A one-ended chain is a chain in which the first tetrahedron
         * is also joined to itself along one face (i.e., the underlying
         * face pairing for a layered solid torus).
         *
         * A one-ended chain with a stray bigon describes the following
         * structure.  We begin with a one-ended chain.  Two new
         * tetrahedra are added; these are joined to each other along
         * two pairs of faces, and one of the new tetrahedra is joined
         * to the end of the one-ended chain.  We then ensure that:
         * - This configuration is not part of a longer one-ended chain
         *   that encompasses all of the aforementioned tetrahedra;
         * - There is no extra tetrahedron that is joined to both the
         *   two new tetrahedra and the end of the chain;
         * - There is no extra tetrahedron that is joined to the end of
         *   the chain along one face and the far new tetrahedron along
         *   two additional faces (where by "the far new tetrahedron"
         *   we mean the new tetrahedron that was not originally joined to
         *   the chain).
         *
         * Aside from these constraints, the remaining four tetrahedron faces
         * (two faces of the far new tetrahedron, one face of the other new
         * tetrahedron, and one face at the end of the chain) remain
         * unaccounted for by this structure.
         *
         * A face pairing containing a structure of this type
         * cannot model a closed minimal irreducible P^2-irreducible
         * 3-manifold triangulation on more than two tetrahedra.  See
         * "Enumeration of non-orientable 3-manifolds using face-pairing
         * graphs and union-find", Benjamin A. Burton,
         * Discrete Comput. Geom. 38 (2007), no. 3, 527--571.
         *
         * @return \c true if and only if this face pairing contains a
         * one-ended chain with a stray bigon.
         */
        bool hasOneEndedChainWithStrayBigon() const;

        /**
         * Determines whether this face pairing contains a triple
         * one-ended chain.
         *
         * A chain involves a sequence of tetrahedra, each joined to the
         * next along two faces, and is described in detail in the
         * documentation for followChain().
         *
         * A one-ended chain is a chain in which the first tetrahedron
         * is also joined to itself along one face (i.e., the underlying
         * face pairing for a layered solid torus).
         *
         * A triple one-ended chain is created from three one-ended
         * chains as follows.  Two new tetrahedra are added, and each
         * one-ended chain is joined to each of the new tetrahedra along
         * a single face.  The remaining two faces (one from each of the
         * new tetrahedra) remain unaccounted for by this structure.
         *
         * A face pairing containing a triple one-ended chain
         * cannot model a closed minimal irreducible P^2-irreducible
         * 3-manifold triangulation on more than two tetrahedra.  See
         * "Enumeration of non-orientable 3-manifolds using face-pairing
         * graphs and union-find", Benjamin A. Burton,
         * Discrete Comput. Geom. 38 (2007), no. 3, 527--571.
         *
         * @return \c true if and only if this face pairing contains a
         * triple one-ended chain.
         */
        bool hasTripleOneEndedChain() const;

        /**
         * Determines whether this face pairing contains a single-edged
         * star.
         *
         * A single-edged star involves two tetrahedra that are adjacent
         * along a single face, where the six remaining faces of these
         * tetrahedra are joined to six entirely new tetrahedra (so that
         * none of the eight tetrahedra described in this structure are
         * the same).
         *
         * @return \c true if and only if this face pairing contains a
         * single-edged star.
         */
        bool hasSingleStar() const;

        /**
         * Determines whether this face pairing contains a double-edged
         * star.
         *
         * A double-edged star involves two tetrahedra that are adjacent
         * along two separate pairs of faces, where the four remaining
         * faces of these tetrahedra are joined to four entirely new
         * tetrahedra (so that none of the six tetrahedra described in
         * this structure are the same).
         *
         * @return \c true if and only if this face pairing contains a
         * double-edged star.
         */
        bool hasDoubleStar() const;

        /**
         * Determines whether this face pairing contains a double-edged
         * square.
         *
         * A double-edged square involves four distinct tetrahedra that
         * meet each other as follows.  Two pairs of tetrahedra are
         * joined along two pairs of faces each.  Then each tetrahedron
         * is joined along a single face to one tetrahedron of the other
         * pair.  The four tetrahedron faces not yet joined to anything
         * (one from each tetrahedron) remain unaccounted for by this
         * structure.
         *
         * @return \c true if and only if this face pairing contains a
         * double-edged square.
         */
        bool hasDoubleSquare() const;

    private:
        /**
         * Creates a new face pairing.  All internal arrays will be
         * allocated but not initialised.
         *
         * \pre \a nTetrahedra is at least 1.
         *
         * @param nTetrahedra the number of tetrahedra under
         * consideration in this new face pairing.
         */
        NFacePairing(unsigned nTetrahedra);

        /**
         * Internal to hasBrokenDoubleEndedChain().  This routine assumes
         * that the give face of the given tetrahedron is glued to this
         * same tetrahedron, and attempts to find a broken double-ended
         * chain for which this tetrahedron is at the end of one of the
         * one-ended chains therein.
         *
         * \pre The given face of the given tetrahedron is paired with
         * another face of the same tetrahedron under this face pairing.
         *
         * @param tet the index in the triangulation of the given
         * tetrahedron.
         * @param face the number of the given face in the tetrahedron;
         * this must be between 0 and 3 inclusive.
         *
         * @return \c true if and only if this face pairing contains a
         * broken double-ended chain as described above.
         */
        bool hasBrokenDoubleEndedChain(unsigned tet, unsigned face) const;

        /**
         * Internal to hasOneEndedChainWithDoubleHandle().  This routine
         * assumes that the give face of the given tetrahedron is glued
         * to this same tetrahedron, and attempts to find a one-ended
         * chain with a double handle for which this tetrahedron is at
         * the end of the chain contained therein.
         *
         * \pre The given face of the given tetrahedron is paired with
         * another face of the same tetrahedron under this face pairing.
         *
         * @param tet the index in the triangulation of the given
         * tetrahedron.
         * @param face the number of the given face in the tetrahedron;
         * this must be between 0 and 3 inclusive.
         *
         * @return \c true if and only if this face pairing contains a
         * one-ended chain with a double handle as described above.
         */
        bool hasOneEndedChainWithDoubleHandle(unsigned tet,
            unsigned face) const;

        /**
         * Internal to hasWedgedDoubleEndedChain().  This routine assumes
         * that the give face of the given tetrahedron is glued to this
         * same tetrahedron, and attempts to find a wedged double-ended
         * chain for which this tetrahedron is at one end of the
         * double-ended chain.
         *
         * \pre The given face of the given tetrahedron is paired with
         * another face of the same tetrahedron under this face pairing.
         *
         * @param tet the index in the triangulation of the given
         * tetrahedron.
         * @param face the number of the given face in the tetrahedron;
         * this must be between 0 and 3 inclusive.
         *
         * @return \c true if and only if this face pairing contains a
         * wedged double-ended chain as described above.
         */
        bool hasWedgedDoubleEndedChain(unsigned tet, unsigned face) const;

        /**
         * Internal to hasOneEndedChainWithStrayBigon().  This routine assumes
         * that the give face of the given tetrahedron is glued to this
         * same tetrahedron, and attempts to find a one-ended chain with
         * stray bigon for which this tetrahedron is at the end of the
         * one-ended chain.
         *
         * \pre The given face of the given tetrahedron is paired with
         * another face of the same tetrahedron under this face pairing.
         *
         * @param tet the index in the triangulation of the given
         * tetrahedron.
         * @param face the number of the given face in the tetrahedron;
         * this must be between 0 and 3 inclusive.
         *
         * @return \c true if and only if this face pairing contains a
         * one-ended chain with stray bigon as described above.
         */
        bool hasOneEndedChainWithStrayBigon(unsigned tet, unsigned face) const;

        /**
         * Internal to hasTripleOneEndedChain().  This routine assumes
         * that the give face of the given tetrahedron is glued to this
         * same tetrahedron, and attempts to find a triple one-ended
         * chain for which this tetrahedron is at the end of one of the
         * individual one-ended chains.
         *
         * \pre The given face of the given tetrahedron is paired with
         * another face of the same tetrahedron under this face pairing.
         *
         * @param tet the index in the triangulation of the given
         * tetrahedron.
         * @param face the number of the given face in the tetrahedron;
         * this must be between 0 and 3 inclusive.
         *
         * @return \c true if and only if this face pairing contains a
         * triple one-ended chain as described above.
         */
        bool hasTripleOneEndedChain(unsigned tet, unsigned face) const;

    // Make sure the parent class can call the private constructor.
    friend class NGenericFacetPairing<3>;
};

/*@}*/

// Inline functions for NFacePairing

inline NFacePairing::NFacePairing(const NFacePairing& cloneMe) :
        NGenericFacetPairing<3>(cloneMe) {
}

inline NFacePairing::NFacePairing(const NTriangulation& tri) :
        NGenericFacetPairing<3>(tri) {
}

inline NFacePairing::NFacePairing(unsigned nTetrahedra) :
        NGenericFacetPairing<3>(nTetrahedra) {
}

inline unsigned NFacePairing::getNumberOfTetrahedra() const {
    return size_;
}

} // namespace regina

#endif

