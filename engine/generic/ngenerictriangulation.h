
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

/*! \file generic/ngenerictriangulation.h
 *  \brief Generic code for working with triangulations of arbitrary dimension.
 */

#ifndef __NGENERICTRIANGULATION_H
#ifndef __DOXYGEN
#define __NGENERICTRIANGULATION_H
#endif

#include "regina-core.h"
#include "generic/dimtraits.h"
#include <list>
#include <memory>
#include <string>

namespace regina {

/**
 * \weakgroup generic
 * @{
 */

/**
 * A generic helper class for working with triangulations of arbitrary
 * dimension.
 *
 * This class is designed to implement member functions of the various
 * triangulation classes in a unified, dimension-agnostic manner.
 *
 * End users should not use this class directly.  Instead they should
 * call the corresponding member functions from the corresponding
 * triangulation classes (NTriangulation and so on).
 *
 * \pre The template argument \a dim is one of Regina's
 * \ref stddim "standard dimensions".
 *
 * \headers Parts of this template class are implemented in separate headers,
 * which are not included automatically by this file.  However, typical
 * end users should never need these extra headers, since Regina's
 * calculation engine already includes explicit instantiations
 * for \ref stddim "standard dimensions".
 *
 * \apinotfinal
 *
 * \ifacespython Not present.
 */
template <int dim>
class REGINA_API NGenericTriangulation : public DimTraits<dim> {
    public:
        using typename DimTraits<dim>::Isomorphism;
        using typename DimTraits<dim>::Perm;
        using typename DimTraits<dim>::Simplex;
        using typename DimTraits<dim>::Triangulation;

    public:
        /**
         * \name Basic Properties
         */
        /*@{*/

        /**
         * Determines whether this triangulation is empty.
         * An empty triangulation is one with no simplices at all.
         *
         * @return \c true if and only if this triangulation is empty.
         */
        bool isEmpty() const;

        /**
         * Returns the number of top-dimensional simplices in the
         * triangulation.
         *
         * This is identical to getNumberOfSimplices(), but with two extra
         * advantages: (i) it is shorter to type; and (ii) it appears
         * in the template base class NGenericTriangulation, which may
         * be useful for templated code.
         *
         * In three dimensions, this routine is equivalent to calling
         * NTriangulation::getNumberOfTetrahedra().
         *
         * @return The number of top-dimensional simplices.
         */
        unsigned long size() const;

        /*@}*/
        /**
         * \name Isomorphism Testing
         */
        /*@{*/

        /**
         * Determines if this triangulation is combinatorially identical
         * to the given triangulation.
         *
         * Here "identical" means that the triangulations have the same
         * number of top-dimensional simplices, with gluings between the same
         * pairs of numbered simplices using the same gluing permutations.
         * In other words, "identical" means that the triangulations
         * are isomorphic via the identity isomorphism.
         *
         * To test for the less strict combinatorial isomorphism (which
         * allows relabelling of the top-dimensional simplices and their
         * vertices), see isIsomorphicTo() instead.
         *
         * This test does \e not examine the textual simplex descriptions,
         * as seen in Simplex::getDescription(); these may still differ.
         * It also does not test the numbering of vertices, edges and so on,
         * as used by getVertex(), getEdge() and so on;
         * although at the time of writing these will always be
         * numbered the same for identical triangulations, it is
         * conceivable that in future versions of Regina there may
         * be situations in which identical triangulations can acquire
         * different numberings for vertices, edges, etc.
         *
         * @param other the triangulation to compare with this one.
         * @return \c true if and only if the two triangulations are
         * combinatorially identical.
         */
        bool isIdenticalTo(const typename DimTraits<dim>::Triangulation& other)
            const;

        /**
         * Determines if this triangulation is combinatorially
         * isomorphic to the given triangulation.
         *
         * Specifically, this routine determines if there is a
         * one-to-one and onto boundary complete combinatorial
         * isomorphism from this triangulation to \a other.  Boundary
         * complete isomorphisms are described in detail in the
         * Isomorphism class notes.
         *
         * In particular, note that this triangulation and \a other must
         * contain the same number of top-dimensional simplices for such an
         * isomorphism to exist.
         *
         * If you need to ensure that top-dimensional simplices are labelled
         * the same in both triangulations, see the stricter test
         * isIdenticalTo() instead.
         *
         * If a boundary complete isomorphism is found, the details of
         * this isomorphism are returned.  The isomorphism is newly
         * constructed, and so to assist with memory management is
         * returned as a std::auto_ptr.  Thus, to test whether an
         * isomorphism exists without having to explicitly deal with the
         * isomorphism itself, you can call
         * <tt>if (isIsomorphicTo(other).get())</tt> and the newly
         * created isomorphism (if it exists) will be automatically
         * destroyed.
         *
         * If more than one such isomorphism exists, only one will be
         * returned.  For a routine that returns all such isomorphisms,
         * see findAllIsomorphisms().
         *
         * \todo \opt Improve the complexity by choosing a simplex
         * mapping from each component and following gluings to
         * determine the others.
         *
         * @param other the triangulation to compare with this one.
         * @return details of the isomorphism if the two triangulations
         * are combinatorially isomorphic, or a null pointer otherwise.
         */
        std::auto_ptr<typename DimTraits<dim>::Isomorphism> isIsomorphicTo(
            const typename DimTraits<dim>::Triangulation& other) const;

        /**
         * Determines if an isomorphic copy of this triangulation is
         * contained within the given triangulation, possibly as a
         * subcomplex of some larger component (or components).
         *
         * Specifically, this routine determines if there is a boundary
         * incomplete combinatorial isomorphism from this triangulation
         * to \a other.  Boundary incomplete isomorphisms are described
         * in detail in the Isomorphism class notes.
         *
         * In particular, note that facets of top-dimensional simplices that
         * lie on the boundary of this triangulation need not correspond to
         * boundary facets of \a other, and that \a other may contain more
         * top-dimensional simplices than this triangulation.
         *
         * If a boundary incomplete isomorphism is found, the details of
         * this isomorphism are returned.  The isomorphism is newly
         * constructed, and so to assist with memory management is
         * returned as a std::auto_ptr.  Thus, to test whether an
         * isomorphism exists without having to explicitly deal with the
         * isomorphism itself, you can call
         * <tt>if (isContainedIn(other).get())</tt> and the newly
         * created isomorphism (if it exists) will be automatically
         * destroyed.
         *
         * If more than one such isomorphism exists, only one will be
         * returned.  For a routine that returns all such isomorphisms,
         * see findAllSubcomplexesIn().
         *
         * @param other the triangulation in which to search for an
         * isomorphic copy of this triangulation.
         * @return details of the isomorphism if such a copy is found,
         * or a null pointer otherwise.
         */
        std::auto_ptr<typename DimTraits<dim>::Isomorphism> isContainedIn(
            const typename DimTraits<dim>::Triangulation& other) const;

        /**
         * Finds all ways in which this triangulation is combinatorially
         * isomorphic to the given triangulation.
         *
         * This routine behaves identically to isIsomorphicTo(), except that
         * instead of returning just one isomorphism, all such isomorphisms
         * are returned.
         *
         * See the isIsomorphicTo() notes for additional information.
         *
         * The isomorphisms that are found will be inserted into the
         * given list.  These isomorphisms will be newly created, and
         * the caller of this routine is responsible for destroying
         * them.  The given list will not be emptied before the new
         * isomorphisms are inserted.
         *
         * \ifacespython Not present.
         *
         * @param other the triangulation to compare with this one.
         * @param results the list in which any isomorphisms found will
         * be stored.
         * @return the number of isomorphisms that were found.
         */
        unsigned long findAllIsomorphisms(
            const typename DimTraits<dim>::Triangulation& other,
            std::list<typename DimTraits<dim>::Isomorphism*>& results) const;

        /**
         * Finds all ways in which an isomorphic copy of this triangulation
         * is contained within the given triangulation, possibly as a
         * subcomplex of some larger component (or components).
         *
         * This routine behaves identically to isContainedIn(), except
         * that instead of returning just one isomorphism (which may be
         * boundary incomplete and need not be onto), all such isomorphisms
         * are returned.
         *
         * See the isContainedIn() notes for additional information.
         *
         * The isomorphisms that are found will be inserted into the
         * given list.  These isomorphisms will be newly created, and
         * the caller of this routine is responsible for destroying
         * them.  The given list will not be emptied before the new
         * isomorphisms are inserted.
         *
         * \ifacespython Not present.
         *
         * @param other the triangulation in which to search for
         * isomorphic copies of this triangulation.
         * @param results the list in which any isomorphisms found will
         * be stored.
         * @return the number of isomorphisms that were found.
         */
        unsigned long findAllSubcomplexesIn(
            const typename DimTraits<dim>::Triangulation& other,
            std::list<typename DimTraits<dim>::Isomorphism*>& results) const;

        /**
         * Relabel the top-dimensional simplices and their vertices so that
         * this triangulation is in canonical form.  This is essentially
         * the lexicographically smallest labelling when the facet
         * gluings are written out in order.
         *
         * Two triangulations are isomorphic if and only if their canonical
         * forms are identical.
         *
         * The lexicographic ordering assumes that the facet gluings are
         * written in order of simplex index and then facet number.
         * Each gluing is written as the destination simplex index
         * followed by the gluing permutation (which in turn is written
         * as the images of 0,1,...,<i>dim</i> in order).
         *
         * \pre This routine currently works only when the triangulation
         * is connected.  It may be extended to work with disconnected
         * triangulations in later versions of Regina.
         *
         * @return \c true if the triangulation was changed, or \c false
         * if the triangulation was in canonical form to begin with.
         */
        bool makeCanonical();

        /*@}*/
        /**
         * \name Exporting Triangulations
         */
        /*@{*/

        /**
         * Constructs the isomorphism signature for this triangulation.
         *
         * An <i>isomorphism signature</i> is a compact text representation of
         * a triangulation.  Unlike dehydrations for 3-manifold triangulations,
         * an isomorphism signature uniquely determines a triangulation up
         * to combinatorial isomorphism (assuming the dimension is known in
         * advance).
         * That is, two triangulations of dimension \a dim are combinatorially
         * isomorphic if and only if their isomorphism signatures are the same.
         *
         * The isomorphism signature is constructed entirely of printable
         * characters, and has length proportional to <tt>n log n</tt>,
         * where \a n is the number of top-dimenisonal simplices.
         *
         * Isomorphism signatures are more general than dehydrations:
         * they can be used with any triangulation (including closed,
         * bounded and/or disconnected triangulations, as well
         * as triangulations with large numbers of triangles).
         *
         * The time required to construct the isomorphism signature of a
         * triangulation is <tt>O(n^2 log^2 n)</tt>.
         *
         * The routine fromIsoSig() can be used to recover a
         * triangulation from an isomorphism signature.  The triangulation
         * recovered might not be identical to the original, but it will be
         * combinatorially isomorphic.
         *
         * If \a relabelling is non-null (i.e., it points to some
         * Isomorphism pointer \a p), then it will be modified to point
         * to a new isomorphism that describes the precise relationship
         * between this triangulation and the reconstruction from fromIsoSig().
         * Specifically, the triangulation that is reconstructed from
         * fromIsoSig() will be combinatorially identical to
         * <tt>relabelling.apply(this)</tt>.
         *
         * For a full and precise description of the isomorphism signature
         * format for 3-manifold triangulations, see <i>Simplification paths
         * in the Pachner graphs of closed orientable 3-manifold
         * triangulations</i>, Burton, 2011, <tt>arXiv:1110.6080</tt>.
         * The format for other dimensions is essentially the same, but with
         * minor dimension-specific adjustments.
         *
         * \ifacespython The isomorphism argument is not present.
         * Instead there are two routines: fromIsoSig(), which returns a
         * string only, and fromIsoSigDetail(), which returns a pair
         * (signature, relabelling).
         *
         * \pre If \a relabelling is non-null, then this triangulation
         * must be non-empty and connected.  The facility to return a
         * relabelling for disconnected triangulations may be added to
         * Regina in a later release.
         *
         * \warning Do not mix isomorphism signatures between dimensions!
         * It is possible that the same string could corresponding to both a
         * \a p-dimensional triangulation and a \a q-dimensional triangulation
         * for different \a p and \a q.
         *
         * @param relabelling if non-null, this will be modified to point to a
         * new isomorphism describing the relationship between this
         * triangulation and that reconstructed from fromIsoSig(), as
         * described above.
         * @return the isomorphism signature of this triangulation.
         */
        std::string isoSig(
            typename DimTraits<dim>::Isomorphism** relabelling = 0) const;

        /*@}*/
        /**
         * \name Importing Triangulations
         */
        /*@{*/

        /**
         * Recovers a full triangulation from an isomorphism signature.
         *
         * See isoSig() for more information on isomorphism signatures.
         * It will be assumed that the signature describes a triangulation of
         * dimension \a dim.
         *
         * The triangulation that is returned will be newly created.
         *
         * Calling isoSig() followed by fromIsoSig() is not guaranteed to
         * produce an identical triangulation to the original, but it
         * \e is guaranteed to produce a combinatorially isomorphic
         * triangulation.
         *
         * For a full and precise description of the isomorphism signature
         * format for 3-manifold triangulations, see <i>Simplification paths
         * in the Pachner graphs of closed orientable 3-manifold
         * triangulations</i>, Burton, 2011, <tt>arXiv:1110.6080</tt>.
         * The format for other dimensions is essentially the same, but with
         * minor dimension-specific adjustments.
         *
         * \warning Do not mix isomorphism signatures between dimensions!
         * It is possible that the same string could corresponding to both a
         * \a p-dimensional triangulation and a \a q-dimensional triangulation
         * for different \a p and \a q.
         *
         * @param sig the isomorphism signature of the
         * triangulation to construct.  Note that, unlike dehydration
         * strings for 3-manifold triangulations, case is important for
         * isomorphism signatures.
         * @return a newly allocated triangulation if the reconstruction was
         * successful, or null if the given string was not a valid
         * isomorphism signature.
         */
        static typename DimTraits<dim>::Triangulation* fromIsoSig(
            const std::string& sig);

        /**
         * Deduces the number of top-dimensional simplices in a
         * connected triangulation from its isomorphism signature.
         *
         * See isoSig() for more information on isomorphism signatures.
         * It will be assumed that the signature describes a triangulation of
         * dimension \a dim.
         *
         * If the signature describes a connected triangulation, this
         * routine will simply return the size of that triangulation
         * (e.g., the number of tetrahedra in the case \a dim = 3).
         * You can also pass an isomorphism signature that describes a
         * disconnected triangulation; however, this routine will only
         * return the number of simplices in the first connected component.
         * If you need the total number of simplices in a disconnected
         * triangulation, you will need to reconstruct the full triangulation
         * by calling fromIsoSig() instead.
         *
         * This routine is very fast, since it only examines the first
         * few characters of the isomorphism signature (in which the size
         * of the first component is encoded).  However, it is therefore
         * possible to pass an invalid isomorphism signature and still
         * receive a positive result.  If you need to \e test whether a
         * signature is valid or not, you must call fromIsoSig()
         * instead, which will examine the entire signature in full.
         *
         * \warning Do not mix isomorphism signatures between dimensions!
         * It is possible that the same string could corresponding to both a
         * \a p-dimensional triangulation and a \a q-dimensional triangulation
         * for different \a p and \a q.
         *
         * @param sig an isomorphism signature of a \a dim-dimensional
         * triangulation.  Note that, unlike dehydration strings for
         * 3-manifold triangulations, case is important for isomorphism
         * signatures.
         * @return the number of top-dimensional simplices in the first
         * connected component, or 0 if this could not be determined
         * because the given string was not a valid isomorphism signature.
         */
        static size_t isoSigComponentSize(const std::string& sig);

        /*@}*/

    private:
        /**
         * Internal to isoSig().
         *
         * Constructs a candidate isomorphism signature for a single
         * component of this triangulation.  This candidate signature
         * assumes that the given simplex with the given labelling
         * of its vertices becomes simplex zero with vertices 0..dim
         * under the "canonical isomorphism".
         *
         * @param simp the index of some simplex in this triangulation.
         * @param vertices some ordering of the vertices of the
         * given tetrahedron.
         * @param if this is non-null, it will be filled with the canonical
         * isomorphism; in this case it must already have been constructed
         * for the correct number of simplices.
         * @return the candidate isomorphism signature.
         */
        static std::string isoSigFrom(
            const typename DimTraits<dim>::Triangulation& tri,
            unsigned simp,
            const typename DimTraits<dim>::Perm& vertices,
            typename DimTraits<dim>::Isomorphism* relabelling);
};

/**
 * A function object used for sorting faces of triangulations by
 * increasing degree.  This can (for instance) be used with std::sort().
 *
 * The template argument \a dim refers to the dimension of the overall
 * triangluation(s) with which you are working.  The template argument
 * \a subdim refers to the dimension of the faces that you are sorting.
 * So, for instance, to sort edges of a 3-manifold triangulation by
 * increasing edge degree, you would use DegreeLessThan<3, 1>.
 *
 * A single instance of this class works with faces of a single
 * fixed triangulation (which is passed to the class constructor).
 *
 * \pre \a dim is one of the supported triangulation dimensions.
 * \pre \a subdim is between 0 and \a dim-1 inclusive.
 */
template <int dim, int subdim>
class REGINA_API DegreeLessThan {
    private:
        const typename DimTraits<dim>::Triangulation& tri_;
            /**< The triangulation with which we are working. */

    public:
        /**
         * Constructions a function object for working with faces of the
         * given triangulation.
         *
         * @param tri the triangulation with which we are working.
         */
        DegreeLessThan(const typename DimTraits<dim>::Triangulation& tri);
        /**
         * Compares the degrees of the \a subdim-dimensional faces
         * at the given indices within the working triangulation.
         * The triangulation that is used will be the one that was
         * passed to the class constructor.
         *
         * \pre Both \a a and \a b are non-negative, and are strictly
         * less than the total number of \a subdim-dimensional faces in
         * the triangulation.
         *
         * @param a the index of the first \a subdim-dimensional face
         * within the triangulation.
         * @param b the index of the second \a subdim-dimensional face
         * within the triangulation.
         * @return \c true if and only if face \a a has smaller degree than
         * face \a b within the given triangulation.
         */
        bool operator() (unsigned a, unsigned b) const;
};

/**
 * A function object used for sorting faces of triangulations by
 * decreasing degree.  This can (for instance) be used with std::sort().
 *
 * The template argument \a dim refers to the dimension of the overall
 * triangluation(s) with which you are working.  The template argument
 * \a subdim refers to the dimension of the faces that you are sorting.
 * So, for instance, to sort edges of a 3-manifold triangulation by
 * decreasing edge degree, you would use DegreeGreaterThan<3, 1>.
 *
 * A single instance of this class works with faces of a single
 * fixed triangulation (which is passed to the class constructor).
 *
 * \pre \a dim is one of the supported triangulation dimensions.
 * \pre \a subdim is between 0 and \a dim-1 inclusive.
 */
template <int dim, int subdim>
class REGINA_API DegreeGreaterThan {
    private:
        const typename DimTraits<dim>::Triangulation& tri_;
            /**< The triangulation with which we are working. */

    public:
        /**
         * Constructions a function object for working with faces of the
         * given triangulation.
         *
         * @param tri the triangulation with which we are working.
         */
        DegreeGreaterThan(const typename DimTraits<dim>::Triangulation& tri);
        /**
         * Compares the degrees of the \a subdim-dimensional faces
         * at the given indices within the working triangulation.
         * The triangulation that is used will be the one that was
         * passed to the class constructor.
         *
         * \pre Both \a a and \a b are non-negative, and are strictly
         * less than the total number of \a subdim-dimensional faces in
         * the triangulation.
         *
         * @param a the index of the first \a subdim-dimensional face
         * within the triangulation.
         * @param b the index of the second \a subdim-dimensional face
         * within the triangulation.
         * @return \c true if and only if face \a a has greater degree than
         * face \a b within the given triangulation.
         */
        bool operator() (unsigned a, unsigned b) const;
};

/*@}*/

// Help the compiler by noting which explicit instantiations we offer.
extern template class REGINA_API NGenericTriangulation<2>;
extern template class REGINA_API NGenericTriangulation<3>;

// Inline functions:

template <int dim>
inline bool NGenericTriangulation<dim>::isEmpty() const {
    return (static_cast<const typename DimTraits<dim>::Triangulation*>(this)->
        getNumberOfSimplices() == 0);
}

template <int dim>
inline unsigned long NGenericTriangulation<dim>::size() const {
    return static_cast<const typename DimTraits<dim>::Triangulation*>(this)->
        getNumberOfSimplices();
}

template <int dim, int subdim>
inline DegreeLessThan<dim, subdim>::DegreeLessThan(
        const typename DimTraits<dim>::Triangulation& tri) : tri_(tri) {
}

template <int dim, int subdim>
inline bool DegreeLessThan<dim, subdim>::operator () (
        unsigned a, unsigned b) const {
    return (tri_.template getFace<subdim>(a)->getNumberOfEmbeddings() <
            tri_.template getFace<subdim>(b)->getNumberOfEmbeddings());
}

template <int dim, int subdim>
inline DegreeGreaterThan<dim, subdim>::DegreeGreaterThan(
        const typename DimTraits<dim>::Triangulation& tri) : tri_(tri) {
}

template <int dim, int subdim>
inline bool DegreeGreaterThan<dim, subdim>::operator () (
        unsigned a, unsigned b) const {
    return (tri_.template getFace<subdim>(a)->getNumberOfEmbeddings() >
            tri_.template getFace<subdim>(b)->getNumberOfEmbeddings());
}

} // namespace regina

#endif

