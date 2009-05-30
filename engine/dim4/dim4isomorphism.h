
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

/*! \file dim4/dim4isomorphism.h
 *  \brief Deals with combinatorial isomorphisms of 4-manifold triangulations.
 */

#ifndef __DIM4ISOMORPHISM_H
#ifndef __DOXYGEN
#define __DIM4ISOMORPHISM_H
#endif

#include "shareableobject.h"
#include "dim4/dim4pentfacet.h"
#include "maths/nperm5.h"

namespace regina {

class Dim4Triangulation;

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Represents a combinatorial isomorphism from one 4-manifold triangulation
 * into another.
 *
 * A combinatorial isomorphism from triangulation T to triangulation U
 * is a one-to-one map f from the individual pentachoron facets of T to
 * the individual pentachoron facets of U for which the following
 * conditions hold:
 *
 *   - if facets x and y belong to the same pentachoron of T then
 *     facets f(x) and f(y) belong to the same pentachoron of U;
 *   - if facets x and y are identified in T then facets f(x) and f(y)
 *     are identified in U.
 *
 * Isomorphisms can be <i>boundary complete</i> or
 * <i>boundary incomplete</i>.  A boundary complete isomorphism
 * satisfies the additional condition:
 *
 *   - if facet x is a boundary facet of T then facet f(x) is a boundary
 *     facet of U.
 *
 * A boundary complete isomorphism thus indicates that a copy of
 * triangulation T is present as an entire component (or components) of U,
 * whereas a boundary incomplete isomorphism represents an embedding of a
 * copy of triangulation T as a subcomplex of some possibly larger component
 * (or components) of U.
 *
 * Note that in all cases triangulation U may contain more tetrahedra
 * than triangulation T.
 */
class Dim4Isomorphism : public ShareableObject {
    protected:
        unsigned nPentachora_;
            /**< The number of pentachora in the source triangulation. */
        int* pentImage_;
            /**< The pentachoron of the destination triangulation that
                 each pentachoron of the source triangulation maps to. */
        NPerm5* facetPerm_;
            /**< The permutation applied to the five facets of each
                 source pentachoron. */

    public:
        /**
         * Creates a new isomorphism with no initialisation.
         *
         * \ifacespython Not present.
         *
         * @param sourcePentachora the number of pentachora in the source
         * triangulation associated with this isomorphism; this may be zero.
         */
        Dim4Isomorphism(unsigned sourcePentachora);
        /**
         * Creates a new isomorphism identical to the given isomorphism.
         *
         * @param cloneMe the isomorphism upon which to base the new
         * isomorphism.
         */
        Dim4Isomorphism(const Dim4Isomorphism& cloneMe);
        /**
         * Destroys this isomorphism.
         */
        ~Dim4Isomorphism();

        /**
         * Returns the number of pentachora in the source triangulation
         * associated with this isomorphism.  Note that this is always
         * less than or equal to the number of pentachora in the
         * destination triangulation.
         *
         * @return the number of pentachora in the source triangulation.
         */
        unsigned getSourcePentachora() const;

        /**
         * Determines the image of the given source pentachoron under
         * this isomorphism.
         *
         * \ifacespython Not present, though the read-only version of this
         * routine is.
         *
         * @param sourcePent the index of the source pentachoron; this must
         * be between 0 and <tt>getSourcePentachora()-1</tt> inclusive.
         * @return a reference to the index of the destination pentachoron
         * that the source pentachoron maps to.
         */
        int& pentImage(unsigned sourcePent);
        /**
         * Determines the image of the given source pentachoron under
         * this isomorphism.
         *
         * @param sourcePent the index of the source pentachoron; this must
         * be between 0 and <tt>getSourcePentachora()-1</tt> inclusive.
         * @return the index of the destination pentachoron
         * that the source pentachoron maps to.
         */
        int pentImage(unsigned sourcePent) const;
        /**
         * Returns a read-write reference to the permutation that is
         * applied to the five facets of the given source pentachoron
         * under this isomorphism.
         * Facet \a i of source pentachoron \a sourcePent will be mapped to
         * facet <tt>facetPerm(sourcePent)[i]</tt> of pentachoron
         * <tt>pentImage(sourcePent)</tt>.
         *
         * \ifacespython Not present, though the read-only version of this
         * routine is.
         *
         * @param sourcePent the index of the source pentachoron containing
         * the original five facets; this must be between 0 and
         * <tt>getSourcePentachora()-1</tt> inclusive.
         * @return a read-write reference to the permutation applied to the
         * five facets of the source pentachoron.
         */
        NPerm5& facetPerm(unsigned sourcePent);
        /**
         * Determines the permutation that is applied to the five facets
         * of the given source pentachoron under this isomorphism.
         * Facet \a i of source pentachoron \a sourcePent will be mapped to
         * face <tt>facetPerm(sourcePent)[i]</tt> of pentachoron
         * <tt>pentImage(sourcePent)</tt>.
         *
         * @param sourcePent the index of the source pentachoron containing
         * the original five facets; this must be between 0 and
         * <tt>getSourcePentachora()-1</tt> inclusive.
         * @return the permutation applied to the five facets of the
         * source pentachoron.
         */
        NPerm5 facetPerm(unsigned sourcePent) const;
        /**
         * Determines the image of the given source pentachoron facet
         * under this isomorphism.  Note that a value only is returned; this
         * routine cannot be used to alter the isomorphism.
         *
         * @param source the given source pentachoron facet; this must
         * be one of the five facets of one of the getSourcePentachora()
         * pentachora in the source triangulation.
         * @return the image of the source pentachoron facet under this
         * isomorphism.
         */
        Dim4PentFacet operator [] (const Dim4PentFacet& source) const;

        /**
         * Determines whether or not this is an identity isomorphism.
         *
         * In an identity isomorphism, each pentachoron image is itself,
         * and within each pentachoron the facet/vertex permutation is
         * the identity on (0,1,2,3,4).
         *
         * @return \c true if this is an identity isomorphism, or
         * \c false otherwise.
         */
        bool isIdentity() const;

        /**
         * Applies this isomorphism to the given triangulation.
         *
         * The given triangulation (call this T) is not modified in any way.
         * A new triangulation (call this S) is returned, so that this
         * isomorphism represents a one-to-one, onto and boundary complete
         * isomorphism from T to S.  That is, T and S are combinatorially
         * identical triangulations, and this isomorphism describes the
         * corresponding mapping between pentachora and pentachoron faces.
         *
         * The resulting triangulation S is newly created, and must be
         * destroyed by the caller of this routine.
         *
         * There are several preconditions to this routine.  This
         * routine does a small amount of sanity checking (and returns 0
         * if an error is detected), but it certainly does not check the
         * entire set of preconditions.  It is up to the caller of this
         * routine to verify that all of the following preconditions are
         * met.
         *
         * \pre The number of pentachora in the given triangulation is
         * precisely the number returned by getSourcePentachora() for
         * this isomorphism.
         * \pre This is a valid isomorphism (i.e., it has been properly
         * initialised, so that all pentachoron images are non-negative
         * and distinct, and all facet permutations are real permutations
         * of (0,1,2,3,4).
         * \pre Each pentachoron image for this isomorphism lies
         * between 0 and <tt>getSourcePentachora()-1</tt> inclusive
         * (i.e., this isomorphism does not represent a mapping from a
         * smaller triangulation into a larger triangulation).
         *
         * @param original the triangulation to which this isomorphism
         * should be applied.
         * @return the resulting new triangulation, or 0 if a problem
         * was encountered (i.e., an unmet precondition was noticed).
         */
        Dim4Triangulation* apply(const Dim4Triangulation* original) const;

        void writeTextShort(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;
};

/*@}*/

// Inline functions for Dim4Isomorphism

inline Dim4Isomorphism::Dim4Isomorphism(unsigned sourcePentachora) :
        nPentachora_(sourcePentachora),
        pentImage_(sourcePentachora > 0 ? new int[sourcePentachora] : 0),
        facetPerm_(sourcePentachora > 0 ? new NPerm5[sourcePentachora] : 0) {
}
inline Dim4Isomorphism::~Dim4Isomorphism() {
    // Always safe to delete null.
    delete[] pentImage_;
    delete[] facetPerm_;
}

inline unsigned Dim4Isomorphism::getSourcePentachora() const {
    return nPentachora_;
}

inline int& Dim4Isomorphism::pentImage(unsigned sourcePent) {
    return pentImage_[sourcePent];
}
inline int Dim4Isomorphism::pentImage(unsigned sourcePent) const {
    return pentImage_[sourcePent];
}
inline NPerm5& Dim4Isomorphism::facetPerm(unsigned sourcePent) {
    return facetPerm_[sourcePent];
}
inline NPerm5 Dim4Isomorphism::facetPerm(unsigned sourcePent) const {
    return facetPerm_[sourcePent];
}
inline Dim4PentFacet Dim4Isomorphism::operator [] (
        const Dim4PentFacet& source) const {
    return Dim4PentFacet(pentImage_[source.pent],
        facetPerm_[source.pent][source.facet]);
}

} // namespace regina

#endif

