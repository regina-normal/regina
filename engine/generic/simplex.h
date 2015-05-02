
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2015, Ben Burton                                   *
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

/*! \file generic/simplex.h
 *  \brief Deals with top-dimensional simplices in a triangulation.
 */

#ifndef __SIMPLEX_H
#ifndef __DOXYGEN
#define __SIMPLEX_H
#endif

#include "regina-core.h"
#include "output.h"
#include "generic/dimtraits.h"
#include "generic/policies.h"
#include "maths/nperm.h"
#include "utilities/nmarkedvector.h"
#include <cassert>
#include <boost/noncopyable.hpp>

namespace regina {

template <int dim>
class Simplex;

/**
 * \weakgroup generic
 * @{
 */

/**
 * Provides core functionality for a top-dimensional simplex in a
 * <i>dim</i>-manifold triangulation.
 *
 * Such a simplex is represented by the class Simplex<dim>, which uses this as
 * a base class.  End users should not need to refer to SimplexBase directly.
 *
 * See the Simplex template class notes for further information.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be at least 2.
 */
template <int dim>
class REGINA_API SimplexBase :
        public NMarkedElement,
        public Output<SimplexBase<dim>>,
        public boost::noncopyable {
    static_assert(dim >= 2, "SimplexBase requires dimension >= 2.");
    private:
        Simplex<dim>* adj_[dim + 1];
            /**< Stores the adjacent simplex glued to each facet of this
                 simplex.  Specifically, <tt>adj_[f]</tt> represents the
                 simplex joined to facet \a f of this simplex, or is 0
                 if facet \a f lies on the triangulation boundary. */
        NPerm<dim+1> gluing_[dim + 1];
            /**< Indicates how vertices map to each other across each gluing.
                 Specifically, if facet \a f is joined to some other simplex
                 (i.e., it is not boundary), then \a gluing_[\a f] represents
                 the induced mapping from vertices of this simplex to
                 vertices of the adjacent simplex. */
        std::string description_;
            /**< The description of this simplex, or the empty string
                 if there is no description. */

        typename DimTraits<dim>::Triangulation* tri_;
            /**< The triangulation to which this simplex belongs. */

    public:
        /**
         * Returns the description associated with this simplex.
         *
         * @return the description of this simplex, or the empty string
         * if no description is stored.
         */
        const std::string& getDescription() const;

        /**
         * Sets the description associated with this simplex.
         *
         * This may be any text whatsoever; typically it is intended to
         * be human-readable.  Descriptions do not need to be unique.
         *
         * To remove an existing description, you can simply set the
         * description to the empty string.
         *
         * @param desc the new description to assign to this simplex.
         */
        void setDescription(const std::string& desc);

        /**
         * Returns the index of this simplex in the underlying triangulation.
         * This is identical to calling
         * <tt>getTriangulation()->simplexIndex(this)</tt>.
         *
         * The index will be an integer between 0 and
         * <tt>getTriangulation()->size()-1</tt> inclusive.
         *
         * Note that indexing may change when a simplex is added to or removed
         * from the underlying triangulation.
         *
         * @return the index of this simplex.
         */
        size_t index() const;

        // TODO: Documentation needs to be revised from here onwards.

        /**
         * Returns the adjacent tetrahedron glued to the given face of this
         * tetrahedron, or 0 if the given face is on the triangulation
         * boundary.
         *
         * Here "simplex" refers to a top-dimensional simplex (which for
         * 3-manifold triangulations means a tetrahedron).
         *
         * @param face the face of this tetrahedron to examine.  This
         * should be between 0 and 3 inclusive, where face \c i is
         * opposite vertex \c i of the tetrahedron.
         * @return the adjacent tetrahedron glued to the given face, or 0
         * if the given face lies on the boundary.
         */
        Simplex<dim>* adjacentSimplex(int face) const;

        /**
         * Returns a permutation describing the correspondence between
         * vertices of this tetrahedron and vertices of the adjacent
         * tetrahedron glued to the given face of this tetrahedron.
         *
         * If we call this permutation \c p, then for each vertex \c v of this
         * tetrahedron, <tt>p[v]</tt> will be the vertex of the adjacent
         * tetrahedron that is identified with \c v according to the gluing
         * along the given face of this tetrahedron.
         *
         * \pre The given face of this tetrahedron has some tetrahedron
         * (possibly this one) glued to it.
         *
         * @param face the face of this tetrahedron whose gluing we
         * will examine.  This should be between 0 and 3 inclusive, where
         * face \c i is opposite vertex \c i of the tetrahedron.
         * @return a permutation mapping the vertices of this
         * tetrahedron to the vertices of the tetrahedron adjacent along
         * the given face.
         */
        NPerm<dim+1> adjacentGluing(int face) const;
        /**
         * Examines the tetrahedron glued to the given face of this
         * tetrahedron, and returns the corresponding face of that
         * tetrahedron.  That is, the returned face of the adjacent
         * tetrahedron is glued to the given face of this tetrahedron.
         *
         * Here "facet" refers to a facet of a top-dimensional simplex
         * (e.g., for a 3-manifold triangulations this means a 2-face of
         * a tetrahedron).
         *
         * \pre The given face of this tetrahedron has some tetrahedron
         * (possibly this one) glued to it.
         *
         * @param face the face of this tetrahedron whose gluing we
         * will examine.  This
         * should be between 0 and 3 inclusive, where face \c i is
         * opposite vertex \c i of the tetrahedron.
         * @return the face of the tetrahedron adjacent along the given
         * face that is in fact glued to the given face of this
         * tetrahedron.
         */
        int adjacentFacet(int facet) const;
        /**
         * Determines if this tetrahedron has any faces that are
         * boundary triangles.
         *
         * @return \c true if and only if this tetrahedron has any
         * boundary triangles.
         */
        bool hasBoundary() const;

        /**
         * Joins the given face of this tetrahedron to another
         * tetrahedron.  The other tetrahedron involved will be
         * automatically updated.
         *
         * Neither tetrahedron needs to belong to a triangulation (i.e.,
         * you can join tetrahedra together before or after calling
         * NTriangulation::addTetrahedron()).  However, if both
         * tetrahedra do belong to a triangulation then it must be the
         * \e same triangulation.
         *
         * \pre This and the given tetrahedron do not belong to
         * different triangulations.
         * \pre The given face of this tetrahedron is not currently glued to
         * anything.
         * \pre The face of the other tetrahedron that will be glued to the
         * given face of this tetrahedron is not currently glued to anything.
         * \pre If the other tetrahedron involved is this tetrahedron, we are
         * not attempting to glue a face to itself.
         *
         * @param myFacet the face of this tetrahedron that will be glued
         * to the given other tetrahedron.  This
         * should be between 0 and 3 inclusive, where face \c i is
         * opposite vertex \c i of the tetrahedron.
         * @param you the tetrahedron (possibly this one) that will be
         * glued to the given face of this tetrahedron.
         * @param gluing a permutation describing the mapping of
         * vertices by which the two tetrahedra will be joined.  Each
         * vertex \c v of this tetrahedron that lies on the given face will
         * be identified with vertex <tt>gluing[v]</tt> of tetrahedron
         * <tt>you</tt>.  In addition, the face of <tt>you</tt> that
         * will be glued to the given face of this tetrahedron will be
         * face number <tt>gluing[myFacet]</tt>.
         */
        void joinTo(int myFacet, Simplex<dim>* you, NPerm<dim+1> gluing);
        // TODO: Documentation needs to be revised above this point.
        /**
         * Unglues the given facet of this simplex from whatever it is
         * joined to.  As a result, the given facet of this simplex
         * will become a boundary facet.
         *
         * If there was an adjacent simplex to begin with, then this other
         * simplex will be updated automatically (i.e., you only need to
         * call unjoin() from one side of the gluing).
         *
         * This routine is safe to call even if the given facet is
         * already a boundary facet (in which case it will do nothing).
         *
         * @param myFacet the facet of this simplex whose gluing we
         * will undo.  This should be between 0 and \a dim inclusive.
         * @return the simplex that was originally glued to the given facet
         * of this simplex, or 0 if this was already a boundary facet.
         */
        Simplex<dim>* unjoin(int myFacet);
        /**
         * Unglues this simplex from any adjacent simplices.
         * As a result, every facet of this simplex will become a boundary
         * facet, and this simplex will form its own separate component
         * of the underlying triangulation.
         *
         * If there were any adjacent simplices to begin with, these
         * will be updated automatically.
         *
         * This routine is safe to call even if there are no adjacent
         * simplices (in which case it will do nothing).
         */
        void isolate();

        /**
         * Returns the triangulation to which this simplex belongs.
         *
         * @return the triangulation containing this simplex.
         */
        typename DimTraits<dim>::Triangulation* getTriangulation() const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

    protected:
        /**
         * Creates a new simplex with no description and no facets joined
         * to anything.
         *
         * @param tri the triangulation to which the new simplex belongs.
         */
        SimplexBase(typename DimTraits<dim>::Triangulation* tri);
        /**
         * Creates a new simplex with the given description and no facets
         * joined to anything.
         *
         * @param desc the description to give the new simplex.
         * @param tri the triangulation to which the new simplex belongs.
         */
        SimplexBase(const std::string& desc,
            typename DimTraits<dim>::Triangulation* tri);

    friend typename DimTraits<dim>::Triangulation;
};

/**
 * Represents a top-dimensional simplex in a <i>dim</i>-manifold triangulation.
 *
 * For example, for 3-manifolds this class represents a tetrahedron, and for
 * 2-manifolds this class represents a triangle.
 *
 * Top-dimensional simplices cannot exist in isolation (without a
 * triangulation object), and they cannot be created or destroyed directly.
 * Instead, you create and destroy them through the underlying triangulation,
 * by calling routines such as Triangulation<dim>::newSimplex() or
 * Triangulation<dim>::removeSimplex().
 *
 * Amongst other things, this class is used to view and change the gluings
 * between top-dimensional simplices.  For this we number the facets and
 * vertices of each simplex 0,...,\a dim, so that facet \a i is opposite
 * vertex \a i.
 *
 * Each simplex may have an optional description.  This is typically a
 * human-readable piece of text.  Descriptions are not required, and do
 * not need to be unique.
 *
 * For \a dim = 2, 3 and 4, this template is specialised and offers
 * significant extra functionality.  In order to use these specialised
 * classes, you will need to include the corresponding headers (e.g.,
 * dim2/dim2triangle.h for \a dim = 2, or triangulation/ntetrahedron.h
 * for \a dim = 3).  Otherwise any code that uses Simplex<2>, Simplex<3> or
 * Simplex<4> will not compile.  For convenience, the typedefs Dim2Triangle,
 * NTetrahedron and Dim4Pentachoron are also available for these
 * specialised classes.
 *
 * \ifacespython Python does not support templates.  For \a dim = 2, 3 and 4,
 * this class is available in Python under the name Simplex2, Simplex3 or
 * Simplex4 respectively.  Higher-dimensional classes are not available in
 * Python for the time being.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be at least 2.
 */
template <int dim>
class REGINA_API Simplex : public SimplexBase<dim> {
    protected:
        /**
         * Creates a new simplex with no description and no facets joined
         * to anything.
         *
         * @param tri the triangulation to which the new simplex belongs.
         */
        Simplex(typename DimTraits<dim>::Triangulation* tri);
        /**
         * Creates a new simplex with the given description and no facets
         * joined to anything.
         *
         * @param desc the description to give the new simplex.
         * @param tri the triangulation to which the new simplex belongs.
         */
        Simplex(const std::string& desc,
            typename DimTraits<dim>::Triangulation* tri);
};

// Note that some of our simplex classes are specialised elsewhere.
// Do not explicitly drag in the specialised headers for now.
template <> class Simplex<2>;
template <> class Simplex<3>;
template <> class Simplex<4>;

/*@}*/

// Inline functions for SimplexBase

template <int dim>
inline SimplexBase<dim>::SimplexBase(
        typename DimTraits<dim>::Triangulation* tri) : tri_(tri) {
    for (int i = 0; i <= dim; ++i)
        adj_[i] = 0;
}

template <int dim>
inline SimplexBase<dim>::SimplexBase(const std::string& desc,
        typename DimTraits<dim>::Triangulation* tri) :
        description_(desc), tri_(tri) {
    for (int i = 0; i <= dim; ++i)
        adj_[i] = 0;
}

template <int dim>
inline const std::string& SimplexBase<dim>::getDescription() const {
    return description_;
}

template <int dim>
inline void SimplexBase<dim>::setDescription(const std::string& desc) {
    ChangeEventSpan<typename DimTraits<dim>::Triangulation> span(tri_);
    description_ = desc;
}

template <int dim>
inline size_t SimplexBase<dim>::index() const {
    return markedIndex();
}

template <int dim>
inline Simplex<dim>* SimplexBase<dim>::adjacentSimplex(int facet) const {
    return adj_[facet];
}

template <int dim>
inline int SimplexBase<dim>::adjacentFacet(int facet) const {
    return gluing_[facet][facet];
}

template <int dim>
inline NPerm<dim+1> SimplexBase<dim>::adjacentGluing(
        int face) const {
    return gluing_[face];
}

template <int dim>
inline typename DimTraits<dim>::Triangulation*
        SimplexBase<dim>::getTriangulation() const {
    return tri_;
}

template <int dim>
inline void SimplexBase<dim>::writeTextShort(std::ostream& out) const {
    out << dim << "-simplex";
    if (! description_.empty())
        out << ": " << description_;
}

template <int dim>
bool SimplexBase<dim>::hasBoundary() const {
    for (int i = 0; i <= dim; ++i)
        if (! adj_[i])
            return true;
    return false;
}

template <int dim>
void SimplexBase<dim>::isolate() {
    for (int i = 0; i <= dim; ++i)
        if (adj_[i])
            unjoin(i);
}

template <int dim>
Simplex<dim>* SimplexBase<dim>::unjoin(int myFacet) {
    if (! adj_[myFacet])
        return 0;

    ChangeEventSpan<typename DimTraits<dim>::Triangulation> span(tri_);

    Simplex<dim>* you = adj_[myFacet];
    int yourFacet = gluing_[myFacet][myFacet];
    assert(you->adj_[yourFacet] == this);
    you->adj_[yourFacet] = 0;
    adj_[myFacet] = 0;

    tri_->clearAllProperties();
    return you;
}

template <int dim>
void SimplexBase<dim>::joinTo(int myFacet, Simplex<dim>* you,
        NPerm<dim+1> gluing) {
    ChangeEventSpan<typename DimTraits<dim>::Triangulation> span(tri_);

    assert(tri_ == you->tri_);
    assert((! adj_[myFacet]) ||
        (adj_[myFacet] == you && gluing_[myFacet] == gluing));

    adj_[myFacet] = you;
    gluing_[myFacet] = gluing;
    int yourFacet = gluing[myFacet];
    assert((! you->adj_[yourFacet]) ||
        (you->adj_[yourFacet] == this &&
            you->gluing_[yourFacet] == gluing.inverse()));
    assert(! (you == this && yourFacet == myFacet));
    you->adj_[yourFacet] = static_cast<Simplex<dim>*>(this);
    you->gluing_[yourFacet] = gluing.inverse();

    tri_->clearAllProperties();
}

template <int dim>
void SimplexBase<dim>::writeTextLong(std::ostream& out) const {
    writeTextShort(out);
    out << std::endl;
    int facet, j;
    for (facet = dim; facet >= 0; --facet) {
        for (j = 0; j <= dim; ++j)
            if (j != facet)
                out << regina::digit(j);
        out << " -> ";
        if (! adj_[facet])
            out << "boundary";
        else {
            out << adj_[facet]->markedIndex() << " (";
            for (j = 0; j <= dim; ++j)
                if (j != facet)
                    out << regina::digit(gluing_[facet][j]);
            out << ')';
        }
        out << std::endl;
    }
}

// Inline functions for Simplex

template <int dim>
inline Simplex<dim>::Simplex(typename DimTraits<dim>::Triangulation* tri) :
        SimplexBase<dim>(tri) {
}

template <int dim>
inline Simplex<dim>::Simplex(const std::string& desc,
        typename DimTraits<dim>::Triangulation* tri) :
        SimplexBase<dim>(desc, tri) {
}

} // namespace regina

#endif

