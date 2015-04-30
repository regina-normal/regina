
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
#include "generic/dimtraits.h"
#include "generic/policies.h"
#include "maths/nperm.h"
#include "utilities/nmarkedvector.h"
#include <cassert>
#include <boost/noncopyable.hpp>

namespace regina {

/**
 * \weakgroup generic
 * @{
 */

/**
 * Represents a top-dimensional simplex in a <i>dim</i>-manifold triangulation.
 *
 * For example, for 3-manifolds this would be a tetrahedron, and for
 * 2-manifolds this would be a triangle.
 *
 * Top-dimensional simplices cannot exist in isolation (without a
 * triangulation object), and they cannot be created or destroyed directly.
 * Instead, you create and destroy them through the underlying triangulation,
 * by calling routines such as Triangulation<dim>::newSimplex() or
 * Triangulation<dim>::removeSimplex().
 *
 * \tparam dim the dimension of the underlying triangulation.
 */
template <int dim, bool isPacket = false>
class REGINA_API SimplexBase :
        public DimTraits<dim>,
        public NMarkedElement,
        public boost::noncopyable {
    public:
        using typename DimTraits<dim>::Triangulation;
        using typename DimTraits<dim>::Simplex;
        using typename DimTraits<dim>::Perm;

    private:
        Simplex* adj_[dim + 1];
            /**< Stores the adjacent simplex glued to each facet of this
                 simplex.  Specifically, <tt>adj_[f]</tt> represents the
                 simplex joined to facet \a f of this simplex, or is 0
                 if facet \a f lies on the triangulation boundary.  Facets are
                 numbered from 0 to \a dim inclusive, where facet \a i is
                 opposite vertex \a i of the simplex. */
        Perm gluing_[dim + 1];
            /**< Stores the corresponence between vertices of this simplex and
                 adjacent simplices.  If facet \a f is joined to some other
                 simplex (i.e., it is not boundary), then \a gluing_[\a f]
                 represents the permutation \a p whereby vertex \a v of
                 this simplex is identified with vertex \a p[\a v] of
                 the adjacent simplex. */
        std::string description_;
            /**< A text description of this simplex.
                 Descriptions are not mandatory and need not be unique. */

        Triangulation* tri_;
            /**< The triangulation to which this simplex belongs. */

    public:
        /**
         * Returns the text description associated with this simplex.
         *
         * @return the description of this simplex.
         */
        const std::string& getDescription() const;

        /**
         * Sets the text description associated with this simplex.
         * Note that descriptions need not be unique, and may be empty.
         *
         * @param desc the new description to assign to this simplex.
         */
        void setDescription(const std::string& desc);

        /**
         * Returns the index of this simplex in the underlying triangulation.
         * This is identical to calling
         * <tt>getTriangulation()->tetrahedronIndex(this)</tt>.
         *
         * Note that indexing may change when a simplex is added to or removed
         * from the underlying triangulation.
         *
         * @return the index of this simplex.
         */
        size_t index() const;

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
        Simplex* adjacentSimplex(int face) const;

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
        Perm adjacentGluing(int face) const;
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
        void joinTo(int myFacet, Simplex* you, Perm gluing);
        /**
         * Unglues the given face of this tetrahedron from whatever is
         * joined to it.  The other tetrahedron involved (possibly this
         * one) will be automatically updated.
         *
         * \pre The given face of this tetrahedron has some tetrahedron
         * (possibly this one) glued to it.
         *
         * @param myFacet the face of this tetrahedron whose gluing we
         * will undo.  This should be between 0 and 3 inclusive, where
         * face \c i is opposite vertex \c i of the tetrahedron.
         * @return the ex-adjacent tetrahedron that was originally glued
         * to the given face of this tetrahedron.
         */
        Simplex* unjoin(int myFacet);
        /**
         * Undoes any face gluings involving this tetrahedron.
         * Any other tetrahedra involved will be automatically updated.
         */
        void isolate();

        /**
         * Returns the triangulation to which this tetrahedron belongs.
         *
         * @return the triangulation containing this tetrahedron.
         */
        Triangulation* getTriangulation() const;

        std::string str() const;
        std::ostream&  str(std::ostream& out) const;
        std::string detail() const;
        std::ostream& detail(std::ostream& out) const;

    protected:
        /**
         * Creates a new simplex with an empty description and no
         * facets joined to anything.
         *
         * @param tri the triangulation to which the new simplex belongs.
         */
        SimplexBase(Triangulation* tri);
        /**
         * Creates a new triangle with the given description and
         * no facets joined to anything.
         *
         * @param desc the description to give the new simplex.
         * @param tri the triangulation to which the new simplex belongs.
         */
        SimplexBase(const std::string& desc, Triangulation* tri);

    friend Triangulation;
};

// TODO: Disambiguate str() from Dim2Triangle.

/*@}*/

// Inline functions for SimplexBase

template <int dim, bool isPacket>
inline SimplexBase<dim, isPacket>::SimplexBase(Triangulation* tri) : tri_(tri) {
    for (int i = 0; i <= dim; ++i)
        adj_[i] = 0;
}

template <int dim, bool isPacket>
inline SimplexBase<dim, isPacket>::SimplexBase(const std::string& desc,
        Triangulation* tri) : description_(desc), tri_(tri) {
    for (int i = 0; i <= dim; ++i)
        adj_[i] = 0;
}

template <int dim, bool isPacket>
inline const std::string& SimplexBase<dim, isPacket>::getDescription() const {
    return description_;
}

template <int dim, bool isPacket>
inline void SimplexBase<dim, isPacket>::setDescription(const std::string& desc) {
    ChangeEventSpan<isPacket> span(tri_);
    description_ = desc;
}

template <int dim, bool isPacket>
inline size_t SimplexBase<dim, isPacket>::index() const {
    return markedIndex();
}

template <int dim, bool isPacket>
inline typename SimplexBase<dim, isPacket>::Simplex* SimplexBase<dim, isPacket>::adjacentSimplex(
        int facet) const {
    return adj_[facet];
}

template <int dim, bool isPacket>
inline int SimplexBase<dim, isPacket>::adjacentFacet(int facet) const {
    return gluing_[facet][facet];
}

template <int dim, bool isPacket>
inline typename SimplexBase<dim, isPacket>::Perm SimplexBase<dim, isPacket>::adjacentGluing(
        int face) const {
    return gluing_[face];
}

template <int dim, bool isPacket>
inline typename SimplexBase<dim, isPacket>::Triangulation*
        SimplexBase<dim, isPacket>::getTriangulation() const {
    return tri_;
}

template <int dim, bool isPacket>
inline std::ostream& SimplexBase<dim, isPacket>::str(std::ostream& out) const {
    out << dim << "-simplex";
    if (! description_.empty())
        out << ": " << description_;
    return out;
}

template <int dim, bool isPacket>
bool SimplexBase<dim, isPacket>::hasBoundary() const {
    for (int i = 0; i <= dim; ++i)
        if (! adj_[i])
            return true;
    return false;
}

template <int dim, bool isPacket>
void SimplexBase<dim, isPacket>::isolate() {
    for (int i = 0; i <= dim; ++i)
        if (adj_[i])
            unjoin(i);
}

template <int dim, bool isPacket>
typename SimplexBase<dim, isPacket>::Simplex* SimplexBase<dim, isPacket>::unjoin(int myFacet) {
    ChangeEventSpan<isPacket> span(tri_);

    Simplex* you = adj_[myFacet];
    int yourFacet = gluing_[myFacet][myFacet];
    assert(you);
    assert(you->adj_[yourFacet]);
    you->adj_[yourFacet] = 0;
    adj_[myFacet] = 0;

    tri_->clearAllProperties();

    return you;
}

template <int dim, bool isPacket>
void SimplexBase<dim, isPacket>::joinTo(int myFacet, Simplex* you,
        Perm gluing) {
    ChangeEventSpan<isPacket> span(tri_);

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
    you->adj_[yourFacet] = static_cast<Simplex*>(this);
    you->gluing_[yourFacet] = gluing.inverse();

    tri_->clearAllProperties();
}

template <int dim, bool isPacket>
std::ostream& SimplexBase<dim, isPacket>::detail(std::ostream& out) const {
    str(out) << std::endl;
    for (int i = dim; i >= 0; --i) {
        // TODO: output.
        /*
        out << NTriangle::ordering[i].trunc3() << " -> ";
        if (! adj_[i])
            out << "boundary";
        else
            out << adj_[i]->markedIndex() << " ("
                << (gluing_[i] * NTriangle::ordering[i]).trunc3()
                << ')';
        out << std::endl;
        */
    }
    return out;
}

} // namespace regina

#endif

