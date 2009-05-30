
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

/*! \file census/dim4gluingperms.h
 *  \brief Deals with selecting gluing permutations to complement a
 *  particular pairing of pentachoron facets.
 */

#ifndef __DIM4GLUINGPERMS_H
#ifndef __DOXYGEN
#define __DIM4GLUINGPERMS_H
#endif

#include "census/dim4facetpairing.h"
#include "maths/permconv.h"

namespace regina {

class Dim4GluingPerms;
class Dim4Triangulation;

/**
 * \weakgroup census
 * @{
 */

/**
 * Represents a specific set of gluing permutations to complement a
 * particular pairwise matching of pentachoron facets.  Given a
 * pairwise matching of facets (as described by class Dim4FacetPairing), each
 * facet that is matched with some other facet will have an associated
 * permutation of five elements (as described by class NPerm5).
 *
 * If a facet is matched with some other facet, the two associated
 * permutations in this set will be inverses.  If a facet is left
 * deliberately unmatched, it will have no associated permutation in this set.
 *
 * Such a set of permutations models part of the structure of a 4-manifold
 * triangulation, in which each pentachoron facet that is glued to another
 * facet has a corresponding gluing permutation (and the matched facet has
 * the inverse gluing permutation).
 *
 * \ifacespython Not present.
 */
class Dim4GluingPerms {
    protected:
        const Dim4FacetPairing* pairing_;
            /**< The facet pairing that this permutation set complements.
                 This is guaranteed to be the minimal representative of
                 its facet pairing isomorphism class. */
        int* permIndices_;
            /**< The index into array NPerm::S4 describing how each
                 pentachoron facet is glued to its partner.  Note that this
                 is not a gluing permutation as such but rather a permutation
                 of 0, 1, 2 and 3 only (see the routines gluingToIndex() and
                 indexToGluing() for conversions).  If a permutation has not
                 yet been selected (e.g., if this permutation set is still
                 under construction) then this index is -1. */

        bool inputError_;
            /**< Has an error occurred during construction from an
                 input stream? */

    public:
        /**
         * Creates a new set of gluing permutations that is a clone of
         * the given permutation set.
         *
         * @param cloneMe the gluing permutations to clone.
         */
        Dim4GluingPerms(const Dim4GluingPerms& cloneMe);

        /**
         * Reads a new set of gluing permutations from the given input
         * stream.  This routine reads data in the format written by
         * dumpData().
         *
         * If the data found in the input stream is invalid or
         * incorrectly formatted, the routine inputError() will return
         * \c true but the contents of this object will be otherwise
         * undefined.
         *
         * \warning The data format is liable to change between
         * Regina releases.  Data in this format should be used on a
         * short-term temporary basis only.
         *
         * @param in the input stream from which to read.
         */
        Dim4GluingPerms(std::istream& in);

        /**
         * Deallocates any memory used by this structure.
         */
        virtual ~Dim4GluingPerms();

        /**
         * Was an error found during construction from an input stream?
         *
         * This routine returns \c true if an input stream constructor was
         * used to create this object but the data in the input stream
         * was invalid or incorrectly formatted.
         *
         * If a different constructor was called (i.e., no input stream
         * was used), then this routine will always return \c false.
         *
         * @return \c true if an error occurred during construction from
         * an input stream, or \c false otherwise.
         */
        bool inputError() const;

        /**
         * Returns the total number of pentachora under consideration.
         *
         * @return the number of pentachora under consideration.
         */
        unsigned getNumberOfPentachora() const;

        /**
         * Returns the specific pairing of pentachoron facets that this
         * set of gluing permutations complements.
         *
         * @return the corresponding pentachoron facet pairing.
         */
        const Dim4FacetPairing* getFacetPairing() const;

        /**
         * Returns the gluing permutation associated with the given
         * pentachoron facet.
         *
         * \pre The given facet is actually paired with some other facet in
         * the underlying pairwise matching (see routine getFacetPairing()).
         * \pre The given facet is a real pentachoron
         * facet (not boundary, before-the-start or past-the-end).
         *
         * @param source the pentachoron facet under investigation.
         * @return the associated gluing permutation.
         */
        NPerm5 gluingPerm(const Dim4PentFacet& source) const;

        /**
         * Returns the gluing permutation associated with the given
         * pentachoron facet.
         *
         * \pre The given facet is actually paired with some other facet in
         * the underlying pairwise matching (see routine getFacetPairing()).
         *
         * @param pent the pentachoron under investigation (this must be
         * strictly less than the total number of pentachora under
         * consideration).
         * @param facet the facet of the given pentachoron under
         * investigation (between 0 and 4 inclusive).
         * @return the associated gluing permutation.
         */
        NPerm5 gluingPerm(unsigned pent, unsigned facet) const;

        /**
         * Returns a newly created 4-manifold triangulation as modelled
         * by this set of gluing permutations and the associated pentachoron
         * facet pairing.
         *
         * Each matched pair of facets and their associated permutations
         * will be realised as two pentachoron facets glued
         * together with the corresponding gluing permutation.  Each
         * unmatched facet will be realised as a boundary pentachoron facet.
         *
         * It is the responsibility of the caller of this routine to
         * delete this triangulation once it is no longer required.
         *
         * @return a newly created 4-manifold triangulation modelled by
         * this structure.
         */
        Dim4Triangulation* triangulate() const;

        /**
         * Dumps all internal data in a plain text format to the given
         * output stream.  This object can be recreated from this text
         * data by calling the input stream constructor for this class.
         *
         * This routine may be useful for transferring objects from
         * one processor to another.
         *
         * Note that subclass data is written after superclass data, so
         * it is safe to dump data from a subclass and then recreate a
         * new superclass object from that data (though subclass-specific
         * information will of course be lost).
         *
         * \warning The data format is liable to change between
         * Regina releases.  Data in this format should be used on a
         * short-term temporary basis only.
         *
         * @param out the output stream to which the data should be
         * written.
         */
        virtual void dumpData(std::ostream& out) const;

    protected:
        /**
         * Creates a new permutation set.  All internal arrays will be
         * allocated but not initialised.
         *
         * \pre The given facet pairing is connected, i.e., it is possible
         * to reach any pentachoron from any other pentachoron via a
         * series of matched facet pairs.
         * \pre The given facet pairing is in canonical form as described
         * by Dim4FacetPairing::isCanonical().  Note that all facet pairings
         * constructed by Dim4FacetPairing::findAllPairings() are of this form.
         *
         * @param pairing the specific pairing of pentachoron facets
         * that this permutation set will complement.
         */
        Dim4GluingPerms(const Dim4FacetPairing* pairing);

        /**
         * Returns the index into array NPerm::S4 describing how the
         * the given facet is joined to its partner.
         *
         * Note that this permutation is not a gluing permutation as such,
         * but rather a permutation of 0, 1, 2 and 3 only.  For a real
         * facet gluing permutation, see routine gluingPerm().
         *
         * \pre The given facet is a real pentachoron
         * facet (not boundary, before-the-start or past-the-end).
         *
         * @param source the pentachoron facet under investigation.
         * @return a reference to the corresponding array index.
         */
        int& permIndex(const Dim4PentFacet& source);

        /**
         * Returns the index into array NPerm::S4 describing how the
         * the given facet is joined to its partner.
         *
         * Note that this permutation is not a gluing permutation as such,
         * but rather a permutation of 0, 1, 2 and 3 only.  For a real
         * facet gluing permutation, see routine gluingPerm().
         *
         * @param pent the pentachoron under investigation (this must be
         * strictly less than the total number of pentachora under
         * consideration).
         * @param facet the facet of the given pentachoron under
         * investigation (between 0 and 4 inclusive).
         * @return a reference to the corresponding array index.
         */
        int& permIndex(unsigned pent, unsigned facet);

        /**
         * Returns the index into array NPerm::S4 describing how the
         * the given facet is joined to its partner.
         *
         * Note that this permutation is not a gluing permutation as such,
         * but rather a permutation of 0, 1, 2 and 3 only.  For a real
         * facet gluing permutation, see routine gluingPerm().
         *
         * \pre The given facet is a real pentachoron
         * facet (not boundary, before-the-start or past-the-end).
         *
         * @param source the pentachoron facet under investigation.
         * @return a reference to the corresponding array index.
         */
        const int& permIndex(const Dim4PentFacet& source) const;

        /**
         * Returns the index into array NPerm::S4 describing how the
         * the given facet is joined to its partner.
         *
         * Note that this permutation is not a gluing permutation as such,
         * but rather a permutation of 0, 1, 2 and 3 only.  For a real
         * facet gluing permutation, see routine gluingPerm().
         *
         * @param pent the pentachoron under investigation (this must be
         * strictly less than the total number of pentachora under
         * consideration).
         * @param facet the facet of the given pentachoron under
         * investigation (between 0 and 4 inclusive).
         * @return a reference to the corresponding array index.
         */
        const int& permIndex(unsigned pent, unsigned facet) const;

        /**
         * Returns the index into array NPerm::S4 corresponding to
         * the given gluing permutation from the given facet to its
         * partner.  This need not be the index into NPerm::S4 that
         * is currently stored for the given facet.
         *
         * Indices into array NPerm::S4 are stored internally in the
         * array \a permIndices_.  Full gluing permutations on the other
         * hand are used in constructing 4-manifold triangulations.
         *
         * \pre The given pentachoron facet has a partner according to
         * the underlying facet pairing, i.e., is not a boundary facet.
         * \pre If the given pentachoron facet and its partner are facets
         * \a x and \a y of their respective pentachora, then the
         * given gluing permutation maps \a x to \a y.
         *
         * @param source the pentachoron facet under investigation.
         * @param gluing a possible gluing permutation from the given
         * pentachoron facet to its partner according to the underlying
         * facet pairing.
         * @return the index into NPerm::S4 corresponding to the
         * given gluing permutation; this will be between 0 and 23
         * inclusive.
         */
        int gluingToIndex(const Dim4PentFacet& source, const NPerm5& gluing)
            const;

        /**
         * Returns the index into array NPerm::S4 corresponding to
         * the given gluing permutation from the given facet to its
         * partner.  This need not be the index into NPerm::S4 that
         * is currently stored for the given facet.
         *
         * Indices into array NPerm::S4 are stored internally in the
         * array \a permIndices_.  Full gluing permutations on the other
         * hand are used in constructing 4-manifold triangulations.
         *
         * \pre The given pentachoron facet has a partner according to
         * the underlying facet pairing, i.e., is not a boundary facet.
         * \pre If the given pentachoron facet and its partner are facets
         * \a x and \a y of their respective pentachora, then the
         * given gluing permutation maps \a x to \a y.
         *
         * @param pent the pentachoron under investigation; this must be
         * strictly less than the total number of pentachora under
         * consideration.
         * @param facet the facet of the given pentachoron under
         * investigation; this must be between 0 and 4 inclusive.
         * @param gluing a possible gluing permutation from the given
         * pentachoron facet to its partner according to the underlying
         * facet pairing.
         * @return the index into NPerm::S4 corresponding to the
         * given gluing permutation; this will be between 0 and 23
         * inclusive.
         */
        int gluingToIndex(unsigned pent, unsigned facet, const NPerm5& gluing)
            const;

        /**
         * Returns the gluing permutation from the given facet to its
         * partner that corresponds to the given index into array
         * NPerm::S4.  This index into NPerm::S4 need not
         * be the index that is currently stored for the given facet.
         *
         * Indices into array NPerm::S4 are stored internally in the
         * array \a permIndices_.  Full gluing permutations on the other
         * hand are used in constructing 4-manifold triangulations.
         *
         * If the given pentachoron facet and its partner according to
         * the underlying facet pairing are facets \a x and \a y of their
         * respective pentachora, then the resulting gluing permutation
         * will map \a x to \a y.
         *
         * \pre The given pentachoron facet has a partner according to
         * the underlying facet pairing, i.e., is not a boundary facet.
         *
         * @param source the pentachoron facet under investigation.
         * @param index an index into NPerm::S4; this must be
         * between 0 and 23 inclusive.
         * @return the gluing permutation corresponding to the given
         * index into NPerm::S4.
         */
        NPerm5 indexToGluing(const Dim4PentFacet& source, int index) const;

        /**
         * Returns the gluing permutation from the given facet to its
         * partner that corresponds to the given index into array
         * NPerm::S4.  This index into NPerm::S4 need not
         * be the index that is currently stored for the given facet.
         *
         * Indices into array NPerm::S4 are stored internally in the
         * array \a permIndices_.  Full gluing permutations on the other
         * hand are used in constructing 4-manifold triangulations.
         *
         * If the given pentachoron facet and its partner according to
         * the underlying facet pairing are facets \a x and \a y of their
         * respective pentachora, then the resulting gluing permutation
         * will map \a x to \a y.
         *
         * \pre The given pentachoron facet has a partner according to
         * the underlying facet pairing, i.e., is not a boundary facet.
         *
         * @param pent the pentachoron under investigation; this must be
         * strictly less than the total number of pentachora under
         * consideration.
         * @param facet the facet of the given pentachoron under
         * investigation; this must be between 0 and 4 inclusive.
         * @param index an index into NPerm::S4; this must be
         * between 0 and 23 inclusive.
         * @return the gluing permutation corresponding to the given
         * index into NPerm::S4.
         */
        NPerm5 indexToGluing(unsigned pent, unsigned facet, int index) const;
};

/*@}*/

// Inline functions for Dim4GluingPerms

inline Dim4GluingPerms::Dim4GluingPerms(const Dim4FacetPairing* pairing) :
        pairing_(pairing),
        permIndices_(new int[pairing->getNumberOfPentachora() * 5]),
        inputError_(false) {
}

inline Dim4GluingPerms::~Dim4GluingPerms() {
    delete[] permIndices_;
}

inline bool Dim4GluingPerms::inputError() const {
    return inputError_;
}

inline unsigned Dim4GluingPerms::getNumberOfPentachora() const {
    return pairing_->getNumberOfPentachora();
}

inline const Dim4FacetPairing* Dim4GluingPerms::getFacetPairing() const {
    return pairing_;
}

inline NPerm5 Dim4GluingPerms::gluingPerm(const Dim4PentFacet& source) const {
    return indexToGluing(source, permIndex(source));
}

inline NPerm5 Dim4GluingPerms::gluingPerm(unsigned pent, unsigned facet) const {
    return indexToGluing(pent, facet, permIndex(pent, facet));
}

inline int& Dim4GluingPerms::permIndex(const Dim4PentFacet& source) {
    return permIndices_[5 * source.pent + source.facet];
}

inline int& Dim4GluingPerms::permIndex(unsigned pent, unsigned facet) {
    return permIndices_[5 * pent + facet];
}

inline const int& Dim4GluingPerms::permIndex(const Dim4PentFacet& source)
        const {
    return permIndices_[5 * source.pent + source.facet];
}

inline const int& Dim4GluingPerms::permIndex(unsigned pent, unsigned facet)
        const {
    return permIndices_[5 * pent + facet];
}

inline NPerm5 Dim4GluingPerms::indexToGluing(
        const Dim4PentFacet& source, int index) const {
    return NPerm5(pairing_->dest(source).facet, 4) *
        perm4to5(NPerm::S4[index]) * NPerm5(source.facet, 4);
}

inline NPerm5 Dim4GluingPerms::indexToGluing(unsigned pent, unsigned facet,
        int index) const {
    return NPerm5(pairing_->dest(pent, facet).facet, 4) *
        perm4to5(NPerm::S4[index]) * NPerm5(facet, 4);
}

} // namespace regina

#endif

